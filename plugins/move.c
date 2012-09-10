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

#include <X11/cursorfont.h>

#include <ecomp.h>

static CompMetadata moveMetadata;

/* #define NUM_KEYS (sizeof (mKeys) / sizeof (mKeys[0]))
 *
 * #define KEY_MOVE_INC 24
 *
 * #define SNAP_BACK 20
 * #define SNAP_OFF  100 */

static int displayPrivateIndex;

#define MOVE_DISPLAY_OPTION_INITIATE          0
#define MOVE_DISPLAY_OPTION_OPACITY           1
#define MOVE_DISPLAY_OPTION_CONSTRAIN_Y       2
#define MOVE_DISPLAY_OPTION_SNAPOFF_MAXIMIZED 3
#define MOVE_DISPLAY_OPTION_LAZY_POSITIONING  4
#define MOVE_DISPLAY_OPTION_NUM               5

typedef struct _MoveDisplay
{
   int             screenPrivateIndex;
   HandleEventProc handleEvent;

   CompOption      opt[MOVE_DISPLAY_OPTION_NUM];

   CompWindow     *w;

   GLushort        moveOpacity;
} MoveDisplay;

typedef struct _MoveScreen
{
   PaintWindowProc        paintWindow;
   PreparePaintScreenProc preparePaintScreen;
   int                    grabIndex;

   unsigned int           origState;
   double                 progress;
   int                    active;
} MoveScreen;

#define GET_MOVE_DISPLAY(d) \
  ((MoveDisplay *)(d)->privates[displayPrivateIndex].ptr)

#define MOVE_DISPLAY(d) \
  MoveDisplay * md = GET_MOVE_DISPLAY (d)

#define GET_MOVE_SCREEN(s, md) \
  ((MoveScreen *)(s)->privates[(md)->screenPrivateIndex].ptr)

#define MOVE_SCREEN(s) \
  MoveScreen * ms = GET_MOVE_SCREEN (s, GET_MOVE_DISPLAY (s->display))

#define NUM_OPTIONS(s) (sizeof ((s)->opt) / sizeof (CompOption))

static Bool
moveInitiate(CompDisplay    *d,
             CompAction     *action,
             CompActionState state,
             CompOption     *option,
             int             nOption)
{
   CompWindow *w;
   Window xid;
   int i, x, y;
   unsigned int ui;
   unsigned int mods = 0;

   MOVE_DISPLAY (d);
   xid = getIntOptionNamed (option, nOption, "window", 0);

   w = findWindowAtDisplay (d, xid);
   if (!w) return FALSE;

   if (otherScreenGrabExist (w->screen, "move", 0))
     return FALSE;

   MOVE_SCREEN (w->screen);

   XQueryPointer (d->display, w->screen->root,
                  &xid, &xid, &x, &y, &i, &i, &ui);

   if (md->w)
     return FALSE;

   lastPointerX = x;
   lastPointerY = y;

   ms->origState = w->state;

   if (!ms->grabIndex)
     ms->grabIndex = pushScreenGrab (w->screen, 0, "move");

   if (ms->grabIndex)
     {
        md->w = w;

        (w->screen->windowGrabNotify)(w, x, y, mods,
                                      CompWindowGrabMoveMask |
                                      CompWindowGrabButtonMask);

        if (md->moveOpacity != OPAQUE)
          addWindowDamage (w);
     }

   return FALSE;
}

static Bool
moveTerminate(CompDisplay    *d,
              CompAction     *action,
              CompActionState state,
              CompOption     *option,
              int             nOption)
{
   MOVE_DISPLAY (d);

   if (md->w)
     {
        MOVE_SCREEN (md->w->screen);

        (md->w->screen->windowUngrabNotify)(md->w);

        if (ms->grabIndex)
          {
             removeScreenGrab (md->w->screen, ms->grabIndex, NULL);
             ms->grabIndex = 0;
          }

        if (md->moveOpacity != OPAQUE)
          /* let the window fade in */
          addWindowDamage (md->w);
        else
          md->w = 0;
     }

   return FALSE;
}

static void
moveHandleEvent(CompDisplay *d,
                XEvent      *event)
{
   CompScreen *s;

   MOVE_DISPLAY (d);

   switch (event->type)
     {
      case ClientMessage:
        if (event->xclient.message_type == d->eManagedAtom)
          {
             CompWindow *w;

             Window win = event->xclient.window;
             unsigned int type = event->xclient.data.l[0];
             if (type != 4) break;

             w = findWindowAtDisplay (d, win);
             if (w)
               {
                  unsigned int state = event->xclient.data.l[2];
                  s = w->screen;

                  if(state)
                    {
                       CompOption o[1];
                       CompAction *action = NULL;

                       o[0].type = CompOptionTypeInt;
                       o[0].name = "window";
                       o[0].value.i = w->id;

                       moveInitiate (d, action, CompActionStateInitButton, o, 1);
                       MOVE_SCREEN(s);

                       ms->active = 1;
                    }
                  else
                    {
                       moveTerminate (d, NULL, 0, NULL, 0);
                    }
               }
          }

        break;

      case DestroyNotify:
        if (md->w && md->w->id == event->xdestroywindow.window)
          moveTerminate (d, NULL, 0, NULL, 0);
        break;

      case UnmapNotify:
        if (md->w && md->w->id == event->xunmap.window)
          moveTerminate (d, NULL, 0, NULL, 0);

      default:
        break;
     }

   UNWRAP (md, d, handleEvent);
   (*d->handleEvent)(d, event);
   WRAP (md, d, handleEvent, moveHandleEvent);
}

static void
movePreparePaintScreen(CompScreen *s,
                       int         _ms)
{
   MOVE_SCREEN(s);

   if (ms->grabIndex || ms->active)
     {
        float val = (((float)_ms / 1000.0) / 0.2);

        if (ms->grabIndex)
          ms->progress = MIN (1.0, ms->progress + val);
        else
          ms->progress = MAX (0.0, ms->progress - val);
     }

   UNWRAP (ms, s, preparePaintScreen);
   (*s->preparePaintScreen)(s, _ms);
   WRAP (ms, s, preparePaintScreen, movePreparePaintScreen);
}

static Bool
movePaintWindow(CompWindow *w, const WindowPaintAttrib *attrib,
                const CompTransform *transform, Region region, unsigned int mask)
{
   WindowPaintAttrib sAttrib;
   CompScreen *s = w->screen;
   Bool status;

   MOVE_SCREEN (s);

   if (ms->active)
     {
        MOVE_DISPLAY (s->display);

        addWindowDamage(w);

        if (md->w == w && md->moveOpacity != OPAQUE)
          {
             sAttrib = *attrib;
             attrib = &sAttrib;
             float opacity_percent, one_percent;

             one_percent = (float)1 / (float)100 * sAttrib.opacity;
             opacity_percent = 100 - (((float)md->moveOpacity) / ((float)sAttrib.opacity) * 100);
             sAttrib.opacity -= (sAttrib.opacity + (opacity_percent * one_percent * ms->progress - sAttrib.opacity));
          }
        if (ms->progress == 0.0)
          {
             ms->active = 0;
             md->w = 0;
          }
     }

   UNWRAP (ms, s, paintWindow);
   status = (*s->paintWindow)(w, attrib, transform, region, mask);
   WRAP (ms, s, paintWindow, movePaintWindow);

   return status;
}

static CompOption *
moveGetDisplayOptions(CompPlugin *plugin, CompDisplay *display, int *count)
{
   MOVE_DISPLAY (display);

   *count = NUM_OPTIONS (md);
   return md->opt;
}

static Bool
moveSetDisplayOption(CompPlugin      *plugin,
                     CompDisplay     *display,
                     char            *name,
                     CompOptionValue *value)
{
   CompOption *o;
   int index;

   MOVE_DISPLAY (display);

   o = compFindOption (md->opt, NUM_OPTIONS (md), name, &index);
   if (!o)
     return FALSE;

   switch (index) {
      case MOVE_DISPLAY_OPTION_OPACITY:
        if (compSetIntOption (o, value))
          {
             md->moveOpacity = (o->value.i * OPAQUE) / 100;
             return TRUE;
          }
        break;

      default:
        return compSetDisplayOption (display, o, value);
     }

   return FALSE;
}

static const CompMetadataOptionInfo moveDisplayOptionInfo[] = {
   { "initiate", "action", 0, moveInitiate, moveTerminate },
   { "opacity", "int", "<min>0</min><max>100</max>", 0, 0 },
   { "constrain_y", "bool", 0, 0, 0 },
   { "snapoff_maximized", "bool", 0, 0, 0 },
   { "lazy_positioning", "bool", 0, 0, 0 }
};

static Bool
moveInitDisplay(CompPlugin  *p,
                CompDisplay *d)
{
   MoveDisplay *md;

   md = malloc (sizeof (MoveDisplay));
   if (!md) return FALSE;

   md->w = 0;

   if (!compInitDisplayOptionsFromMetadata
         (d, &moveMetadata, moveDisplayOptionInfo, md->opt, MOVE_DISPLAY_OPTION_NUM))
     {
        free (md);
        return FALSE;
     }

   md->screenPrivateIndex = allocateScreenPrivateIndex (d);
   if (md->screenPrivateIndex < 0)
     {
        compFiniDisplayOptions (d, md->opt, MOVE_DISPLAY_OPTION_NUM);
        free (md);
        return FALSE;
     }

   md->moveOpacity = (md->opt[MOVE_DISPLAY_OPTION_OPACITY].value.i * OPAQUE) / 100;

   WRAP (md, d, handleEvent, moveHandleEvent);

   d->privates[displayPrivateIndex].ptr = md;

   return TRUE;
}

static void
moveFiniDisplay(CompPlugin  *p,
                CompDisplay *d)
{
   MOVE_DISPLAY (d);

   freeScreenPrivateIndex (d, md->screenPrivateIndex);

   UNWRAP (md, d, handleEvent);

   compFiniDisplayOptions (d, md->opt, MOVE_DISPLAY_OPTION_NUM);

   free (md);
}

static Bool
moveInitScreen(CompPlugin *p,
               CompScreen *s)
{
   MoveScreen *ms;

   MOVE_DISPLAY (s->display);

   ms = malloc (sizeof (MoveScreen));
   if (!ms)
     return FALSE;

   ms->grabIndex = 0;
   ms->active = 0;
   ms->progress = 0.0;

   WRAP (ms, s, paintWindow, movePaintWindow);
   WRAP (ms, s, preparePaintScreen, movePreparePaintScreen);

   s->privates[md->screenPrivateIndex].ptr = ms;

   return TRUE;
}

static void
moveFiniScreen(CompPlugin *p,
               CompScreen *s)
{
   MOVE_SCREEN (s);

   UNWRAP (ms, s, paintWindow);
   UNWRAP (ms, s, preparePaintScreen);

   free (ms);
}

static Bool
moveInit(CompPlugin *p)
{
   if (!compInitPluginMetadataFromInfo
         (&moveMetadata, p->vTable->name, moveDisplayOptionInfo,
         MOVE_DISPLAY_OPTION_NUM, 0, 0))
     return FALSE;

   displayPrivateIndex = allocateDisplayPrivateIndex ();
   if (displayPrivateIndex < 0)
     {
        compFiniMetadata (&moveMetadata);
        return FALSE;
     }

   compAddMetadataFromFile (&moveMetadata, p->vTable->name);

   return TRUE;
}

static void
moveFini(CompPlugin *p)
{
   freeDisplayPrivateIndex (displayPrivateIndex);
   compFiniMetadata (&moveMetadata);
}

static int
moveGetVersion(CompPlugin *plugin,
               int         version)
{
   return ABIVERSION;
}

static CompMetadata *
moveGetMetadata(CompPlugin *plugin)
{
   return &moveMetadata;
}

CompPluginVTable moveVTable = {
   "move",
   moveGetVersion,
   moveGetMetadata,
   moveInit,
   moveFini,
   moveInitDisplay,
   moveFiniDisplay,
   moveInitScreen,
   moveFiniScreen,
   0,  /* InitWindow */
   0,  /* FiniWindow */
   moveGetDisplayOptions,
   moveSetDisplayOption,
   0,  /* GetScreenOptions */
   0   /* SetScreenOption */
};

CompPluginVTable *
getCompPluginInfo(void)
{
   return &moveVTable;
}

