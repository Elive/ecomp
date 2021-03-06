/*
 * Copyright © 2005 Novell, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Novell, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 * Novell, Inc. makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * NOVELL, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL NOVELL, INC. BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: David Reveman <davidr@novell.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include <X11/Xatom.h>
#include <X11/Xproto.h>

#include <cube.h>

static int cubeDisplayPrivateIndex;

#define ROTATE_POINTER_SENSITIVITY_FACTOR 0.05f

static CompMetadata rotateMetadata;

static int displayPrivateIndex;

#define ROTATE_DISPLAY_OPTION_INITIATE 0
#define ROTATE_DISPLAY_OPTION_NUM      1

typedef struct _RotateDisplay
{
   int             screenPrivateIndex;
   HandleEventProc handleEvent;

   CompOption      opt[ROTATE_DISPLAY_OPTION_NUM];
} RotateDisplay;

#define ROTATE_SCREEN_OPTION_POINTER_INVERT_Y    0
#define ROTATE_SCREEN_OPTION_POINTER_SENSITIVITY 1
#define ROTATE_SCREEN_OPTION_ACCELERATION        2
#define ROTATE_SCREEN_OPTION_SNAP_TOP            3
#define ROTATE_SCREEN_OPTION_SPEED               4
#define ROTATE_SCREEN_OPTION_TIMESTEP            5
#define ROTATE_SCREEN_OPTION_ZOOM                6
#define ROTATE_SCREEN_OPTION_NUM                 7

typedef struct _RotateScreen
{
   PreparePaintScreenProc preparePaintScreen;
   DonePaintScreenProc    donePaintScreen;
   PaintScreenProc        paintScreen;
   PaintOutputProc        paintOutput;
   WindowGrabNotifyProc   windowGrabNotify;
   WindowUngrabNotifyProc windowUngrabNotify;

   CubeGetRotationProc    getRotation;

   CompOption             opt[ROTATE_SCREEN_OPTION_NUM];

   float                  pointerSensitivity;

   Bool                   snapTop;

   int                    grabIndex;

   GLfloat                xrot, xVelocity;
   GLfloat                yrot, yVelocity;

   GLfloat                baseXrot;

   Bool                   moving;
   GLfloat                moveTo;

   Window                 moveWindow;
   int                    moveWindowX;

   XPoint                 savedPointer;
   Bool                   grabbed;

   CompTimeoutHandle      rotateHandle;
   Bool                   slow;
   unsigned int           grabMask;
   CompWindow            *grabWindow;

   GLfloat                zoomTranslate;
   GLfloat                zoomVelocity;

   int                    destX;
} RotateScreen;

#define GET_ROTATE_DISPLAY(d) \
  ((RotateDisplay *)(d)->privates[displayPrivateIndex].ptr)

#define ROTATE_DISPLAY(d) \
  RotateDisplay *rd = GET_ROTATE_DISPLAY (d)

#define GET_ROTATE_SCREEN(s, rd) \
  ((RotateScreen *)(s)->privates[(rd)->screenPrivateIndex].ptr)

#define ROTATE_SCREEN(s) \
  RotateScreen *rs = GET_ROTATE_SCREEN (s, GET_ROTATE_DISPLAY (s->display))

#define NUM_OPTIONS(s) (sizeof ((s)->opt) / sizeof (CompOption))

static CompOption *
rotateGetScreenOptions(CompPlugin *plugin,
                       CompScreen *screen,
                       int        *count)
{
   ROTATE_SCREEN (screen);

   *count = NUM_OPTIONS (rs);
   return rs->opt;
}

static Bool
rotateSetScreenOption(CompPlugin      *plugin,
                      CompScreen      *screen,
                      char            *name,
                      CompOptionValue *value)
{
   CompOption *o;
   int index;

   ROTATE_SCREEN (screen);

   o = compFindOption (rs->opt, NUM_OPTIONS (rs), name, &index);
   if (!o)
     return FALSE;

   switch (index) {
      case ROTATE_SCREEN_OPTION_POINTER_SENSITIVITY:
        if (compSetFloatOption (o, value))
          {
             rs->pointerSensitivity = o->value.f *
               ROTATE_POINTER_SENSITIVITY_FACTOR;
             return TRUE;
          }
        break;

      default:
        return compSetScreenOption (screen, o, value);
     }

   return FALSE;
}

static int
adjustVelocity(RotateScreen *rs,
               int           size)
{
   float xrot, yrot, adjust, amount;

   if (rs->moving)
     {
        xrot = rs->moveTo + (rs->xrot + rs->baseXrot);
     }
   else
     {
        xrot = rs->xrot;
        if (rs->xrot < -180.0f / size)
          xrot = 360.0f / size + rs->xrot;
        else if (rs->xrot > 180.0f / size)
          xrot = rs->xrot - 360.0f / size;
     }

   adjust = -xrot * 0.05f * rs->opt[ROTATE_SCREEN_OPTION_ACCELERATION].value.f;
   amount = fabs (xrot);
   if (amount < 10.0f)
     amount = 10.0f;
   else if (amount > 30.0f)
     amount = 30.0f;

   if (rs->slow)
     adjust *= 0.05f;

   rs->xVelocity = (amount * rs->xVelocity + adjust) / (amount + 2.0f);

   if (rs->snapTop && rs->yrot > 50.0f)
     yrot = -(90.f - rs->yrot);
   else
     yrot = rs->yrot;

   adjust = -yrot * 0.05f * rs->opt[ROTATE_SCREEN_OPTION_ACCELERATION].value.f;
   amount = fabs (rs->yrot);
   if (amount < 10.0f)
     amount = 10.0f;
   else if (amount > 30.0f)
     amount = 30.0f;

   rs->yVelocity = (amount * rs->yVelocity + adjust) / (amount + 2.0f);

   return fabs (xrot) < 0.1f && fabs (rs->xVelocity) < 0.2f &&
          fabs (yrot) < 0.1f && fabs (rs->yVelocity) < 0.2f;
}

static void
rotateReleaseMoveWindow(CompScreen *s)
{
   CompWindow *w;

   ROTATE_SCREEN (s);

   w = findWindowAtScreen (s, rs->moveWindow);
   if (w)
     syncWindowPosition (w);

   rs->moveWindow = None;
}

static void
rotatePreparePaintScreen(CompScreen *s,
                         int         msSinceLastPaint)
{
   ROTATE_SCREEN (s);
   CUBE_SCREEN (s);

   float oldXrot = rs->xrot + rs->baseXrot;

   if (rs->grabIndex || rs->moving)
     {
        int steps;
        float amount, chunk;

        amount = msSinceLastPaint * 0.05f *
          rs->opt[ROTATE_SCREEN_OPTION_SPEED].value.f;
        steps = amount /
          (0.5f * rs->opt[ROTATE_SCREEN_OPTION_TIMESTEP].value.f);
        if (!steps) steps = 1;
        chunk = amount / (float)steps;

        while (steps--)
          {
             rs->xrot += rs->xVelocity * chunk;
             rs->yrot += rs->yVelocity * chunk;

             if (rs->xrot > 360.0f / s->hsize)
               {
                  rs->baseXrot += 360.0f / s->hsize;
                  rs->xrot -= 360.0f / s->hsize;
               }
             else if (rs->xrot < 0.0f)
               {
                  rs->baseXrot -= 360.0f / s->hsize;
                  rs->xrot += 360.0f / s->hsize;
               }

             if (cs->invert == -1)
               {
                  if (rs->yrot > 45.0f)
                    {
                       rs->yVelocity = 0.0f;
                       rs->yrot = 45.0f;
                    }
                  else if (rs->yrot < -45.0f)
                    {
                       rs->yVelocity = 0.0f;
                       rs->yrot = -45.0f;
                    }
               }
             else
               {
                  if (rs->yrot > 100.0f)
                    {
                       rs->yVelocity = 0.0f;
                       rs->yrot = 100.0f;
                    }
                  else if (rs->yrot < -100.0f)
                    {
                       rs->yVelocity = 0.0f;
                       rs->yrot = -100.0f;
                    }
               }

             if (rs->grabbed)
               {
                  rs->xVelocity /= 1.25f;
                  rs->yVelocity /= 1.25f;

                  if (fabs (rs->xVelocity) < 0.01f)
                    rs->xVelocity = 0.0f;
                  if (fabs (rs->yVelocity) < 0.01f)
                    rs->yVelocity = 0.0f;
               }
             else if (adjustVelocity (rs, s->hsize))
               {
                  rs->xVelocity = 0.0f;
                  rs->yVelocity = 0.0f;

                  if (fabs (rs->yrot) < 0.1f)
                    {
                       float xrot;
                       int tx;

                       xrot = rs->baseXrot + rs->xrot;
                       if (xrot < 0.0f)
                         tx = (s->hsize * xrot / 360.0f) - 0.5f;
                       else
                         tx = (s->hsize * xrot / 360.0f) + 0.5f;

     /* flag end of rotation */
                       cs->rotationState = RotationNone;

                       moveScreenViewport (s, tx, 0, TRUE);

                       rs->xrot = 0.0f;
                       rs->yrot = 0.0f;
                       rs->baseXrot = rs->moveTo = 0.0f;
                       rs->moving = FALSE;

                       if (rs->grabIndex)
                         {
                            removeScreenGrab (s, rs->grabIndex, 0); //&rs->savedPointer);
                            rs->grabIndex = 0;
                            sendScreenViewportMessage(s);
                         }

                       if (rs->moveWindow)
                         {
                            CompWindow *w;

                            w = findWindowAtScreen (s, rs->moveWindow);
                            if (w)
                              {
                                 moveWindow (w, rs->moveWindowX - w->attrib.x, 0,
                                             TRUE, TRUE);
                                 syncWindowPosition (w);
                              }
                         }
                       else
                         {
                            int i;

                            for (i = 0; i < s->maxGrab; i++)
                              if (s->grabs[i].active &&
                                  strcmp ("switcher", s->grabs[i].name) == 0)
                                break;

     /* only focus default window if switcher isn't active */
                            //if (i == s->maxGrab)
     //   focusDefaultWindow (s->display);
                         }

                       rs->moveWindow = 0;
                    }
                  break;
               }
          }

        if (rs->moveWindow)
          {
             CompWindow *w;

             w = findWindowAtScreen (s, rs->moveWindow);
             if (w)
               {
                  float xrot = (s->hsize * (rs->baseXrot + rs->xrot)) / 360.0f;

                  moveWindowToViewportPosition (w,
                                                rs->moveWindowX - xrot * s->width,
                                                w->attrib.y,
                                                FALSE);
               }
          }
     }

   if (rs->moving && cs->invert == 1 && !cs->unfolded)
     {
        if (fabs (rs->xrot + rs->baseXrot + rs->moveTo) <=
            (360.0 / (s->hsize * 2.0)))
          {
             rs->zoomTranslate = rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f *
               fabs(rs->xrot + rs->baseXrot + rs->moveTo) /
               (360.0 / (s->hsize * 2.0));
          }
        else if (fabs (rs->xrot + rs->baseXrot) <= (360.0 / (s->hsize * 2.0)))
          {
             rs->zoomTranslate = rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f *
               fabs(rs->xrot + rs->baseXrot) /
               (360.0 / (s->hsize * 2.0));
          }
        else
          {
             rs->zoomTranslate += fabs (rs->xrot + rs->baseXrot - oldXrot) /
               (360.0 / (s->hsize * 2.0)) *
               rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f;
             rs->zoomTranslate =
               MIN (rs->zoomTranslate,
                    rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f);
          }
     }
   else if ((rs->zoomTranslate != 0.0f || rs->grabbed) && cs->invert == 1 &&
            !cs->unfolded)
     {
        int steps;
        float amount, chunk;

        amount = msSinceLastPaint * 0.05f *
          rs->opt[ROTATE_SCREEN_OPTION_SPEED].value.f;
        steps = amount /
          (0.5f * rs->opt[ROTATE_SCREEN_OPTION_TIMESTEP].value.f);
        if (!steps)
          steps = 1;

        chunk = amount / (float)steps;

        while (steps--)
          {
             float dt, adjust, tamount;

             if (rs->grabbed)
               dt = rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f -
                 rs->zoomTranslate;
             else
               dt = 0.0f - rs->zoomTranslate;

             adjust = dt * 0.15f;
             tamount = fabs (dt) * 1.5f;
             if (tamount < 0.2f)
               tamount = 0.2f;
             else if (tamount > 2.0f)
               tamount = 2.0f;

             rs->zoomVelocity = (tamount * rs->zoomVelocity + adjust) /
               (tamount + 1.0f);

             if (fabs (dt) < 0.1f && fabs (rs->zoomVelocity) < 0.0005f)
               {
                  if (rs->grabbed)
                    rs->zoomTranslate =
                      rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f;
                  else
                    rs->zoomTranslate = 0.0f;

                  break;
               }
             rs->zoomTranslate += rs->zoomVelocity * chunk;
          }
     }

   UNWRAP (rs, s, preparePaintScreen);
   (*s->preparePaintScreen)(s, msSinceLastPaint);
   WRAP (rs, s, preparePaintScreen, rotatePreparePaintScreen);
}

static void
rotateDonePaintScreen(CompScreen *s)
{
   ROTATE_SCREEN (s);

   if (rs->grabIndex || rs->moving)
     {
        if ((!rs->grabbed && !rs->snapTop) || rs->xVelocity || rs->yVelocity)
          damageScreen (s);
     }

   if (rs->zoomTranslate > 0.0f &&
       rs->zoomTranslate < rs->opt[ROTATE_SCREEN_OPTION_ZOOM].value.f)
     damageScreen (s);

   UNWRAP (rs, s, donePaintScreen);
   (*s->donePaintScreen)(s);
   WRAP (rs, s, donePaintScreen, rotateDonePaintScreen);
}

static void
rotateGetRotation(CompScreen *s,
                  float      *x,
                  float      *v)
{
   CUBE_SCREEN (s);
   ROTATE_SCREEN (s);

   UNWRAP (rs, cs, getRotation);
   (*cs->getRotation)(s, x, v);
   WRAP (rs, cs, getRotation, rotateGetRotation);

   *x += rs->baseXrot + rs->xrot;
   *v += rs->yrot;
}

static void
rotatePaintScreen(CompScreen  *s,
                  CompOutput  *outputs,
                  int          numOutputs,
                  unsigned int mask)
{
   ROTATE_SCREEN (s);
   CUBE_SCREEN (s);

   UNWRAP (rs, s, paintScreen);
   if ((rs->grabIndex || rs->moving || rs->zoomTranslate || cs->grabIndex) &&
       cs->moMode == CUBE_MOMODE_ONE && s->nOutputDev)
     (*s->paintScreen)(s, &s->fullscreenOutput, 1, mask);
   else
     (*s->paintScreen)(s, outputs, numOutputs, mask);
   WRAP (rs, s, paintScreen, rotatePaintScreen);
}

static Bool
rotatePaintOutput(CompScreen              *s,
                  const ScreenPaintAttrib *sAttrib,
                  const CompTransform     *transform,
                  Region                   region,
                  CompOutput              *output,
                  unsigned int             mask)
{
   Bool status;
   ScreenPaintAttrib sA = *sAttrib;

   ROTATE_SCREEN (s);

   if (rs->grabIndex || rs->moving || rs->zoomTranslate != 0.0f)
     {
        sA.zCamera -= rs->zoomTranslate;
        mask &= ~PAINT_SCREEN_REGION_MASK;
        mask |= PAINT_SCREEN_TRANSFORMED_MASK;
     }

   UNWRAP (rs, s, paintOutput);
   status = (*s->paintOutput)(s, &sA, transform, region, output, mask);
   WRAP (rs, s, paintOutput, rotatePaintOutput);

   return status;
}

static Bool
rotateInitiate2(CompScreen *s, int x, int y, int manual)
{
   ROTATE_SCREEN (s);
   CUBE_SCREEN (s);

   if (s->hsize < 2)
     return FALSE;

   if (rs->rotateHandle && rs->grabWindow)
     {
        if (otherScreenGrabExist (s, "rotate", "move", 0))
          return FALSE;
     }
   else
     {
        if (otherScreenGrabExist (s, "rotate", "switcher", "cube", 0))
          return FALSE;
     }

   rs->moving = FALSE;
   rs->slow = FALSE;

   /* Set the rotation state for cube - if action is non-NULL,
      we set it to manual (as we were called from the 'Initiate
      Rotation' binding. Otherwise, we set it to Change. */
   if (manual)
     cs->rotationState = RotationManual;
   else
     cs->rotationState = RotationChange;

   if (!rs->grabIndex)
     {
        rs->grabIndex = pushScreenGrab (s, 0 /* s->invisibleCursor */, "rotate");

        if (rs->grabIndex)
          {
             rs->savedPointer.x = x;
             rs->savedPointer.y = y;
          }
     }

   if (rs->grabIndex)
     {
        rs->moveTo = 0.0f;

        rs->grabbed = TRUE;
        rs->snapTop = rs->opt[ROTATE_SCREEN_OPTION_SNAP_TOP].value.b;
     }

   return TRUE;
}

static Bool
rotateInitiate(CompDisplay    *d,
               CompAction     *action,
               CompActionState state,
               CompOption     *option,
               int             nOption)
{
   CompScreen *s;
   Window xid;

   xid = getIntOptionNamed (option, nOption, "root", 0);

   s = findScreenAtDisplay (d, xid);
   if (s)
     {
        int x, y;

        x = getIntOptionNamed (option, nOption, "x", 0);
        y = getIntOptionNamed (option, nOption, "y", 0);
        rotateInitiate2(s, x, y, 0);
     }

   return TRUE;
}

static Bool
rotateTerminate2(CompScreen *s)
{
   ROTATE_SCREEN (s);

   if (rs->grabIndex)
     {
        //if (!xid)
          rs->snapTop = FALSE;

          rs->grabbed = FALSE;
          damageScreen (s);
     }
   return TRUE;
}

static Bool
rotateTerminate(CompDisplay    *d,
                CompAction     *action,
                CompActionState state,
                CompOption     *option,
                int             nOption)
{
   CompScreen *s;
   Window xid;

   xid = getIntOptionNamed (option, nOption, "root", 0);

   for (s = d->screens; s; s = s->next)
     {
        ROTATE_SCREEN (s);

        if (xid && s->root != xid)
          continue;

        if (rs->grabIndex)
          {
             if (!xid)
               rs->snapTop = FALSE;

             rs->grabbed = FALSE;
             damageScreen (s);
          }

        //ecompActionTerminateNotify (s, 1);
     }

   return FALSE;
}

static Bool
rotate(CompDisplay    *d,
       CompAction     *action,
       CompActionState state,
       CompOption     *option,
       int             nOption)
{
   CompScreen *s;
   Window xid;

   xid = getIntOptionNamed (option, nOption, "root", 0);

   s = findScreenAtDisplay (d, xid);
   if (s)
     {
        int direction;

        ROTATE_SCREEN (s);

        if (s->hsize < 2)
          return FALSE;

        if (otherScreenGrabExist (s, "rotate", "move", "switcher",
                                  "group-drag", "cube", 0))
          return FALSE;

        direction = getIntOptionNamed (option, nOption, "direction", 0);
        if (!direction)
          return FALSE;

        if (rs->moveWindow)
          rotateReleaseMoveWindow (s);

        /* we allow the grab to fail here so that we can rotate on
           drag-and-drop */
        if (!rs->grabIndex)
          {
             CompOption o[3];

             o[0].type = CompOptionTypeInt;
             o[0].name = "x";
             o[0].value.i = getIntOptionNamed (option, nOption, "x", 0);

             o[1].type = CompOptionTypeInt;
             o[1].name = "y";
             o[1].value.i = getIntOptionNamed (option, nOption, "y", 0);

             o[2].type = CompOptionTypeInt;
             o[2].name = "root";
             o[2].value.i = s->root;

             rotateInitiate (d, NULL, 0, o, 3);
          }

        rs->moving = TRUE;
        rs->moveTo += (360.0f / s->hsize) * direction;
        rs->grabbed = FALSE;

        damageScreen (s);
     }

   return FALSE;
}

static Bool
rotateWithWindow(CompDisplay    *d,
                 CompAction     *action,
                 CompActionState state,
                 CompOption     *option,
                 int             nOption)
{
   CompScreen *s;
   Window xid;

   xid = getIntOptionNamed (option, nOption, "root", 0);

   s = findScreenAtDisplay (d, xid);
   if (s)
     {
        int direction;

        ROTATE_SCREEN (s);

        if (s->hsize < 2)
          return FALSE;

        direction = getIntOptionNamed (option, nOption, "direction", 0);
        if (!direction)
          return FALSE;

        xid = getIntOptionNamed (option, nOption, "window", 0);

        if (rs->moveWindow != xid)
          {
             CompWindow *w;

             rotateReleaseMoveWindow (s);

             if (!rs->grabIndex && !rs->moving)
               {
                  w = findWindowAtScreen (s, xid);
                  if (w)
                    {
                       if (!(w->state & CompWindowStateStickyMask))
                         {
                            rs->moveWindow = w->id;
                            rs->moveWindowX = w->attrib.x;
                         }
                    }
               }
          }

        CompOption o[4];

        o[0].type = CompOptionTypeInt;
        o[0].name = "x";
        o[0].value.i = getIntOptionNamed (option, nOption, "x", 0);

        o[1].type = CompOptionTypeInt;
        o[1].name = "y";
        o[1].value.i = getIntOptionNamed (option, nOption, "y", 0);

        o[2].type = CompOptionTypeInt;
        o[2].name = "root";
        o[2].value.i = s->root;

        o[3].type = CompOptionTypeInt;
        o[3].name = "direction";
        o[3].value.i = getIntOptionNamed (option, nOption, "direction", 0);

        rotate (d, NULL, 0, o, 4);
     }

   return FALSE;
}

static void
rotateHandleEvent(CompDisplay *d,
                  XEvent      *event)
{
   CompScreen *s;

   ROTATE_DISPLAY (d);

   switch (event->type) {
      case MotionNotify:
        s = findScreenAtDisplay (d, event->xmotion.root);
        if (s)
          {
             ROTATE_SCREEN (s);
             CUBE_SCREEN (s);

             if (rs->grabIndex)
               {
                  if (rs->grabbed)
                    {
                       GLfloat pointerDx, pointerDy;

                       pointerDx = pointerX - lastPointerX;
                       pointerDy = pointerY - lastPointerY;

                       if (event->xmotion.x_root < 50 ||
                           event->xmotion.y_root < 50 ||
                           event->xmotion.x_root > s->width - 50 ||
                           event->xmotion.y_root > s->height - 50)
                         {
                            warpPointer (s,
                                         (s->width / 2) - pointerX,
                                         (s->height / 2) - pointerY);
                         }

                       if (rs->opt[ROTATE_SCREEN_OPTION_POINTER_INVERT_Y].value.b)
                         pointerDy = -pointerDy;

                       rs->xVelocity += pointerDx * rs->pointerSensitivity *
                         cs->invert;
                       rs->yVelocity += pointerDy * rs->pointerSensitivity;

                       damageScreen (s);
                    }
                  else
                    {
                       rs->savedPointer.x += pointerX - lastPointerX;
                       rs->savedPointer.y += pointerY - lastPointerY;
                    }
               }
          }
        break;

      case ButtonPress:
        if (event->xbutton.button == Button1)
          {
             CompScreen *s;
             for (s = d->screens; s; s = s->next)
               {
                  rotateTerminate2(s);
                  ecompActionTerminateNotify (s, 1);
               }
          }
        break;

      case ClientMessage:
        if (event->xclient.message_type == d->ecoPluginAtom)
          {
             if(event->xclient.data.l[1] != ECO_PLUGIN_ROTATE) break;

             CompScreen *s;

             Window win = event->xclient.data.l[0];

             if (!(s = findScreenAtDisplay (d, win)))
               {
     // XXX ecompActionTerminateNotify (s, 1);
                    break;
               }
             unsigned int action = event->xclient.data.l[2];
             unsigned int option = event->xclient.data.l[3];
             unsigned int option2 = event->xclient.data.l[4];

             if (action == ECO_ACT_TERMINATE)
               {
                  rotateTerminate2(s);
                  ecompActionTerminateNotify (s, 1);
                  break;
               }
             else if (action == ECO_ACT_INITIATE)
               {
                  rotateInitiate2(s, pointerX, pointerY, 1);
               }
             else if (action == ECO_ACT_MOUSE_MOVE)
               {
                  pointerX = option;
                  pointerY = option2;

                  ROTATE_SCREEN (s);
                  CUBE_SCREEN (s);

                  if (rs->grabIndex)
                    {
                       if (rs->grabbed)
                         {
                            GLfloat pointerDx, pointerDy;

                            pointerDx = pointerX - lastPointerX;
                            pointerDy = pointerY - lastPointerY;

                            if (pointerX < 50 ||
                                pointerY < 50 ||
                                pointerX > s->width - 50 ||
                                pointerY > s->height - 50)
                              {
                                 warpPointer (s,
                                              (s->width / 2) - pointerX,
                                              (s->height / 2) - pointerY);
                              }

                            if (rs->opt[ROTATE_SCREEN_OPTION_POINTER_INVERT_Y].value.b)
                              pointerDy = -pointerDy;

                            rs->xVelocity += pointerDx * rs->pointerSensitivity *
                              cs->invert;
                            rs->yVelocity += pointerDy * rs->pointerSensitivity;

                            damageScreen (s);
                         }
                       else
                         {
                            rs->savedPointer.x += pointerX - lastPointerX;
                            rs->savedPointer.y += pointerY - lastPointerY;
                         }
                    }
               }
          }

        if (event->xclient.message_type == d->desktopViewportAtom)
          {
             /* ingnore messages sent to e */
              if (event->xclient.data.l[0]) break;

              event->xclient.data.l[3] = 1;
              s = findScreenAtDisplay (d, event->xclient.window);
              if (s)
                {
                   int dx, dy, toX, toY, cx;

                   ROTATE_SCREEN (s);

                   if (otherScreenGrabExist (s, "rotate", "switcher", "cube", "move", 0))
                     break;

                   if (rs->moving)
                     {
                        cx = rs->destX;
                     }
                   else
                     {
                        cx = s->x;
     /* reset movement */
                        rs->moveTo = 0.0f;
                     }

                   toX = event->xclient.data.l[1] / s->width;
                   dx = toX - cx; //s->x;

                   rs->destX = toX;

                   toY = event->xclient.data.l[2] / s->height;
                   dy = toY - s->y;

                   if (dy)
                     {
                        rs->moveTo = 0.0;

                        moveScreenViewport (s, -dx, -dy, TRUE);
                     }
                   else if (dx)
                     {
                        Window win;
                        int i, x, y;
                        unsigned int ui;
                        CompOption o[5];
     /* XXX what is this for ?*/
                        XQueryPointer (d->display, s->root,
                                       &win, &win, &x, &y, &i, &i, &ui);

                        if (dx > (s->hsize + 1) / 2)
                          dx -= s->hsize;
                        else if (dx < -(s->hsize + 1) / 2)
                          dx += s->hsize;

                        o[0].type = CompOptionTypeInt;
                        o[0].name = "x";
                        o[0].value.i = x;

                        o[1].type = CompOptionTypeInt;
                        o[1].name = "y";
                        o[1].value.i = y;

                        o[2].type = CompOptionTypeInt;
                        o[2].name = "root";
                        o[2].value.i = s->root;

                        o[3].type = CompOptionTypeInt;
                        o[3].name = "direction";
                        o[3].value.i = dx;

                        win = event->xclient.data.l[3];
                        unsigned int moveType = event->xclient.data.l[4];

                        if(win && moveType == 2) /* move window by */
                          {
                             o[4].type = CompOptionTypeInt;
                             o[4].name = "window";
                             o[4].value.i = win;
                             CompWindow *w;

                             w = findWindowAtScreen(s, win);
                             if(w)
                               {
                                  moveWindow(w, -dx * s->width, -dy * s->height, TRUE, TRUE);
                               }
                             rotateWithWindow (d, NULL, 0, o, 5);
                          }
                        else if(win && moveType == 1) /*move with window */
                          {
     /*CompWindow *w;

        w = findWindowAtScreen(s, win);
        if(w)
        {
         moveWindow(w, -dx * s->width, -dy * s->height, TRUE, TRUE);
         }*/
                               rotate (d, NULL, 0, o, 4);
                          }
                        else
                          rotate (d, NULL, 0, o, 4);
                     }
                }
          }

      default:
        break;
     }

   UNWRAP (rd, d, handleEvent);
   (*d->handleEvent)(d, event);
   WRAP (rd, d, handleEvent, rotateHandleEvent);
}

static void
rotateWindowGrabNotify(CompWindow  *w,
                       int          x,
                       int          y,
                       unsigned int state,
                       unsigned int mask)
{
   ROTATE_SCREEN (w->screen);

   if (!rs->grabWindow)
     {
        rs->grabMask = mask;
        rs->grabWindow = w;
     }

   UNWRAP (rs, w->screen, windowGrabNotify);
   (*w->screen->windowGrabNotify)(w, x, y, state, mask);
   WRAP (rs, w->screen, windowGrabNotify, rotateWindowGrabNotify);
}

static void
rotateWindowUngrabNotify(CompWindow *w)
{
   ROTATE_SCREEN (w->screen);

   if (w == rs->grabWindow)
     {
        rs->grabMask = 0;
        rs->grabWindow = NULL;
     }

   UNWRAP (rs, w->screen, windowUngrabNotify);
   (*w->screen->windowUngrabNotify)(w);
   WRAP (rs, w->screen, windowUngrabNotify, rotateWindowUngrabNotify);
}

static CompOption *
rotateGetDisplayOptions(CompPlugin  *plugin,
                        CompDisplay *display,
                        int         *count)
{
   ROTATE_DISPLAY (display);

   *count = NUM_OPTIONS (rd);
   return rd->opt;
}

static Bool
rotateSetDisplayOption(CompPlugin      *plugin,
                       CompDisplay     *display,
                       char            *name,
                       CompOptionValue *value)
{
   CompOption *o;

   ROTATE_DISPLAY (display);

   o = compFindOption (rd->opt, NUM_OPTIONS (rd), name, NULL);
   if (!o)
     return FALSE;

   return compSetDisplayOption (display, o, value);
}

static const CompMetadataOptionInfo rotateDisplayOptionInfo[] = {
   { "initiate", "action", 0, rotateInitiate, rotateTerminate }
};

static Bool
rotateInitDisplay(CompPlugin  *p,
                  CompDisplay *d)
{
   RotateDisplay *rd;
   CompPlugin *cube = findActivePlugin ("cube");
   CompOption *option;
   int nOption;

   if (!cube || !cube->vTable->getDisplayOptions)
     return FALSE;

   option = (*cube->vTable->getDisplayOptions)(cube, d, &nOption);

   if (getIntOptionNamed (option, nOption, "abi", 0) != CUBE_ABIVERSION)
     {
        compLogMessage (d, "rotate", CompLogLevelError,
                        "cube ABI version mismatch");
        return FALSE;
     }

   cubeDisplayPrivateIndex = getIntOptionNamed (option, nOption, "index", -1);
   if (cubeDisplayPrivateIndex < 0)
     return FALSE;

   rd = malloc (sizeof (RotateDisplay));
   if (!rd)
     return FALSE;

   if (!compInitDisplayOptionsFromMetadata (d,
                                            &rotateMetadata,
                                            rotateDisplayOptionInfo,
                                            rd->opt,
                                            ROTATE_DISPLAY_OPTION_NUM))
     {
        free (rd);
        return FALSE;
     }

   rd->screenPrivateIndex = allocateScreenPrivateIndex (d);
   if (rd->screenPrivateIndex < 0)
     {
        compFiniDisplayOptions (d, rd->opt, ROTATE_DISPLAY_OPTION_NUM);
        free (rd);
        return FALSE;
     }

   WRAP (rd, d, handleEvent, rotateHandleEvent);

   d->privates[displayPrivateIndex].ptr = rd;

   return TRUE;
}

static void
rotateFiniDisplay(CompPlugin  *p,
                  CompDisplay *d)
{
   ROTATE_DISPLAY (d);

   freeScreenPrivateIndex (d, rd->screenPrivateIndex);

   UNWRAP (rd, d, handleEvent);

   compFiniDisplayOptions (d, rd->opt, ROTATE_DISPLAY_OPTION_NUM);

   free (rd);
}

static const CompMetadataOptionInfo rotateScreenOptionInfo[] = {
   { "invert_y", "bool", 0, 0, 0 },
   { "sensitivity", "float", 0, 0, 0 },
   { "acceleration", "float", "<min>1.0</min>", 0, 0 },
   { "snap_top", "bool", 0, 0, 0 },
   { "speed", "float", "<min>0.1</min>", 0, 0 },
   { "timestep", "float", "<min>0.1</min>", 0, 0 },
   { "zoom", "float", 0, 0, 0 }
};

static Bool
rotateInitScreen(CompPlugin *p,
                 CompScreen *s)
{
   RotateScreen *rs;

   ROTATE_DISPLAY (s->display);
   CUBE_SCREEN (s);

   rs = malloc (sizeof (RotateScreen));
   if (!rs)
     return FALSE;

   if (!compInitScreenOptionsFromMetadata (s,
                                           &rotateMetadata,
                                           rotateScreenOptionInfo,
                                           rs->opt,
                                           ROTATE_SCREEN_OPTION_NUM))
     {
        free (rs);
        return FALSE;
     }

   rs->grabIndex = 0;

   rs->xrot = 0.0f;
   rs->xVelocity = 0.0f;
   rs->yrot = 0.0f;
   rs->yVelocity = 0.0f;

   rs->baseXrot = 0.0f;

   rs->moving = FALSE;
   rs->moveTo = 0.0f;

   rs->moveWindow = 0;

   rs->savedPointer.x = 0;
   rs->savedPointer.y = 0;

   rs->grabbed = FALSE;
   rs->snapTop = FALSE;

   rs->slow = FALSE;
   rs->grabMask = FALSE;
   rs->grabWindow = NULL;

   rs->pointerSensitivity =
     rs->opt[ROTATE_SCREEN_OPTION_POINTER_SENSITIVITY].value.f *
     ROTATE_POINTER_SENSITIVITY_FACTOR;

   rs->rotateHandle = 0;

   rs->zoomTranslate = 0.0;
   rs->zoomVelocity = 0.0;

   WRAP (rs, s, preparePaintScreen, rotatePreparePaintScreen);
   WRAP (rs, s, donePaintScreen, rotateDonePaintScreen);
   WRAP (rs, s, paintScreen, rotatePaintScreen);
   WRAP (rs, s, paintOutput, rotatePaintOutput);
   WRAP (rs, s, windowGrabNotify, rotateWindowGrabNotify);
   WRAP (rs, s, windowUngrabNotify, rotateWindowUngrabNotify);

   WRAP (rs, cs, getRotation, rotateGetRotation);

   s->privates[rd->screenPrivateIndex].ptr = rs;

   return TRUE;
}

static void
rotateFiniScreen(CompPlugin *p,
                 CompScreen *s)
{
   CUBE_SCREEN (s);
   ROTATE_SCREEN (s);

   UNWRAP (rs, cs, getRotation);

   UNWRAP (rs, s, preparePaintScreen);
   UNWRAP (rs, s, donePaintScreen);
   UNWRAP (rs, s, paintScreen);
   UNWRAP (rs, s, paintOutput);
   UNWRAP (rs, s, windowGrabNotify);
   UNWRAP (rs, s, windowUngrabNotify);

   compFiniScreenOptions (s, rs->opt, ROTATE_SCREEN_OPTION_NUM);

   free (rs);
}

static Bool
rotateInit(CompPlugin *p)
{
   if (!compInitPluginMetadataFromInfo (&rotateMetadata,
                                        p->vTable->name,
                                        rotateDisplayOptionInfo,
                                        ROTATE_DISPLAY_OPTION_NUM,
                                        rotateScreenOptionInfo,
                                        ROTATE_SCREEN_OPTION_NUM))
     return FALSE;

   displayPrivateIndex = allocateDisplayPrivateIndex ();
   if (displayPrivateIndex < 0)
     {
        compFiniMetadata (&rotateMetadata);
        return FALSE;
     }

   compAddMetadataFromFile (&rotateMetadata, p->vTable->name);

   return TRUE;
}

static void
rotateFini(CompPlugin *p)
{
   freeDisplayPrivateIndex (displayPrivateIndex);
   compFiniMetadata (&rotateMetadata);
}

static int
rotateGetVersion(CompPlugin *plugin,
                 int         version)
{
   return ABIVERSION;
}

static CompMetadata *
rotateGetMetadata(CompPlugin *plugin)
{
   return &rotateMetadata;
}

CompPluginVTable rotateVTable = {
   "rotate",
   rotateGetVersion,
   rotateGetMetadata,
   rotateInit,
   rotateFini,
   rotateInitDisplay,
   rotateFiniDisplay,
   rotateInitScreen,
   rotateFiniScreen,
   0,  /* InitWindow */
   0,  /* FiniWindow */
   rotateGetDisplayOptions,
   rotateSetDisplayOption,
   rotateGetScreenOptions,
   rotateSetScreenOption
};

CompPluginVTable *
getCompPluginInfo(void)
{
   return &rotateVTable;
}

