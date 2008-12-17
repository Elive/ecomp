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

#define A(x) do { printf(__FILE__ ":%d:\t", __LINE__); printf x; fflush(stdout); } while(0)   
#define D(x)
#define B(x)
#define C(x)
#define E(x)

  
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xcomposite.h>

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>

#include <ecomp.h>

#define MwmHintsFunctions   (1L << 0)
#define MwmHintsDecorations (1L << 1)

#define PropMotifWmHintElements 3

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
} MwmHints;

static int
reallocWindowPrivates (int  size,
		       void *closure)
{
  CompScreen *s = (CompScreen *) closure;
  CompWindow *w;
  void       *privates;

  for (w = s->windows; w; w = w->next)
    {
      privates = realloc (w->privates, size * sizeof (CompPrivate));
      if (!privates)
	return FALSE;

      w->privates = (CompPrivate *) privates;
    }

  return TRUE;
}

int
allocateWindowPrivateIndex (CompScreen *screen)
{
  return allocatePrivateIndex (&screen->windowPrivateLen,
			       &screen->windowPrivateIndices,
			       reallocWindowPrivates,
			       (void *) screen);
}

void
freeWindowPrivateIndex (CompScreen *screen,
			int	   index)
{
  freePrivateIndex (screen->windowPrivateLen,
		    screen->windowPrivateIndices,
		    index);
}

void
updateWindowClassHints (CompWindow *w)
{
  XClassHint classHint;
  int	       status;

  if (w->resName)
    {
      free (w->resName);
      w->resName = NULL;
    }

  if (w->resClass)
    {
      free (w->resClass);
      w->resClass = NULL;
    }

  if(w->clientId)
    status = XGetClassHint (w->screen->display->display, w->clientId, &classHint);
  else
    status = XGetClassHint (w->screen->display->display, w->id, &classHint);

  if (status)
    {
      if (classHint.res_name)
	{
	  w->resName = strdup (classHint.res_name);
	  XFree (classHint.res_name);
	}

      if (classHint.res_class)
	{
	  w->resClass = strdup (classHint.res_class);
	  XFree (classHint.res_class);
  	}

      if (((w->wmType == CompWindowTypeNormalMask) ||
	   (w->wmType == CompWindowTypeUnknownMask) ||
	   (w->wmType == CompWindowTypeUtilMask)) && 
	  (!w->clientId))
	{
	  if ((!strcasecmp (w->resName, "gecko")) ||
	      (!strcasecmp (w->resName, "popup")) ||
	      (!strcasecmp (w->resName, "VCLSalFrame")))
	    {
	      w->wmType = CompWindowTypeDropdownMenuMask;
	    }
	}
      else if ((!strcmp (w->resName, "sun-awt-X11-XMenuWindow")) ||
	       (!strcmp (w->resName, "sun-awt-X11-XWindowPeer")))
	{
	  w->wmType = CompWindowTypeDropdownMenuMask;
	}
      else if (!strcmp (w->resName, "sun-awt-X11-XDialogPeer"))
	{
	  w->wmType = CompWindowTypeDialogMask;
	}
      else if (!strcmp (w->resName, "sun-awt-X11-XFramePeer"))
	{
	  w->wmType = CompWindowTypeNormalMask;
	}
    }
}


int
getWmState (CompDisplay *display,
	    Window      id)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;
    unsigned long state = NormalState;

    result = XGetWindowProperty (display->display, id,
				 display->wmStateAtom, 0L, 2L, FALSE,
				 display->wmStateAtom, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	memcpy (&state, data, sizeof (unsigned long));
	XFree ((void *) data);
    }

    return state;
}


unsigned int
windowStateMask (CompDisplay *display,
		 Atom	     state)
{
    if (state == display->winStateModalAtom)
	return CompWindowStateModalMask;
    else if (state == display->winStateStickyAtom)
	return CompWindowStateStickyMask;
    else if (state == display->winStateMaximizedVertAtom)
	return CompWindowStateMaximizedVertMask;
    else if (state == display->winStateMaximizedHorzAtom)
	return CompWindowStateMaximizedHorzMask;
    else if (state == display->winStateShadedAtom)
	return CompWindowStateShadedMask;
    else if (state == display->winStateSkipTaskbarAtom)
	return CompWindowStateSkipTaskbarMask;
    else if (state == display->winStateSkipPagerAtom)
	return CompWindowStateSkipPagerMask;
    else if (state == display->winStateHiddenAtom)
	return CompWindowStateHiddenMask;
    else if (state == display->winStateFullscreenAtom)
	return CompWindowStateFullscreenMask;
    else if (state == display->winStateAboveAtom)
	return CompWindowStateAboveMask;
    else if (state == display->winStateBelowAtom)
	return CompWindowStateBelowMask;
    else if (state == display->winStateDemandsAttentionAtom)
	return CompWindowStateDemandsAttentionMask;
    else if (state == display->winStateDisplayModalAtom)
	return CompWindowStateDisplayModalMask;

    return 0;
}

unsigned int
windowStateFromString (const char *str)
{
    if (strcasecmp (str, "modal") == 0)
	return CompWindowStateModalMask;
    else if (strcasecmp (str, "sticky") == 0)
	return CompWindowStateStickyMask;
    else if (strcasecmp (str, "maxvert") == 0)
	return CompWindowStateMaximizedVertMask;
    else if (strcasecmp (str, "maxhorz") == 0)
	return CompWindowStateMaximizedHorzMask;
    else if (strcasecmp (str, "shaded") == 0)
	return CompWindowStateShadedMask;
    else if (strcasecmp (str, "skiptaskbar") == 0)
	return CompWindowStateSkipTaskbarMask;
    else if (strcasecmp (str, "skippager") == 0)
	return CompWindowStateSkipPagerMask;
    else if (strcasecmp (str, "hidden") == 0)
	return CompWindowStateHiddenMask;
    else if (strcasecmp (str, "fullscreen") == 0)
	return CompWindowStateFullscreenMask;
    else if (strcasecmp (str, "above") == 0)
	return CompWindowStateAboveMask;
    else if (strcasecmp (str, "below") == 0)
	return CompWindowStateBelowMask;
    else if (strcasecmp (str, "demandsattention") == 0)
	return CompWindowStateDemandsAttentionMask;

    return 0;
}

unsigned int
getWindowState (CompDisplay *display,
		Window      id)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;
    unsigned int  state = 0;

    result = XGetWindowProperty (display->display, id, display->winStateAtom,
				 0L, 1024L, FALSE, XA_ATOM, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	Atom *a = (Atom *) data;

	while (n--)
	    state |= windowStateMask (display, *a++);

	XFree ((void *) data);
    }

    return state;
}

void
setWindowState (CompDisplay  *display,
		unsigned int state,
		Window       id)
{
    Atom data[32];
    int	 i = 0;

    if (state & CompWindowStateModalMask)
	data[i++] = display->winStateModalAtom;
    if (state & CompWindowStateStickyMask)
	data[i++] = display->winStateStickyAtom;
    if (state & CompWindowStateMaximizedVertMask)
	data[i++] = display->winStateMaximizedVertAtom;
    if (state & CompWindowStateMaximizedHorzMask)
	data[i++] = display->winStateMaximizedHorzAtom;
    if (state & CompWindowStateShadedMask)
	data[i++] = display->winStateShadedAtom;
    if (state & CompWindowStateSkipTaskbarMask)
	data[i++] = display->winStateSkipTaskbarAtom;
    if (state & CompWindowStateSkipPagerMask)
	data[i++] = display->winStateSkipPagerAtom;
    if (state & CompWindowStateHiddenMask)
	data[i++] = display->winStateHiddenAtom;
    if (state & CompWindowStateFullscreenMask)
	data[i++] = display->winStateFullscreenAtom;
    if (state & CompWindowStateAboveMask)
	data[i++] = display->winStateAboveAtom;
    if (state & CompWindowStateBelowMask)
	data[i++] = display->winStateBelowAtom;
    if (state & CompWindowStateDemandsAttentionMask)
	data[i++] = display->winStateDemandsAttentionAtom;
    if (state & CompWindowStateDisplayModalMask)
	data[i++] = display->winStateDisplayModalAtom;

    XChangeProperty (display->display, id, display->winStateAtom,
		     XA_ATOM, 32, PropModeReplace,
		     (unsigned char *) data, i);
}

void
changeWindowState (CompWindow   *w,
		   unsigned int newState)
{
    CompDisplay  *d = w->screen->display;
    unsigned int oldState = w->state;

    w->state = newState;

    //    setWindowState (d, w->state, w->id);

    (*w->screen->windowStateChangeNotify) (w, oldState);

    (*d->matchPropertyChanged) (d, w);
}

/* FIXME where is this used ? */
void
getAllowedActionsForWindow (CompWindow   *w,
			    unsigned int *setActions,
			    unsigned int *clearActions)
{
   *setActions   = 0;
   *clearActions = 0;
}

unsigned int
constrainWindowState (unsigned int state,
		      unsigned int actions)
{
    if (!(actions & CompWindowActionMaximizeHorzMask))
	state &= ~CompWindowStateMaximizedHorzMask;

    if (!(actions & CompWindowActionMaximizeVertMask))
	state &= ~CompWindowStateMaximizedVertMask;

    if (!(actions & CompWindowActionShadeMask))
	state &= ~CompWindowStateShadedMask;

    if (!(actions & CompWindowActionFullscreenMask))
	state &= ~CompWindowStateFullscreenMask;

    return state;
}

unsigned int
windowTypeFromString (const char *str)
{
    if (strcasecmp (str, "desktop") == 0)
	return CompWindowTypeDesktopMask;
    else if (strcasecmp (str, "dock") == 0)
	return CompWindowTypeDockMask;
    else if (strcasecmp (str, "toolbar") == 0)
	return CompWindowTypeToolbarMask;
    else if (strcasecmp (str, "menu") == 0)
	return CompWindowTypeMenuMask;
    else if (strcasecmp (str, "utility") == 0)
	return CompWindowTypeUtilMask;
    else if (strcasecmp (str, "splash") == 0)
	return CompWindowTypeSplashMask;
    else if (strcasecmp (str, "dialog") == 0)
	return CompWindowTypeDialogMask;
    else if (strcasecmp (str, "normal") == 0)
	return CompWindowTypeNormalMask;
    else if (strcasecmp (str, "dropdownmenu") == 0)
	return CompWindowTypeDropdownMenuMask;
    else if (strcasecmp (str, "popupmenu") == 0)
	return CompWindowTypePopupMenuMask;
    else if (strcasecmp (str, "tooltip") == 0)
	return CompWindowTypeTooltipMask;
    else if (strcasecmp (str, "notification") == 0)
	return CompWindowTypeNotificationMask;
    else if (strcasecmp (str, "combo") == 0)
	return CompWindowTypeComboMask;
    else if (strcasecmp (str, "dnd") == 0)
    	return CompWindowTypeDndMask;
    else if (strcasecmp (str, "modaldialog") == 0)
	return CompWindowTypeModalDialogMask;
    else if (strcasecmp (str, "fullscreen") == 0)
	return CompWindowTypeFullscreenMask;
    else if (strcasecmp (str, "unknown") == 0)
	return CompWindowTypeUnknownMask;
    else if (strcasecmp (str, "any") == 0)
	return ~0;

    return 0;
}

unsigned int
getWindowType (CompDisplay *display,
	       Window      id)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;

    result = XGetWindowProperty (display->display, id, display->winTypeAtom,
				 0L, 1L, FALSE, XA_ATOM, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	Atom a;

	memcpy (&a, data, sizeof (Atom));
	XFree ((void *) data);

	if (a == display->winTypeNormalAtom)
	    return CompWindowTypeNormalMask;
	else if (a == display->winTypeMenuAtom)
	    return CompWindowTypeMenuMask;
	else if (a == display->winTypeDesktopAtom)
	    return CompWindowTypeDesktopMask;
	else if (a == display->winTypeDockAtom)
	    return CompWindowTypeDockMask;
	else if (a == display->winTypeToolbarAtom)
	    return CompWindowTypeToolbarMask;
	else if (a == display->winTypeUtilAtom)
	    return CompWindowTypeUtilMask;
	else if (a == display->winTypeSplashAtom)
	    return CompWindowTypeSplashMask;
	else if (a == display->winTypeDialogAtom)
	    return CompWindowTypeDialogMask;
	else if (a == display->winTypeDropdownMenuAtom)
	    return CompWindowTypeDropdownMenuMask;
	else if (a == display->winTypePopupMenuAtom)
	    return CompWindowTypePopupMenuMask;
	else if (a == display->winTypeTooltipAtom)
	    return CompWindowTypeTooltipMask;
	else if (a == display->winTypeNotificationAtom)
	    return CompWindowTypeNotificationMask;
	else if (a == display->winTypeComboAtom)
	    return CompWindowTypeComboMask;
       	else if (a == display->winTypeDndAtom)
	    return CompWindowTypeDndMask;
    }

    return CompWindowTypeUnknownMask;
}

void
recalcWindowType (CompWindow *w)
{
    unsigned int type;

    type = w->wmType;
    
    if (w->clientId && (w->wmType == CompWindowTypeUnknownMask))
      {
	type = CompWindowTypeNormalMask;
      }
    else if (!w->clientId && (w->wmType == CompWindowTypeNormalMask))
      {
	type = CompWindowTypeUnknownMask;
      }

    

    if (w->state & CompWindowStateFullscreenMask)
	type = CompWindowTypeFullscreenMask;

    w->type = type;
}

void
getMwmHints (CompDisplay  *display,
	     Window	  id,
	     unsigned int *func,
	     unsigned int *decor)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;

    *func  = MwmFuncAll;
    *decor = MwmDecorAll;

    result = XGetWindowProperty (display->display, id, display->mwmHintsAtom,
				 0L, 20L, FALSE, display->mwmHintsAtom,
				 &actual, &format, &n, &left, &data);

    if (result == Success && n && data)
    {
	MwmHints *mwmHints = (MwmHints *) data;

	if (n >= PropMotifWmHintElements)
	{
	    if (mwmHints->flags & MwmHintsDecorations)
		*decor = mwmHints->decorations;

	    if (mwmHints->flags & MwmHintsFunctions)
		*func = mwmHints->functions;
	}

	XFree (data);
    }
}

unsigned int
getProtocols (CompDisplay *display,
	      Window      id)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;
    unsigned int  protocols = 0;

    result = XGetWindowProperty (display->display, id, display->wmProtocolsAtom,
				 0L, 20L, FALSE, XA_ATOM,
				 &actual, &format, &n, &left, &data);

    if (result == Success && n && data)
    {
	Atom *protocol = (Atom *) data;
	int  i;

	for (i = 0; i < n; i++)
	{
	    if (protocol[i] == display->wmDeleteWindowAtom)
		protocols |= CompWindowProtocolDeleteMask;
	    else if (protocol[i] == display->wmTakeFocusAtom)
		protocols |= CompWindowProtocolTakeFocusMask;
	    // else if (protocol[i] == display->wmPingAtom)
	    //	protocols |= CompWindowProtocolPingMask;
	    else if (protocol[i] == display->wmSyncRequestAtom)
		protocols |= CompWindowProtocolSyncRequestMask;
	}

	XFree (data);
    }

    return protocols;
}

unsigned int
getWindowProp (CompDisplay  *display,
	       Window	    id,
	       Atom	    property,
	       unsigned int defaultValue)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;

    result = XGetWindowProperty (display->display, id, property,
				 0L, 1L, FALSE, XA_CARDINAL, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	unsigned long value;

	memcpy (&value, data, sizeof (unsigned long));

	XFree (data);

	return (unsigned int) value;
    }

    return defaultValue;
}

void
setWindowProp (CompDisplay  *display,
	       Window       id,
	       Atom	    property,
	       unsigned int value)
{
    unsigned long data = value;

    XChangeProperty (display->display, id, property,
		     XA_CARDINAL, 32, PropModeReplace,
		     (unsigned char *) &data, 1);
}

Bool
readWindowProp32 (CompDisplay    *display,
		  Window	 id,
		  Atom		 property,
		  unsigned short *returnValue)
{
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;

    result = XGetWindowProperty (display->display, id, property,
				 0L, 1L, FALSE, XA_CARDINAL, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	CARD32 value;

	memcpy (&value, data, sizeof (CARD32));

	XFree (data);

	*returnValue = value >> 16;

	return TRUE;
    }

    return FALSE;
}

unsigned short
getWindowProp32 (CompDisplay	*display,
		 Window		id,
		 Atom		property,
		 unsigned short defaultValue)
{
    unsigned short result;

    if (readWindowProp32 (display, id, property, &result))
	return result;

    return defaultValue;
}

void
setWindowProp32 (CompDisplay    *display,
		 Window         id,
		 Atom		property,
		 unsigned short value)
{
    CARD32 value32;

    value32 = value << 16 | value;

    XChangeProperty (display->display, id, property,
		     XA_CARDINAL, 32, PropModeReplace,
		     (unsigned char *) &value32, 1);
}

void
updateWindowOpacity (CompWindow *w)
{
    CompScreen *s = w->screen;
    int	       opacity = w->opacity;

    if (!w->opacityPropSet && !(w->type & CompWindowTypeDesktopMask))
    {
	CompOption *matches = &s->opt[COMP_SCREEN_OPTION_OPACITY_MATCHES];
	CompOption *values = &s->opt[COMP_SCREEN_OPTION_OPACITY_VALUES];
	int	   i, min;

	min = MIN (matches->value.list.nValue, values->value.list.nValue);

	for (i = 0; i < min; i++)
	{
	    if (matchEval (&matches->value.list.value[i].match, w))
	    {
		opacity = (values->value.list.value[i].i * OPAQUE) / 100;
		break;
	    }
	}
    }

    opacity = (opacity * w->opacityFactor) / 0xff;
    if (opacity != w->paint.opacity)
    {
	w->paint.opacity = opacity;
	addWindowDamage (w);
    }
}


void
updateWindowOutputExtents (CompWindow *w)
{
    CompWindowExtents output;

    (*w->screen->getOutputExtentsForWindow) (w, &output);

    if (output.left   != w->output.left  ||
	output.right  != w->output.right ||
	output.top    != w->output.top   ||
	output.bottom != w->output.bottom)
    {
	w->output = output;

	(*w->screen->windowResizeNotify) (w, 0, 0, 0, 0);
    }
}

static void
setWindowMatrix (CompWindow *w)
{
    w->matrix = w->texture->matrix;
    w->matrix.x0 -= (w->attrib.x * w->matrix.xx);
    w->matrix.y0 -= (w->attrib.y * w->matrix.yy);
}

Bool
bindWindow (CompWindow *w)
{
    redirectWindow (w);

    if (!w->pixmap)
    {
	XWindowAttributes attr;

	/* don't try to bind window again if it failed previously */
	if (w->bindFailed)
	    return FALSE;

	/* We have to grab the server here to make sure that window
	   is mapped when getting the window pixmap */
	XGrabServer (w->screen->display->display);
	XGetWindowAttributes (w->screen->display->display, w->id, &attr);
	if (attr.map_state != IsViewable)
	{
	    XUngrabServer (w->screen->display->display);
	    finiTexture (w->screen, w->texture);
	    w->bindFailed = TRUE;
	    return FALSE;
	}

	w->pixmap = XCompositeNameWindowPixmap (w->screen->display->display,
						w->id);

	XUngrabServer (w->screen->display->display);
    }

    if (!bindPixmapToTexture (w->screen, w->texture, w->pixmap,
			      w->width, w->height,
			      w->attrib.depth))
    {
	compLogMessage (w->screen->display, "core", CompLogLevelInfo,
			"Couldn't bind redirected window 0x%x to "
			"texture\n", (int) w->id);
    }

    setWindowMatrix (w);

    return TRUE;
}

void
releaseWindow (CompWindow *w)
{
    if (w->pixmap)
    {
	CompTexture *texture;

	texture = createTexture (w->screen);
	if (texture)
	{
	    destroyTexture (w->screen, w->texture);

	    w->texture = texture;
	}

	XFreePixmap (w->screen->display->display, w->pixmap);

	w->pixmap = None;
    }
}

static void
freeWindow (CompWindow *w)
{
    releaseWindow (w);

    destroyTexture (w->screen, w->texture);

    if (w->clip)
	XDestroyRegion (w->clip);

    if (w->region)
	XDestroyRegion (w->region);

    if (w->privates)
	free (w->privates);

    if (w->sizeDamage)
	free (w->damageRects);

    if (w->vertices)
	free (w->vertices);

    if (w->indices)
	free (w->indices);

    if (w->struts)
	free (w->struts);

    if (w->icon)
	freeWindowIcons (w);

    if (w->resName)
      {
	free (w->resName);
      }
    
    if (w->resClass)
	free (w->resClass);

    free (w);
}

void
damageTransformedWindowRect (CompWindow *w,
			     float	xScale,
			     float	yScale,
			     float	xTranslate,
			     float	yTranslate,
			     BoxPtr     rect)
{
    REGION reg;

    reg.rects    = &reg.extents;
    reg.numRects = 1;

    reg.extents.x1 = (rect->x1 * xScale) - 1;
    reg.extents.y1 = (rect->y1 * yScale) - 1;
    reg.extents.x2 = (rect->x2 * xScale + 0.5f) + 1;
    reg.extents.y2 = (rect->y2 * yScale + 0.5f) + 1;

    reg.extents.x1 += xTranslate;
    reg.extents.y1 += yTranslate;
    reg.extents.x2 += (xTranslate + 0.5f);
    reg.extents.y2 += (yTranslate + 0.5f);

    if (reg.extents.x2 > reg.extents.x1 && reg.extents.y2 > reg.extents.y1)
    {
	reg.extents.x1 += w->attrib.x + w->attrib.border_width;
	reg.extents.y1 += w->attrib.y + w->attrib.border_width;
	reg.extents.x2 += w->attrib.x + w->attrib.border_width;
	reg.extents.y2 += w->attrib.y + w->attrib.border_width;

	damageScreenRegion (w->screen, &reg);
    }
}

void
damageWindowOutputExtents (CompWindow *w)
{
    if (w->screen->damageMask & COMP_SCREEN_DAMAGE_ALL_MASK)
	return;

    if (w->shaded || (w->attrib.map_state == IsViewable && w->damaged))
    {
	BoxRec box;

	/* top */
	box.x1 = -w->output.left;
	box.y1 = -w->output.top;
	box.x2 = w->width + w->output.right;
	box.y2 = 0;

	if (box.x1 < box.x2 && box.y1 < box.y2)
	    addWindowDamageRect (w, &box);

	/* bottom */
	box.y1 = w->height;
	box.y2 = box.y1 + w->output.bottom;

	if (box.x1 < box.x2 && box.y1 < box.y2)
	    addWindowDamageRect (w, &box);

	/* left */
	box.x1 = -w->output.left;
	box.y1 = 0;
	box.x2 = 0;
	box.y2 = w->height;

	if (box.x1 < box.x2 && box.y1 < box.y2)
	    addWindowDamageRect (w, &box);

	/* right */
	box.x1 = w->width;
	box.x2 = box.x1 + w->output.right;

	if (box.x1 < box.x2 && box.y1 < box.y2)
	    addWindowDamageRect (w, &box);
    }
}

Bool
damageWindowRect (CompWindow *w,
		  Bool       initial,
		  BoxPtr     rect)
{
    return FALSE;
}

void
addWindowDamageRect (CompWindow *w,
		     BoxPtr     rect)
{
    REGION region;

    if (w->screen->damageMask & COMP_SCREEN_DAMAGE_ALL_MASK)
	return;

    region.extents = *rect;

    if (!(*w->screen->damageWindowRect) (w, FALSE, &region.extents))
    {
	region.extents.x1 += w->attrib.x + w->attrib.border_width;
	region.extents.y1 += w->attrib.y + w->attrib.border_width;
	region.extents.x2 += w->attrib.x + w->attrib.border_width;
	region.extents.y2 += w->attrib.y + w->attrib.border_width;

	region.rects = &region.extents;
	region.numRects = region.size = 1;

	damageScreenRegion (w->screen, &region);
    }
}

void
getOutputExtentsForWindow (CompWindow	     *w,
			   CompWindowExtents *output)
{
    output->left   = 0;
    output->right  = 0;
    output->top    = 0;
    output->bottom = 0;
}

void
addWindowDamage (CompWindow *w)
{
    if (w->screen->damageMask & COMP_SCREEN_DAMAGE_ALL_MASK)
	return;

    if (w->shaded || (w->attrib.map_state == IsViewable && w->damaged))
    {
	BoxRec box;

	box.x1 = -w->output.left - w->attrib.border_width;
	box.y1 = -w->output.top - w->attrib.border_width;
	box.x2 = w->width + w->output.right;
	box.y2 = w->height + w->output.bottom;

	addWindowDamageRect (w, &box);
    }
}

void
updateWindowRegion (CompWindow *w)
{
    REGION     rect;
    XRectangle r, *rects, *shapeRects = 0;
    int	       i, n = 0;

    EMPTY_REGION (w->region);

    if (w->screen->display->shapeExtension)
    {
	int order;

	shapeRects = XShapeGetRectangles (w->screen->display->display, w->id,
					  ShapeBounding, &n, &order);
    }

    if (n < 2)
    {
	r.x      = -w->attrib.border_width;
	r.y      = -w->attrib.border_width;
	r.width  = w->width;
	r.height = w->height;

	rects = &r;
	n = 1;
    }
    else
    {
	rects = shapeRects;
    }

    rect.rects = &rect.extents;
    rect.numRects = rect.size = 1;

    for (i = 0; i < n; i++)
    {
	rect.extents.x1 = rects[i].x + w->attrib.border_width;
	rect.extents.y1 = rects[i].y + w->attrib.border_width;
	rect.extents.x2 = rect.extents.x1 + rects[i].width;
	rect.extents.y2 = rect.extents.y1 + rects[i].height;

	if (rect.extents.x1 < 0)
	    rect.extents.x1 = 0;
	if (rect.extents.y1 < 0)
	    rect.extents.y1 = 0;
	if (rect.extents.x2 > w->width)
	    rect.extents.x2 = w->width;
	if (rect.extents.y2 > w->height)
	    rect.extents.y2 = w->height;

	if (rect.extents.y1 < rect.extents.y2 &&
	    rect.extents.x1 < rect.extents.x2)
	{
	    rect.extents.x1 += w->attrib.x;
	    rect.extents.y1 += w->attrib.y;
	    rect.extents.x2 += w->attrib.x;
	    rect.extents.y2 += w->attrib.y;

	    XUnionRegion (&rect, w->region, w->region);
	}
    }

    if (shapeRects)
	XFree (shapeRects);
}
 
static void
setDefaultWindowAttributes (XWindowAttributes *wa)
{
    wa->x		      = 0;
    wa->y		      = 0;
    wa->width		      = 1;
    wa->height		      = 1;
    wa->border_width	      = 0;
    wa->depth		      = 0;
    wa->visual		      = NULL;
    wa->root		      = None;
    wa->class		      = InputOnly;
    wa->bit_gravity	      = NorthWestGravity;
    wa->win_gravity	      = NorthWestGravity;
    wa->backing_store	      = NotUseful;
    wa->backing_planes	      = 0;
    wa->backing_pixel	      = 0;
    wa->save_under	      = FALSE;
    wa->colormap	      = None;
    wa->map_installed	      = FALSE;
    wa->map_state	      = IsUnviewable;
    wa->all_event_masks	      = 0;
    wa->your_event_mask	      = 0;
    wa->do_not_propagate_mask = 0;
    wa->override_redirect     = TRUE;
    wa->screen		      = NULL;
}

void
addWindow (CompScreen *screen,
	   Window     id,
	   Window     aboveId)
{
    B(("0x%x :addWindow\n", (unsigned int) id));
    CompWindow *w;

    w = (CompWindow *) malloc (sizeof (CompWindow));
    if (!w)
	return;

    w->next = NULL;
    w->prev = NULL;

    w->mapNum	 = 0;
    w->activeNum = 0;

    w->frame = None;

    w->placed		 = FALSE;
    w->minimized	 = FALSE;
    w->inShowDesktopMode = FALSE;
    w->shaded		 = FALSE;
    w->hidden		 = FALSE;
    w->grabbed		 = FALSE;
    w->added		 = FALSE;

    w->desktop = screen->currentDesktop;

    w->initialViewportX = screen->x;
    w->initialViewportY = screen->y;

    w->initialTimestamp	   = 0;
    w->initialTimestampSet = FALSE;

    w->pendingUnmaps = 0;
    w->pendingMaps   = 0;

    //    w->startupId = NULL;
    w->resName   = NULL;
    w->resClass  = NULL;

    w->texture = createTexture (screen);
    if (!w->texture)
    {
	free (w);
	return;
    }

    w->screen     = screen;
    w->pixmap     = None;
    w->destroyed  = FALSE;
    w->damaged    = FALSE;
    w->redirected = TRUE;

    w->bindFailed = FALSE;

    w->destroyRefCnt = 1;
    w->unmapRefCnt   = 1;

    w->group = NULL;

    w->damageRects = 0;
    w->sizeDamage  = 0;
    w->nDamage	   = 0;

    w->vertices     = 0;
    w->vertexSize   = 0;
    w->indices      = 0;
    w->indexSize    = 0;
    w->vCount	    = 0;
    w->indexCount   = 0;
    w->texCoordSize = 2;

    w->drawWindowGeometry = NULL;

    w->struts = 0;

    w->icon  = 0;
    w->nIcon = 0;

    w->input.left   = 0;
    w->input.right  = 0;
    w->input.top    = 0;
    w->input.bottom = 0;

    w->output.left   = 0;
    w->output.right  = 0;
    w->output.top    = 0;
    w->output.bottom = 0;

    w->paint.opacity	= w->opacity    = OPAQUE;
    w->paint.brightness = w->brightness = 0xffff;
    w->paint.saturation = w->saturation = COLOR;
    w->paint.xScale	= 1.0f;
    w->paint.yScale	= 1.0f;
    w->paint.xTranslate	= 0.0f;
    w->paint.yTranslate	= 0.0f;

    w->opacityFactor = 0xff;

    w->opacityPropSet = FALSE;

    w->lastPaint = w->paint;

    w->alive = TRUE;

    w->mwmDecor = MwmDecorAll;
    w->mwmFunc  = MwmFuncAll;

    w->closeRequests	    = 0;
    w->lastCloseRequestTime = 0;

    if (screen->windowPrivateLen)
    {
	w->privates = malloc (screen->windowPrivateLen * sizeof (CompPrivate));
	if (!w->privates)
	{
	    destroyTexture (screen, w->texture);
	    free (w);
	    return;
	}
    }
    else
	w->privates = 0;

    w->region = XCreateRegion ();
    if (!w->region)
    {
	freeWindow (w);
	return;
    }

    w->clip = XCreateRegion ();
    if (!w->clip)
    {
	freeWindow (w);
	return;
    }

    /* Failure means that window has been destroyed. We still have to add the
       window to the window list as we might get configure requests which
       require us to stack other windows relative to it. Setting some default
       values if this is the case. */
    if (!XGetWindowAttributes (screen->display->display, id, &w->attrib))
      {
	//freeWindow (w);
	//return;
	setDefaultWindowAttributes (&w->attrib);
      }
 
    w->serverWidth	 = w->attrib.width;
    w->serverHeight	 = w->attrib.height;
    w->serverBorderWidth = w->attrib.border_width;

    w->width  = w->attrib.width  + w->attrib.border_width * 2;
    w->height = w->attrib.height + w->attrib.border_width * 2;

    w->sizeHints.flags = 0;

    //recalcNormalHints (w);

    w->clientMapped = FALSE;
    w->clientId     = None;
    /*w->clientDamage = None;
    w->clientDamageRect.x = 0;
    w->clientDamageRect.y = 0;
    w->clientDamageRect.width  = 0xffffff;
    w->clientDamageRect.height = 0xffffff;
    */

    w->serverX = w->attrib.x;
    w->serverY = w->attrib.y;

    w->syncWait	       = FALSE;
    w->syncX	       = w->attrib.x;
    w->syncY	       = w->attrib.y;
    w->syncWidth       = w->attrib.width;
    w->syncHeight      = w->attrib.height;
    w->syncBorderWidth = w->attrib.border_width;

    w->saveMask = 0;

    XSelectInput (screen->display->display, id,
		  PropertyChangeMask |
		  EnterWindowMask    |
		  FocusChangeMask);

    w->id = id;
    /***
    XGrabButton (screen->display->display, AnyButton,
		 AnyModifier, w->id, TRUE, ButtonPressMask |
		 ButtonReleaseMask | ButtonMotionMask,
		 GrabModeSync, GrabModeSync, None, None);
    */
    w->inputHint = TRUE;
    w->alpha     = (w->attrib.depth == 32);
    w->wmType    = 0;
    w->state     = 0;
    w->actions   = 0;
    w->protocols = 0;
    w->type      = CompWindowTypeUnknownMask;
    //w->lastPong  = screen->display->lastPing;

    if (screen->display->shapeExtension)
	XShapeSelectInput (screen->display->display, id, ShapeNotifyMask);

    insertWindowIntoScreen (screen, w, aboveId);

    EMPTY_REGION (w->region);

    if (w->attrib.class != InputOnly)
    {
	REGION rect;

	rect.rects = &rect.extents;
	rect.numRects = rect.size = 1;

	rect.extents.x1 = w->attrib.x;
	rect.extents.y1 = w->attrib.y;
	rect.extents.x2 = w->attrib.x + w->width;
	rect.extents.y2 = w->attrib.y + w->height;

	XUnionRegion (&rect, w->region, w->region);

	w->damage = XDamageCreate (screen->display->display, id,
				   XDamageReportRawRectangles);

	/* need to check for DisplayModal state on all windows */
	//w->state = getWindowState (screen->display, w->id);
	//updateWindowClassHints (w);
    }
    else
    {
	w->damage = None;
	w->attrib.map_state = IsUnmapped;
	/* XXX does the compositor need to know if input only
	   windows? - maybe to move them on viewport changes  */
    }

    w->invisible = TRUE;
    
    /* check if this is an enlightenment frame window */
    unsigned int clientId = 0;
    //if (w->wmType != screen->display->winTypeDesktopAtom)
    {
	/* TODO use windowProperty32 instead! */
	unsigned char      *prop_ret = NULL;
	Atom                type_ret;
	unsigned long       bytes_after, num_ret;
	int                 format_ret;
	
	XGetWindowProperty(screen->display->display, 
			   w->id, screen->display->eManagedAtom, 
			   0, 0x7fffffff, False,
			   XA_CARDINAL, &type_ret, &format_ret, 
			   &num_ret, &bytes_after, &prop_ret);
		
	if (prop_ret && type_ret == XA_CARDINAL && format_ret == 32 && num_ret == 1)
	  {
	    clientId = (((unsigned long*)prop_ret)[0]);
	  }
	if (prop_ret)
	  XFree(prop_ret);
      }


   if(clientId)
     {
       printf("window managed: 0x%x : 0x%x\n", (unsigned int)w->id, clientId);
       /* XXX remove the use of this. all windows are override
	* redirect. */
       w->attrib.override_redirect = 0; 
       w->clientId = clientId;

       w->wmType = getWindowType (screen->display, w->clientId);
       w->protocols = getProtocols (screen->display, w->clientId);
       w->state = getWindowState (screen->display, w->clientId);
       updateWindowClassHints (w);

       //w->state = getWmState
       //updateNormalHints (w);
       //updateWindowStruts (w);
       //updateWmHints (w);
       //updateTransientHint (w);
       //getMwmHints (screen->display, w->id, &w->mwmFunc,

       recalcWindowType (w);
       
       w->opacityPropSet = readWindowProp32 (screen->display, w->id,
					     screen->display->winOpacityAtom,
					     &w->opacity);	   
       
       w->clientMapped = (w->attrib.map_state == IsViewable) ? 1 : 0;
	
       w->brightness = getWindowProp32 (screen->display, w->id,
					screen->display->winBrightnessAtom,
					BRIGHT);

       if (w->alive)
	 {
	   w->paint.opacity    = w->opacity;
	   w->paint.brightness = w->brightness;
	 }

       if (screen->canDoSaturated)
	 {
	   w->saturation =
	     getWindowProp32 (screen->display, w->id,
			      screen->display->winSaturationAtom,
			      COLOR);
	   if (w->alive)
	     w->paint.saturation = w->saturation;
	 }
     }
    else
    {
      w->wmType    = getWindowType (screen->display, w->id);
      w->protocols = getProtocols (screen->display, w->id);
      updateWindowClassHints (w);
      recalcWindowType (w);
    }
  
    if (w->attrib.map_state == IsViewable)
    {
	w->placed = TRUE;

	if (w->clientId)
	{
	    if (getWmState (screen->display, w->clientId) == IconicState)
	    {
		if (w->state & CompWindowStateShadedMask)
		    w->shaded = TRUE;
		else
		    w->minimized = TRUE;
	    }
	}

	w->attrib.map_state = IsUnmapped;
	w->pendingMaps++;

	mapWindow (w);

	updateWindowAttributes (w, CompStackingUpdateModeNormal);

	/*if (w->minimized || w->inShowDesktopMode || w->hidden || w->shaded)
	{
	    w->state |= CompWindowStateHiddenMask;

	    //w->pendingUnmaps++;

	    //XUnmapWindow (screen->display->display, w->id);

	    //setWindowState (screen->display, w->state, w->id); 
	}*/
    }
    else if (w->clientId) /* TODO check this again */
    {
      if (getWmState (screen->display, w->clientId) == IconicState)
	{
	  w->placed  = TRUE;

	  if (w->state & CompWindowStateHiddenMask)
	    {
	      if (w->state & CompWindowStateShadedMask)
		w->shaded = TRUE;
	      else
		w->minimized = TRUE;
	    }
	}
    }

    windowInitPlugins (w);

    (*w->screen->windowAddNotify) (w);

    //recalcWindowActions (w);
    updateWindowOpacity (w);

    /* if (w->shaded)
	resizeWindow (w,
		      w->attrib.x, w->attrib.y,
		      w->attrib.width, ++w->attrib.height - 1,
		      w->attrib.border_width);
    */
}

void
removeWindow (CompWindow *w)
{
  D(("0x%x : removeWindow\n", (unsigned int)w->id));
  
    unhookWindowFromScreen (w->screen, w);

    if (!w->destroyed)
    {
	CompDisplay *d = w->screen->display;

	if (w->damage)
	    XDamageDestroy (d->display, w->damage);

	if (d->shapeExtension)
	    XShapeSelectInput (d->display, w->id, NoEventMask);

	//	XSelectInput (d->display, w->id, NoEventMask);
	//	XUngrabButton (d->display, AnyButton, AnyModifier, w->id);
    }

    if (w->attrib.map_state == IsViewable && w->damaged)
    {
	if (w->type == CompWindowTypeDesktopMask)
	    w->screen->desktopWindowCount--;

	if (w->struts)
	    updateWorkareaForScreen (w->screen);
    }

    if (!w->redirected)
    {
	w->screen->overlayWindowCount--;

	if (w->screen->overlayWindowCount < 1)
	    showOutputWindow (w->screen);
    }

    windowFiniPlugins (w);

    freeWindow (w);
}

void
destroyWindow (CompWindow *w)
{
  D(("0x%x : destroyWindow\n", (unsigned int)w->id));
  
    w->id = 1;
    w->mapNum = 0;

    w->destroyRefCnt--;
    if (w->destroyRefCnt)
	return;

    if (!w->destroyed)
    {
	w->destroyed = TRUE;
	w->screen->pendingDestroys++;
    }
}

void
sendConfigureNotify (CompWindow *w)
{
    XConfigureEvent xev;

    xev.type   = ConfigureNotify;
    xev.event  = w->id;
    xev.window = w->id;

    /* normally we should never send configure notify events to override
       redirect windows but if they support the _NET_WM_SYNC_REQUEST
       protocol we need to do this when the window is mapped. however the
       only way we can make sure that the attributes we send are correct
       and is to grab the server. */
    /****/
    if (w->attrib.override_redirect)
    {
	XWindowAttributes attrib;

	XGrabServer (w->screen->display->display);

	if (XGetWindowAttributes (w->screen->display->display, w->id, &attrib))
	{
	  D(("sendConfigureNotify to override_redirect window\n"));
	  
	    xev.x	     = attrib.x;
	    xev.y	     = attrib.y;
	    xev.width	     = attrib.width;
	    xev.height	     = attrib.height;
	    xev.border_width = attrib.border_width;

	    xev.above		  = (w->prev) ? w->prev->id : None;
	    xev.override_redirect = TRUE;

	    XSendEvent (w->screen->display->display, w->id, FALSE,
			StructureNotifyMask, (XEvent *) &xev);
	}

	XUngrabServer (w->screen->display->display);
    }
    
    /*
    else
    {
       D(("sendConfigureNotify\n"));	  
	xev.x		 = w->serverX;
	xev.y		 = w->serverY;
	xev.width	 = w->serverWidth;
	xev.height	 = w->serverHeight;
	xev.border_width = w->serverBorderWidth;

	xev.above	      = (w->prev) ? w->prev->id : None;
	xev.override_redirect = w->attrib.override_redirect;

	XSendEvent (w->screen->display->display, w->id, FALSE,
		    StructureNotifyMask, (XEvent *) &xev);
     }*/
}

void
mapWindow (CompWindow *w)
{
    if (w->attrib.map_state == IsViewable)
	return;

    w->pendingMaps--;

    w->mapNum = w->screen->mapNum++;

    if (w->attrib.class == InputOnly)
	return;

    w->unmapRefCnt = 1;

    w->attrib.map_state = IsViewable;

    w->invisible = TRUE;
    w->damaged = FALSE;
    w->alive = TRUE;
    w->bindFailed = FALSE;

    //w->lastPong = w->screen->display->lastPing;

    D(("0x%x : mapWindow\n", (unsigned int) w->id));    
    updateWindowRegion (w);
    //updateWindowSize (w);

    if (w->type & CompWindowTypeDesktopMask)
	w->screen->desktopWindowCount++;

    // if (w->protocols & CompWindowProtocolSyncRequestMask)
    // {
    //	sendSyncRequest (w);
    //	sendConfigureNotify (w);
    //}

    if (!w->attrib.override_redirect)
    {
	if (!w->height)
	    resizeWindow (w,
			  w->attrib.x, w->attrib.y,
			  w->attrib.width, ++w->attrib.height - 1,
			  w->attrib.border_width);
    }
}

void
unmapWindow (CompWindow *w)
{
  B(("- unmapWindow\n"));
  
    if (w->mapNum)
    {
	w->mapNum = 0;
    }

    w->unmapRefCnt--;
    if (w->unmapRefCnt > 0)
	return;

    //if (w->struts)
    //	updateWorkareaForScreen (w->screen);

    if (w->attrib.map_state != IsViewable)
	return;

    if (w->type == CompWindowTypeDesktopMask)
	w->screen->desktopWindowCount--;

    addWindowDamage (w);

    w->attrib.map_state = IsUnmapped;

    w->invisible = TRUE;

    releaseWindow (w);

    /* if (w->shaded && w->height)
	resizeWindow (w,
		      w->attrib.x, w->attrib.y,
		      w->attrib.width, ++w->attrib.height - 1,
		      w->attrib.border_width);
    */
    if (!w->redirected)
	redirectWindow (w);
}

static int
restackWindow (CompWindow *w,
	       Window     aboveId)
{
  D(("0x%x : restack window above 0x%x\n", (unsigned int)w->id, aboveId ? (unsigned int) aboveId : 0));
  
    if (w->prev)
    {
	if (aboveId && aboveId == w->prev->id)
	    return 0;
    }
    else if (aboveId == None && !w->next)
	return 0;

    unhookWindowFromScreen (w->screen, w);
    insertWindowIntoScreen (w->screen, w, aboveId);

    return 1;
}

Bool
resizeWindow (CompWindow *w,
	      int	 x,
	      int	 y,
	      int	 width,
	      int	 height,
	      int	 borderWidth)
{
  D(("0x%x : resizeWindow: %d:%d %dx%d\n", (unsigned int)w->id, x,y,width,height));
  
    if (w->attrib.width        != width  ||
	w->attrib.height       != height ||
	w->attrib.border_width != borderWidth)
    {
	unsigned int pw, ph, actualWidth, actualHeight, ui;
	int	     dx, dy, dwidth, dheight;
	Pixmap	     pixmap = None;
	Window	     root;
	Status	     result;
	int	     i;

	pw = width  + borderWidth * 2;
	ph = height + borderWidth * 2;

	if (w->mapNum && w->redirected)
	{
	    pixmap = XCompositeNameWindowPixmap (w->screen->display->display,
						 w->id);
	    result = XGetGeometry (w->screen->display->display, pixmap, &root,
				   &i, &i, &actualWidth, &actualHeight,
				   &ui, &ui);

	    if (!result || actualWidth != pw || actualHeight != ph)
	    {
		XFreePixmap (w->screen->display->display, pixmap);

		return FALSE;
	    }
	}
	else if (w->shaded)
	{
	    ph = 0;
	}

	addWindowDamage (w);

	dx      = x - w->attrib.x;
	dy      = y - w->attrib.y;
	dwidth  = width - w->attrib.width;
	dheight = height - w->attrib.height;

	w->attrib.x	       = x;
	w->attrib.y	       = y;
	w->attrib.width	       = width;
	w->attrib.height       = height;
	w->attrib.border_width = borderWidth;

	w->width  = pw;
	w->height = ph;

	releaseWindow (w);

	w->pixmap = pixmap;

	if (w->mapNum)
	    updateWindowRegion (w);

	(*w->screen->windowResizeNotify) (w, dx, dy, dwidth, dheight);

	addWindowDamage (w);

	w->invisible = WINDOW_INVISIBLE (w);
    }
    else if (w->attrib.x != x || w->attrib.y != y)
       {
    	int dx, dy;
    
    	dx = x - w->attrib.x;
    	dy = y - w->attrib.y;
	/* FIXME needed ? */
    	moveWindow (w, dx, dy, TRUE, TRUE);
       }
    
    return TRUE;
}

void
configureWindow (CompWindow	 *w,
		 XConfigureEvent *ce)
{
  C(("- configureWindow %d:%d %dx%d\n", ce->x, ce->y, ce->width, ce->height));
  
    if (w->clientId)
      {
	w->attrib.override_redirect = FALSE;
      }
    else
      w->attrib.override_redirect = ce->override_redirect;

    /*FIXME cleanup !!!*/
    /*
    int dy = (w->desktop / w->screen->hsize); 
    int dx = (w->desktop - (dy * w->screen->hsize));

    dx -= w->screen->x;
    dy -= w->screen->y;
    
    dx *= w->screen->width;
    dy *= w->screen->height;
    
    ce->x += dx;
    ce->y += dy;
    */
    /*  if (w->syncWait)
    {
	w->syncX	   = ce->x;
	w->syncY	   = ce->y;
	w->syncWidth       = ce->width;
	w->syncHeight      = ce->height;
	w->syncBorderWidth = ce->border_width;
    }
    else*/
    {
      // if (w->attrib.override_redirect) /* check if this updated
      //	w->serverWidth correctly (for getDefaultViewport)*/ 
	{
	    w->serverX		 = ce->x;
	    w->serverY		 = ce->y;
	    w->serverWidth       = ce->width;
	    w->serverHeight      = ce->height;
	    w->serverBorderWidth = ce->border_width;
	}

	resizeWindow (w, ce->x, ce->y, ce->width, ce->height,
		      ce->border_width);
    }
  
    if (restackWindow (w, ce->above))
      {
	addWindowDamage (w);
      }
}


void
circulateWindow (CompWindow	 *w,
		 XCirculateEvent *ce)
{
    Window newAboveId;

    if (ce->place == PlaceOnTop)
	newAboveId = getTopWindow (w->screen);
    else
	newAboveId = 0;

    if (restackWindow (w, newAboveId))
	addWindowDamage (w);
}

void
moveWindow (CompWindow *w,
	    int        dx,
	    int        dy,
	    Bool       damage,
	    Bool       immediate)
{
    if (dx || dy)
    {
	if (damage)
	    addWindowDamage (w);

	w->attrib.x += dx;
	w->attrib.y += dy;
	
	XOffsetRegion (w->region, dx, dy);

	setWindowMatrix (w);

	w->invisible = WINDOW_INVISIBLE (w);

	(*w->screen->windowMoveNotify) (w, dx, dy, immediate);

	if (damage)
	    addWindowDamage (w);
    }
}

void
syncWindowPosition (CompWindow *w)
{
  D(("0x%x : syncWindowPosition: %d:%d\n", (unsigned int) w->id, w->attrib.x,  w->attrib.y ));
    w->serverX = w->attrib.x;
    w->serverY = w->attrib.y;

    XMoveWindow (w->screen->display->display, w->id, w->attrib.x, w->attrib.y);
    /* we moved without resizing, so we have to send a configure notify */
    sendConfigureNotify (w);
}

Bool
focusWindow (CompWindow *w)
{
    if (w->attrib.override_redirect)
	return FALSE;
      
    if (!w->clientId)
	return FALSE;
 
    if (!w->shaded && (w->state & CompWindowStateHiddenMask))
	return FALSE;
  
    D(("focusWindow: %d:%d\n",w->attrib.x,  w->attrib.y));
    
    if (w->attrib.x + w->width  <= 0	||
	w->attrib.y + w->height <= 0	||
	w->attrib.x >= w->screen->width ||
	w->attrib.y >= w->screen->height)
	return FALSE;
  
    return TRUE;
}

Bool
placeWindow (CompWindow *w,
	     int        x,
	     int        y,
	     int        *newX,
	     int        *newY)
{
    return FALSE;
}

void
windowResizeNotify (CompWindow *w,
		    int        dx,
		    int	       dy,
		    int	       dwidth,
		    int        dheight)
{
}

void
windowMoveNotify (CompWindow *w,
		  int	     dx,
		  int	     dy,
		  Bool	     immediate)
{
}

void
windowGrabNotify (CompWindow   *w,
		  int	       x,
		  int	       y,
		  unsigned int state,
		  unsigned int mask)
{
    w->grabbed = TRUE;
}

void
windowUngrabNotify (CompWindow *w)
{
    w->grabbed = FALSE;
}

void
windowStateChangeNotify (CompWindow   *w,
			 unsigned int lastState)
{
}


void
moveInputFocusToWindow (CompWindow *w)
{
    CompScreen  *s = w->screen;
    CompDisplay *d = s->display;
    //CompWindow  *modalTransient;

    XEvent ev;
    
    ev.type		    = ClientMessage;
    ev.xclient.window	    = w->id;
    ev.xclient.message_type = d->winActiveAtom;
    ev.xclient.format	    = 32;
    ev.xclient.data.l[0]    = 2;
    ev.xclient.data.l[1]    = getCurrentTimeFromDisplay (d);
    ev.xclient.data.l[2]    = 0;
    ev.xclient.data.l[3]    = 0;
    ev.xclient.data.l[4]    = 0;
    
    XSendEvent (d->display, w->id, FALSE, NoEventMask, &ev);

}

static Bool
stackLayerCheck (CompWindow *w,
		 Window	    clientLeader,
		 CompWindow *below)
{
   if (w->state & CompWindowStateAboveMask)
   {
	return TRUE;
   }
   else if (w->state & CompWindowStateBelowMask)
   {
	if (below->state & CompWindowStateBelowMask)
	    return TRUE;
   }
   else if (!(below->state & CompWindowStateAboveMask))
   {
	return TRUE;
   }

   return FALSE;
}

/* goes through the stack, top-down until we find a window we should
   stack above, normal windows can be stacked above fullscreen windows
   if aboveFs is TRUE. */
static CompWindow *
findSiblingBelow (CompWindow *w,
		  Bool	     aboveFs)
{
   CompWindow   *below;
   Window	 clientLeader = None; //w->clientLeader;
   unsigned int type = w->type;
   unsigned int belowMask;

   if (aboveFs)
	belowMask = CompWindowTypeDockMask;
   else
	belowMask = CompWindowTypeDockMask | CompWindowTypeFullscreenMask;

   /* normal stacking of fullscreen windows with below state */
   //if ((type & CompWindowTypeFullscreenMask) &&
	//	(w->state & CompWindowStateBelowMask))
     //type = CompWindowTypeNormalMask;

   for (below = w->screen->reverseWindows; below; below = below->prev)
   {
     if (below == w) // || avoidStackingRelativeTo (below))
	    continue;

	//always above desktop windows
	if (below->type & CompWindowTypeDesktopMask)
	    return below;

	switch (type) {
	case CompWindowTypeDesktopMask:  //desktop window layer
	 break;
	case CompWindowTypeFullscreenMask:
	case CompWindowTypeDockMask:  //fullscreen and dock layer
	 if (below->type & (CompWindowTypeFullscreenMask |
			       CompWindowTypeDockMask))
	    {
		if (stackLayerCheck (w, clientLeader, below))
		    return below;
	    }
	    else
	    {
		return below;
	    }
	    break;
	default:  //fullscreen and normal layer
	 if (!(below->type & belowMask))
	    {
		if (stackLayerCheck (w, clientLeader, below))
		    return below;
	    }
	    break;
	 }
   }

   return NULL;
}

void
configureXWindow (CompWindow	 *w,
		  unsigned int	 valueMask,
		  XWindowChanges *xwc)
{
  D(("configureXWindow, %d:%d %dx%d\n", xwc->x, xwc->y, xwc->width, xwc->height));
  
    if (valueMask & CWX)
	w->serverX = xwc->x;

    if (valueMask & CWY)
	w->serverY = xwc->y;
    
    if (valueMask & CWWidth)
	w->serverWidth = xwc->width;

    if (valueMask & CWHeight)
	w->serverHeight	= xwc->height;

    if (valueMask & CWBorderWidth)
	w->serverBorderWidth = xwc->border_width;


    /**  YEAH this finally fixes the issue that dialog were not
	 raised !!!!!! =)*/
    /* if (valueMask)
    {
	XConfigureWindow (w->screen->display->display, w->id, valueMask, xwc);
    	if (!(valueMask & (CWWidth | CWHeight)))
	{
	    // we have to send a configure notify event if we move or restack
	     //  a window without resizing it according to ICCCM 4.1.5 
    	    sendConfigureNotify (w);
	}
    }
    */
}

static int
addWindowStackChanges (CompWindow     *w,
		       XWindowChanges *xwc,
		       CompWindow     *sibling)
{
    int	mask = 0;

    if (!sibling || sibling->id != w->id)
    {
      	if (w->prev)
	{
	    if (!sibling)
	    {
		XLowerWindow (w->screen->display->display, w->id);
	    }
	    else if (sibling->id != w->prev->id)
	    {
		mask |= CWSibling | CWStackMode;

		xwc->stack_mode = Above;
		xwc->sibling    = sibling->id;
	    }
	}
	else
      if (sibling)
	{
	    mask |= CWSibling | CWStackMode;

	    xwc->stack_mode = Above;
	    xwc->sibling    = sibling->id;
	}
    }
    /**>>**/ 
    /*if (sibling && mask)
      {*/
    	/* a normal window can be stacked above fullscreen windows but we
    	   don't want normal windows to be stacked above dock window so if
    	   the sibling we're stacking above is a fullscreen window we also
    	   update all dock windows. */
    /*	if ((sibling->type & CompWindowTypeFullscreenMask) &&
    	    (!(w->type & (CompWindowTypeFullscreenMask |
    			  CompWindowTypeDockMask))) &&
    	    !isAncestorTo (w, sibling))
    	{
    	    CompWindow *dw;

    	    for (dw = w->screen->reverseWindows; dw; dw = dw->prev)
    		if (dw == sibling)
    		    break;

    	    for (; dw; dw = dw->prev)
    		if (dw->type & CompWindowTypeDockMask)
    		    configureXWindow (dw, mask, xwc);
    	}
	}*/
    /**<<**/
    return mask;
}

void
raiseWindow (CompWindow *w)
{
  D(("0x%x : raiseWindow\n", (unsigned int)w->id));
  XEvent ev;

  ev.type		  = ClientMessage;
  ev.xclient.window	  = w->id;
  ev.xclient.message_type = w->screen->display->restackWindowAtom;
  ev.xclient.format	  = 32;
  ev.xclient.data.l[0]    = 2;
  ev.xclient.data.l[1]    = 0;
  ev.xclient.data.l[2]    = Above;
  ev.xclient.data.l[3]    = 1;
  ev.xclient.data.l[4]    = 0;

  XSendEvent (w->screen->display->display,
	      w->screen->root, 
	      FALSE, 
	      SubstructureRedirectMask | StructureNotifyMask,
	      &ev);
}

void
lowerWindow (CompWindow *w)
{
  D(("0x%x : lowerWindow\n", (unsigned int)w->id));
  XEvent ev;

  ev.type		  = ClientMessage;
  ev.xclient.window	  = w->id;
  ev.xclient.message_type = w->screen->display->restackWindowAtom;
  ev.xclient.format	  = 32;
  ev.xclient.data.l[0]    = 2;
  ev.xclient.data.l[1]    = 0;
  ev.xclient.data.l[2]    = Below;
  ev.xclient.data.l[3]    = 1;
  ev.xclient.data.l[4]    = 0;

  XSendEvent (w->screen->display->display,
	      w->screen->root, 
	      FALSE, 
	      SubstructureRedirectMask | StructureNotifyMask,
	      &ev);
}

void
restackWindowAbove (CompWindow *w,
		    CompWindow *sibling)
{
  D(("0x%x : restackWindowAbove: 0x%x\n", (unsigned int)w->id, (unsigned int) sibling->id));

  XEvent ev;

  ev.type		  = ClientMessage;
  ev.xclient.window	  = w->id;
  ev.xclient.message_type = w->screen->display->restackWindowAtom;
  ev.xclient.format	  = 32;
  ev.xclient.data.l[0]    = 2;
  ev.xclient.data.l[1]    = sibling->id;
  ev.xclient.data.l[2]    = Above;
  ev.xclient.data.l[3]    = 1;
  ev.xclient.data.l[4]    = 0;

  XSendEvent (w->screen->display->display,
	      w->screen->root, 
	      FALSE, 
	      SubstructureRedirectMask | StructureNotifyMask,
	      &ev);
}


void
restackWindowBelow (CompWindow *w,
		    CompWindow *sibling)
{
  D(("0x%x : restackWindowBelow: 0x%x\n", (unsigned int)w->id, (unsigned int) sibling->id));
  XEvent ev;

  ev.type		  = ClientMessage;
  ev.xclient.window	  = w->id;
  ev.xclient.message_type = w->screen->display->restackWindowAtom;
  ev.xclient.format	  = 32;
  ev.xclient.data.l[0]    = 2;
  ev.xclient.data.l[1]    = sibling->id;
  ev.xclient.data.l[2]    = Below;
  ev.xclient.data.l[3]    = 1;
  ev.xclient.data.l[4]    = 0;

  XSendEvent (w->screen->display->display,
	      w->screen->root, 
	      FALSE, 
	      SubstructureRedirectMask | StructureNotifyMask,
	      &ev);
}


/* TODO cleanup __________________________________________________ */
void
updateWindowAttributes (CompWindow             *w,
			CompStackingUpdateMode stackingMode)
{
  D(("0x%x : updateWindowAttributes\n", (unsigned int) w->id));
  
    XWindowChanges xwc;
    int		   mask = 0;

    if (w->attrib.override_redirect || !w->clientId)
	return;

    if (w->state & CompWindowStateShadedMask)
    {
	hideWindow (w);
    }
    else if (w->shaded)
    {
	showWindow (w);
    }

     if (stackingMode != CompStackingUpdateModeNone)
    {
	Bool aboveFs;

	aboveFs = (stackingMode == CompStackingUpdateModeAboveFullscreen);
	mask |= addWindowStackChanges (w, &xwc, findSiblingBelow (w, aboveFs));
    }
    
    if (stackingMode == CompStackingUpdateModeInitialMap)
    {
	  /* If we are called from the MapRequest handler, we have to
		 immediately update the internal stack. If we don't do that,
		 the internal stacking order is invalid until the ConfigureNotify
		 arrives because we put the window at the top of the stack when
		 it was created */
	  if (mask & CWStackMode)
		{
		  Window above = (mask & CWSibling) ? xwc.sibling : 0;
		  restackWindow (w, above);
        }
    }

    //    mask |= addWindowSizeChanges (w, &xwc,
    //				  w->serverX, w->serverY,
    //				  w->serverWidth, w->serverHeight,
    //				  w->serverBorderWidth);
    
    xwc.x = w->serverX;
    xwc.y = w->serverY;
    xwc.width = w->serverWidth;
    xwc.height = w->serverHeight;

    //    if (!mask)
    //    	return;
    //
    //    if (w->mapNum && (mask & (CWWidth | CWHeight)))
    //    	sendSyncRequest (w);
    //    
    //    /* if (mask & (CWSibling | CWStackMode))
    //    {
    //      // transient children above 
    //	if (stackTransients (w, NULL, &xwc))
    //	{
    //	    configureXWindow (w, mask, &xwc);
    //
    //	    //ancestors, sibilings and sibiling transients below 
    //	        stackAncestors (w, &xwc);
    //		}
    //    }
    //    //else
    //    /* TODO needed? */
    //
    //  if (!(mask & (CWSibling | CWStackMode )))
    {
    	configureXWindow (w, mask, &xwc);
    }
}


static void
sendViewportMoveRequest (CompScreen *s,
			 int	    x,
			 int	    y)
{
  D(("sendViewportMoveRequest %d:%d\n",x,y));
  
    XEvent xev;

    xev.xclient.type    = ClientMessage;
    xev.xclient.display = s->display->display;
    xev.xclient.format  = 32;

    xev.xclient.message_type = s->display->desktopViewportAtom;
    xev.xclient.window	     = s->root;

    xev.xclient.data.l[0] = x;
    xev.xclient.data.l[1] = y;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 0;
    xev.xclient.data.l[4] = 0;

    XSendEvent (s->display->display,
		s->root,
		FALSE,
		SubstructureRedirectMask | SubstructureNotifyMask,
		&xev);
}

void
activateWindow (CompWindow *w)
{
  D(("activateWindow %s\n", w->resClass));

  XEvent ev;
  int x,y;

  //setCurrentDesktop (w->screen, w->desktop);

  CompScreen *s = w->screen;

  //forEachWindowOnScreen (s, revealAncestors, (void *) w);
  //revealWindow (w);

  if (w->state & CompWindowStateHiddenMask)
    {
      w->state &= ~CompWindowStateShadedMask;
      if (w->shaded)
	showWindow (w);
    }

  if (w->state & CompWindowStateHiddenMask)
    return;

  //if (!onCurrentDesktop (w)) return;

  //ensureWindowVisibility (w);
  //updateWindowAttributes (w, CompStackingUpdateModeAboveFullscreen);
 
  ev.type		  = ClientMessage;
  ev.xclient.window	  = w->id;
  ev.xclient.message_type = s->display->restackWindowAtom;
  ev.xclient.format	  = 32;
  ev.xclient.data.l[0]    = 2;
  ev.xclient.data.l[1]    = 0;
  ev.xclient.data.l[2]    = Above;
  ev.xclient.data.l[3]    = 1;
  ev.xclient.data.l[4]    = 0;

  XSendEvent (s->display->display, s->root, FALSE, 
	      SubstructureRedirectMask | StructureNotifyMask, 
	      &ev);
  
  defaultViewportForWindow (w, &x, &y);

  if (x != s->x || y != s->y)
    sendViewportMoveRequest (s, x * s->width, y * s->height);

  moveInputFocusToWindow (w);
}


void
hideWindow (CompWindow *w)
{
    Bool onDesktop = onCurrentDesktop (w);

    if (!w->clientId)
	return;

    if (!w->minimized && !w->inShowDesktopMode && !w->hidden && onDesktop)
    {
	if (w->state & CompWindowStateShadedMask)
	{
	    w->shaded = TRUE;
	}
	else
	{
	    return;
	}
    }
    else
    {
	addWindowDamage (w);

	w->shaded = FALSE;
    }

    if (!w->pendingMaps && w->attrib.map_state != IsViewable)
	return;

    w->pendingUnmaps++;

    if (w->shaded && w->id == w->screen->display->activeWindow)
	moveInputFocusToWindow (w);

    //XUnmapWindow (w->screen->display->display, w->id);

    if (w->minimized || w->inShowDesktopMode || w->hidden || w->shaded)
	changeWindowState (w, w->state | CompWindowStateHiddenMask);
}

void
showWindow (CompWindow *w)
{
  D(("- showWindow\n"));
  
    Bool onDesktop = onCurrentDesktop (w);
    
    if (!w->clientId)
	return;

    if (w->minimized || w->inShowDesktopMode || w->hidden || !onDesktop)
    {
	/* no longer hidden but not on current desktop */
	if (!w->minimized && !w->inShowDesktopMode && !w->hidden)
	{
	    if (w->state & CompWindowStateHiddenMask)
		changeWindowState (w, w->state & ~CompWindowStateHiddenMask);
	}

	return;
    }

    /* transition from minimized to shaded */
    if (w->state & CompWindowStateShadedMask)
    {
	w->shaded = TRUE;

	if (w->height)
	    resizeWindow (w,
			  w->attrib.x, w->attrib.y,
			  w->attrib.width, ++w->attrib.height - 1,
			  w->attrib.border_width);

	addWindowDamage (w);

	return;
    }
    else
    {
	w->shaded = FALSE;
    }

    w->pendingMaps++;

    XMapWindow (w->screen->display->display, w->id);

    changeWindowState (w, w->state & ~CompWindowStateHiddenMask);
}

void
unredirectWindow (CompWindow *w)
{
    if (!w->redirected)
	return;

    releaseWindow (w);

    XCompositeUnredirectWindow (w->screen->display->display, w->id,
				CompositeRedirectManual);

    w->redirected = FALSE;
    w->screen->overlayWindowCount++;

    if (w->screen->overlayWindowCount > 0)
	hideOutputWindow (w->screen);
}

void
redirectWindow (CompWindow *w)
{
    if (w->redirected)
	return;

    XCompositeRedirectWindow (w->screen->display->display, w->id,
			      CompositeRedirectManual);

    w->redirected = TRUE;
    w->screen->overlayWindowCount--;

    if (w->screen->overlayWindowCount < 1)
	showOutputWindow (w->screen);
}

void
defaultViewportForWindow (CompWindow *w,
			  int	     *vx,
			  int	     *vy)
{
    viewportForGeometry (w->screen,
			 w->serverX, w->serverY,
			 w->serverWidth, w->serverHeight,
			 w->serverBorderWidth,
			 vx, vy);
  D(("0x%x : defaultViewportForWindow %d:%d %dx%d -> %d:%d\n", (unsigned int)w->id,  w->serverX, w->serverY,  
     w->serverWidth, w->serverHeight, vx ? *vx : 0,vy ? *vy : 0));
}

/* returns icon with dimensions as close as possible to width and height
   but never greater. */
CompIcon *
getWindowIcon (CompWindow *w,
	       int	  width,
	       int	  height)
{
    CompIcon *icon;
    int	     i, wh, diff, oldDiff;
    
    /* need to fetch icon property */
    if (w->nIcon == 0)
    {
	Atom	      actual;
	int	      result, format;
	unsigned long n, left;
	unsigned char *data;

	result = XGetWindowProperty (w->screen->display->display, w->id,
				     w->screen->display->wmIconAtom,
				     0L, 65536L,
				     FALSE, XA_CARDINAL,
				     &actual, &format, &n,
				     &left, &data);

	if (result == Success && n && data)
	{
	    CompIcon **pIcon;
	    CARD32   *p;
	    int      iw, ih, j;

	    for (i = 0; i + 2 < n; i += iw * ih + 2)
	    {
		unsigned long *idata = (unsigned long *) data;

		iw  = idata[i];
		ih = idata[i + 1];

		if (iw * ih + 2 > n - i)
		    break;

		if (iw && ih)
		{
		    icon = malloc (sizeof (CompIcon) +
				   iw * ih * sizeof (CARD32));
		    if (!icon)
			continue;

		    pIcon = realloc (w->icon,
				     sizeof (CompIcon *) * (w->nIcon + 1));
		    if (!pIcon)
		    {
			free (icon);
			continue;
		    }

		    w->icon = pIcon;
		    w->icon[w->nIcon] = icon;
		    w->nIcon++;

		    icon->width  = iw;
		    icon->height = ih;

		    initTexture (w->screen, &icon->texture);

		    p = (CARD32 *) (icon + 1);

		    for (j = 0; j < iw * ih; j++)
		    {
		      p[j] = idata[i + j + 2];
		    }
		}
	    }

	    XFree (data);
	}

	/* don't fetch property again */
	if (w->nIcon == 0)
	    w->nIcon = -1;
    }

    /* no icons available for this window */
    if (w->nIcon == -1)
	return NULL;

    icon = NULL;
    wh   = width + height;

    for (i = 0; i < w->nIcon; i++)
      {
	/* we also pass icons if they are greater */
	/*if (w->icon[i]->width > width || w->icon[i]->height > height)
      	 *   continue;
	 */
	if (icon)
	{
	  diff    = abs(wh - (w->icon[i]->width + w->icon[i]->height));
	  oldDiff = abs(wh - (icon->width + icon->height));

	  if (diff < oldDiff)
		icon = w->icon[i];
	}
	else
	    icon = w->icon[i];
    }
    
    return icon;
}

void
freeWindowIcons (CompWindow *w)
{
    int i;

    for (i = 0; i < w->nIcon; i++)
    {
	finiTexture (w->screen, &w->icon[i]->texture);
	free (w->icon[i]);
    }

    if (w->icon)
    {
	free (w->icon);
	w->icon = NULL;
    }

    w->nIcon = 0;
}

int
outputDeviceForWindow (CompWindow *w)
{
    int output = w->screen->currentOutputDev;
    int	width = w->serverWidth + w->serverBorderWidth * 2;
    int	height = w->serverHeight + w->serverBorderWidth * 2;
    int x1, y1, x2, y2;

    x1 = w->screen->outputDev[output].region.extents.x1;
    y1 = w->screen->outputDev[output].region.extents.y1;
    x2 = w->screen->outputDev[output].region.extents.x2;
    y2 = w->screen->outputDev[output].region.extents.y2;

    if (x1 > w->serverX + width  ||
	y1 > w->serverY + height ||
	x2 < w->serverX		 ||
	y2 < w->serverY)
    {
	output = outputDeviceForPoint (w->screen,
				       w->serverX + width  / 2,
				       w->serverY + height / 2);
    }

    return output;
}

Bool
onCurrentDesktop (CompWindow *w)
{
  //if (w->desktop == 0xffffffff || w->desktop == w->screen->currentDesktop)
	return TRUE;

  //return FALSE;
}

void
setDesktopForWindow (CompWindow   *w,
		     unsigned int desktop)
{
    if (desktop != 0xffffffff)
    {
	if (w->type & (CompWindowTypeDesktopMask | CompWindowTypeDockMask))
	    return;

	if (desktop >= w->screen->nDesktop)
	    return;
    }

    if (desktop == w->desktop)
	return;

    w->desktop = desktop;

    if (desktop == 0xffffffff || desktop == w->screen->currentDesktop)
	showWindow (w);
    else
	hideWindow (w);

    setWindowProp (w->screen->display, w->id,
		   w->screen->display->winDesktopAtom,
		   w->desktop);
}

/* The compareWindowActiveness function compares the two windows 'w1'
   and 'w2'. It returns an integer less than, equal to, or greater
   than zero if 'w1' is found, respectively, to activated longer time
   ago than, to be activated at the same time, or be activated more
   recently than 'w2'. */
int
compareWindowActiveness (CompWindow *w1,
			 CompWindow *w2)
{
    CompScreen		    *s = w1->screen;
    CompActiveWindowHistory *history = &s->history[s->currentHistory];
    int			    i;

    /* check current window history first */
    for (i = 0; i < ACTIVE_WINDOW_HISTORY_SIZE; i++)
    {
	if (history->id[i] == w1->id)
	    return 1;

	if (history->id[i] == w2->id)
	    return -1;

	if (!history->id[i])
	    break;
    }

    return w1->activeNum - w2->activeNum;
}

void
windowAddNotify (CompWindow *w)
{
    w->added = TRUE;
}
