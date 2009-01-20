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

#ifdef HAVE_CONFIG_H
#  include "../config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <ecomp.h>

#include <decoration.h>

#include <X11/Xatom.h>
#include <X11/extensions/shape.h>

static CompMetadata decorMetadata;

typedef struct _Vector {
    int	dx;
    int	dy;
    int	x0;
    int	y0;
} Vector;

#define DECOR_BARE   0
#define DECOR_NORMAL 1
#define DECOR_ACTIVE 2
#define DECOR_NUM    3

typedef struct _DecorTexture {
    struct _DecorTexture *next;
    int			 refCount;
    Pixmap		 pixmap;
    Damage		 damage;
    CompTexture		 texture;
} DecorTexture;

typedef struct _Decoration {
    int		      refCount;
    DecorTexture      *texture;
    CompWindowExtents output;
    CompWindowExtents input;
    CompWindowExtents maxInput;
    int		      minWidth;
    int		      minHeight;
    decor_quad_t      *quad;
    int		      nQuad;
} Decoration;

typedef struct _ScaledQuad {
    CompMatrix matrix;
    BoxRec     box;
    float      sx;
    float      sy;
} ScaledQuad;

typedef struct _WindowDecoration {
    Decoration *decor;
    ScaledQuad *quad;
    int	       nQuad;
} WindowDecoration;

#define DECOR_DISPLAY_OPTION_SHADOW_RADIUS   0
#define DECOR_DISPLAY_OPTION_SHADOW_OPACITY  1
#define DECOR_DISPLAY_OPTION_SHADOW_COLOR    2
#define DECOR_DISPLAY_OPTION_SHADOW_OFFSET_X 3
#define DECOR_DISPLAY_OPTION_SHADOW_OFFSET_Y 4
#define DECOR_DISPLAY_OPTION_COMMAND         5
#define DECOR_DISPLAY_OPTION_MIPMAP          6
#define DECOR_DISPLAY_OPTION_DECOR_MATCH     7
#define DECOR_DISPLAY_OPTION_SHADOW_MATCH    8
#define DECOR_DISPLAY_OPTION_NUM             9

static int displayPrivateIndex;

typedef struct _DecorDisplay {
    int			     screenPrivateIndex;
    HandleEventProc	     handleEvent;
    MatchPropertyChangedProc matchPropertyChanged;
    DecorTexture	     *textures;
    Atom		     supportingDmCheckAtom;
    Atom		     winDecorAtom;
    Atom		     decorAtom[DECOR_NUM];

    CompOption opt[DECOR_DISPLAY_OPTION_NUM];
} DecorDisplay;

typedef struct _DecorScreen {
    int	windowPrivateIndex;

    Window dmWin;

    Decoration *decor[DECOR_NUM];

    DrawWindowProc		  drawWindow;
    DamageWindowRectProc	  damageWindowRect;
    GetOutputExtentsForWindowProc getOutputExtentsForWindow;

    WindowAddNotifyProc    windowAddNotify;
    WindowMoveNotifyProc   windowMoveNotify;
    WindowResizeNotifyProc windowResizeNotify;

    WindowStateChangeNotifyProc windowStateChangeNotify;
} DecorScreen;

typedef struct _DecorWindow {
    WindowDecoration *wd;
    Decoration	     *decor;

    CompTimeoutHandle resizeUpdateHandle;
} DecorWindow;

#define GET_DECOR_DISPLAY(d)				      \
    ((DecorDisplay *) (d)->privates[displayPrivateIndex].ptr)

#define DECOR_DISPLAY(d)		     \
    DecorDisplay *dd = GET_DECOR_DISPLAY (d)

#define GET_DECOR_SCREEN(s, dd)				          \
    ((DecorScreen *) (s)->privates[(dd)->screenPrivateIndex].ptr)

#define DECOR_SCREEN(s)							   \
    DecorScreen *ds = GET_DECOR_SCREEN (s, GET_DECOR_DISPLAY (s->display))

#define GET_DECOR_WINDOW(w, ds)					  \
    ((DecorWindow *) (w)->privates[(ds)->windowPrivateIndex].ptr)

#define DECOR_WINDOW(w)					       \
    DecorWindow *dw = GET_DECOR_WINDOW  (w,		       \
		      GET_DECOR_SCREEN  (w->screen,	       \
		      GET_DECOR_DISPLAY (w->screen->display)))

#define NUM_OPTIONS(d) (sizeof ((d)->opt) / sizeof (CompOption))

static Bool
decorDrawWindow (CompWindow	      *w,
		 const CompTransform  *transform,
		 const FragmentAttrib *attrib,
		 Region		      region,
		 unsigned int	      mask)
{
    Bool status;

    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    UNWRAP (ds, w->screen, drawWindow);
    status = (*w->screen->drawWindow) (w, transform, attrib, region, mask);
    WRAP (ds, w->screen, drawWindow, decorDrawWindow);

    if (w->mapNum && ((mask & PAINT_WINDOW_TRANSFORMED_MASK) ||
	!((w->state & (MAXIMIZE_STATE | CompWindowStateFullscreenMask)) ||
	  (w->type & CompWindowTypeDesktopMask))))
      {
	
	if (mask & PAINT_WINDOW_TRANSFORMED_MASK)
	  region = &infiniteRegion;

	if (dw->wd && region->numRects)
	  {
	    WindowDecoration *wd = dw->wd;
	    REGION	     box;
	    int		     i;

	    mask |= PAINT_WINDOW_BLEND_MASK;

	    box.rects	 = &box.extents;
	    box.numRects = 1;

	    w->vCount = w->indexCount = 0;

	    for (i = 0; i < wd->nQuad; i++)
	      {
		box.extents = wd->quad[i].box;

		if (box.extents.x1 < box.extents.x2 &&
		    box.extents.y1 < box.extents.y2)
		  {
		    (*w->screen->addWindowGeometry) (w,
						     &wd->quad[i].matrix, 1,
						     &box,
						     region);
		  }
	      }

	    if (w->vCount)
	      (*w->screen->drawWindowTexture) (w,
					       &wd->decor->texture->texture,
					       attrib, mask);
	  }
      }
    
    return status;
}

static DecorTexture *
decorGetTexture (CompScreen *screen,
		 Pixmap	    pixmap)
{
    DecorTexture *texture;
    unsigned int width, height, depth, ui;
    Window	 root;
    int		 i;

    DECOR_DISPLAY (screen->display);

    for (texture = dd->textures; texture; texture = texture->next)
    {
	if (texture->pixmap == pixmap)
	{
	    texture->refCount++;
	    return texture;
	}
    }

    texture = malloc (sizeof (DecorTexture));
    if (!texture)
	return NULL;

    initTexture (screen, &texture->texture);

    if (!XGetGeometry (screen->display->display, pixmap, &root,
		       &i, &i, &width, &height, &ui, &depth))
    {
	finiTexture (screen, &texture->texture);
	free (texture);
	return NULL;
    }

    if (!bindPixmapToTexture (screen, &texture->texture, pixmap,
			      width, height, depth))
    {
	finiTexture (screen, &texture->texture);
	free (texture);
	return NULL;
    }

    if (!dd->opt[DECOR_DISPLAY_OPTION_MIPMAP].value.b)
	texture->texture.mipmap = FALSE;

    texture->damage = XDamageCreate (screen->display->display, pixmap,
				     XDamageReportRawRectangles);

    texture->refCount = 1;
    texture->pixmap   = pixmap;
    texture->next     = dd->textures;

    dd->textures = texture;

    return texture;
}

static void
decorReleaseTexture (CompScreen   *screen,
		     DecorTexture *texture)
{
    DECOR_DISPLAY (screen->display);

    texture->refCount--;
    if (texture->refCount)
	return;

    if (texture == dd->textures)
    {
	dd->textures = texture->next;
    }
    else
    {
	DecorTexture *t;

	for (t = dd->textures; t; t = t->next)
	{
	    if (t->next == texture)
	    {
		t->next = texture->next;
		break;
	    }
	}
    }

    finiTexture (screen, &texture->texture);
    free (texture);
}

static void
computeQuadBox (decor_quad_t *q,
		int	     width,
		int	     height,
		int	     *return_x1,
		int	     *return_y1,
		int	     *return_x2,
		int	     *return_y2,
		float        *return_sx,
		float        *return_sy)
{
    int   x1, y1, x2, y2;
    float sx = 1.0f;
    float sy = 1.0f;

    decor_apply_gravity (q->p1.gravity, q->p1.x, q->p1.y, width, height,
			 &x1, &y1);
    decor_apply_gravity (q->p2.gravity, q->p2.x, q->p2.y, width, height,
			 &x2, &y2);

    if (q->clamp & CLAMP_HORZ)
    {
	if (x1 < 0)
	    x1 = 0;
	if (x2 > width)
	    x2 = width;
    }

    if (q->clamp & CLAMP_VERT)
    {
	if (y1 < 0)
	    y1 = 0;
	if (y2 > height)
	    y2 = height;
    }

    if (q->stretch & STRETCH_X)
    {
	sx = (float)q->max_width / ((float)(x2 - x1));
    }
    else if (q->max_width < x2 - x1)
    {
	if (q->align & ALIGN_RIGHT)
	    x1 = x2 - q->max_width;
	else
	    x2 = x1 + q->max_width;
    }

    if (q->stretch & STRETCH_Y)
    {
	sy = (float)q->max_height / ((float)(y2 - y1));
    }
    else if (q->max_height < y2 - y1)
    {
	if (q->align & ALIGN_BOTTOM)
	    y1 = y2 - q->max_height;
	else
	    y2 = y1 + q->max_height;
    }

    *return_x1 = x1;
    *return_y1 = y1;
    *return_x2 = x2;
    *return_y2 = y2;

    if (return_sx)
	*return_sx = sx;
    if (return_sy)
	*return_sy = sy;
}

static Decoration *
decorCreateDecoration (CompScreen *screen,
		       Window	  id,
		       Atom	  decorAtom)
{
    Decoration	    *decoration;
    Atom	    actual;
    int		    result, format;
    unsigned long   n, nleft;
    unsigned char   *data;
    long	    *prop;
    Pixmap	    pixmap;
    decor_extents_t input;
    decor_extents_t maxInput;
    decor_quad_t    *quad;
    int		    nQuad;
    int		    minWidth;
    int		    minHeight;
    int		    left, right, top, bottom;
    int		    x1, y1, x2, y2;

    result = XGetWindowProperty (screen->display->display, id,
				 decorAtom, 0L, 1024L, FALSE,
				 XA_INTEGER, &actual, &format,
				 &n, &nleft, &data);
    
    if (result != Success || !n || !data)
	return NULL;

    prop = (long *) data;

    if (decor_property_get_version (prop) != decor_version ())
    {
	compLogMessage (screen->display, "decoration", CompLogLevelWarn,
			"Property ignored because "
			"version is %d and decoration plugin version is %d\n",
			decor_property_get_version (prop), decor_version ());

	XFree (data);
	return NULL;
    }

    nQuad = (n - BASE_PROP_SIZE) / QUAD_PROP_SIZE;

    quad = malloc (sizeof (decor_quad_t) * nQuad);
    if (!quad)
    {
	XFree (data);
	return NULL;
    }

    nQuad = decor_property_to_quads (prop,
				     n,
				     &pixmap,
				     &input,
				     &maxInput,
				     &minWidth,
				     &minHeight,
				     quad);

    XFree (data);

    if (!nQuad)
    {
	free (quad);
	return NULL;
    }

    decoration = malloc (sizeof (Decoration));
    if (!decoration)
    {
	free (quad);
	return NULL;
    }

    decoration->texture = decorGetTexture (screen, pixmap);
    if (!decoration->texture)
    {
	free (decoration);
	free (quad);
	return NULL;
    }

    decoration->minWidth  = minWidth;
    decoration->minHeight = minHeight;
    decoration->quad	  = quad;
    decoration->nQuad	  = nQuad;

    left   = 0;
    right  = minWidth;
    top    = 0;
    bottom = minHeight;

    while (nQuad--)
    {
	computeQuadBox (quad, minWidth, minHeight, &x1, &y1, &x2, &y2,
			NULL, NULL);

	if (x1 < left)
	    left = x1;
	if (y1 < top)
	    top = y1;
	if (x2 > right)
	    right = x2;
	if (y2 > bottom)
	    bottom = y2;

	quad++;
    }

    decoration->output.left   = -left;
    decoration->output.right  = right - minWidth;
    decoration->output.top    = -top;
    decoration->output.bottom = bottom - minHeight;

    decoration->input.left   = input.left;
    decoration->input.right  = input.right;
    decoration->input.top    = input.top;
    decoration->input.bottom = input.bottom;

    decoration->maxInput.left   = maxInput.left;
    decoration->maxInput.right  = maxInput.right;
    decoration->maxInput.top    = maxInput.top;
    decoration->maxInput.bottom = maxInput.bottom;

    decoration->refCount = 1;

    return decoration;
}

static void
decorReleaseDecoration (CompScreen *screen,
			Decoration *decoration)
{
    decoration->refCount--;
    if (decoration->refCount)
	return;

    decorReleaseTexture (screen, decoration->texture);

    free (decoration->quad);
    free (decoration);
}

static void
decorWindowUpdateDecoration (CompWindow *w)
{
    Decoration *decoration;

    DECOR_DISPLAY (w->screen->display);
    DECOR_WINDOW (w);

    decoration = decorCreateDecoration (w->screen, w->id, dd->winDecorAtom);

    if (dw->decor)
	decorReleaseDecoration (w->screen, dw->decor);

    dw->decor = decoration;
}

static WindowDecoration *
createWindowDecoration (Decoration *d)
{
    WindowDecoration *wd;

    wd = malloc (sizeof (WindowDecoration) +
		 sizeof (ScaledQuad) * d->nQuad);
    if (!wd)
	return NULL;

    d->refCount++;

    wd->decor = d;
    wd->quad  = (ScaledQuad *) (wd + 1);
    wd->nQuad = d->nQuad;

    return wd;
}

static void
destroyWindowDecoration (CompScreen	  *screen,
			 WindowDecoration *wd)
{
    decorReleaseDecoration (screen, wd->decor);
    free (wd);
}

static void
setDecorationMatrices (CompWindow *w)
{
    WindowDecoration *wd;
    int		     i;
    float	     x0, y0;
    decor_matrix_t   a;
    CompMatrix       b;


    DECOR_WINDOW (w);

    wd = dw->wd;
    if (!wd)
	return;

    for (i = 0; i < wd->nQuad; i++)
    {
	wd->quad[i].matrix = wd->decor->texture->texture.matrix;

	x0 = wd->decor->quad[i].m.x0;
	y0 = wd->decor->quad[i].m.y0;

	a = wd->decor->quad[i].m;
	b = wd->quad[i].matrix;

	wd->quad[i].matrix.xx = a.xx * b.xx + a.yx * b.xy;
	wd->quad[i].matrix.yx = a.xx * b.yx + a.yx * b.yy;
	wd->quad[i].matrix.xy = a.xy * b.xx + a.yy * b.xy;
	wd->quad[i].matrix.yy = a.xy * b.yx + a.yy * b.yy;
	wd->quad[i].matrix.x0 = x0 * b.xx + y0 * b.xy + b.x0;
	wd->quad[i].matrix.y0 = x0 * b.yx + y0 * b.yy + b.y0;

	wd->quad[i].matrix.xx *= wd->quad[i].sx;
	wd->quad[i].matrix.yx *= wd->quad[i].sx;
	wd->quad[i].matrix.xy *= wd->quad[i].sy;
	wd->quad[i].matrix.yy *= wd->quad[i].sy;

	if (wd->decor->quad[i].align & ALIGN_RIGHT)
	    x0 = wd->quad[i].box.x2 - wd->quad[i].box.x1;
	else
	    x0 = 0.0f;

	if (wd->decor->quad[i].align & ALIGN_BOTTOM)
	    y0 = wd->quad[i].box.y2 - wd->quad[i].box.y1;
	else
	    y0 = 0.0f;

	wd->quad[i].matrix.x0 -=
	    x0 * wd->quad[i].matrix.xx +
	    y0 * wd->quad[i].matrix.xy;

	wd->quad[i].matrix.y0 -=
	    y0 * wd->quad[i].matrix.yy +
	    x0 * wd->quad[i].matrix.yx;

	wd->quad[i].matrix.x0 -=
	    wd->quad[i].box.x1 * wd->quad[i].matrix.xx +
	    wd->quad[i].box.y1 * wd->quad[i].matrix.xy;

	wd->quad[i].matrix.y0 -=
	    wd->quad[i].box.y1 * wd->quad[i].matrix.yy +
	    wd->quad[i].box.x1 * wd->quad[i].matrix.yx;
    }
}

static void
updateWindowDecorationScale (CompWindow *w)
{
    WindowDecoration *wd;
    int		     x1, y1, x2, y2;
    float            sx, sy;
    int		     i;

    DECOR_WINDOW (w);

    wd = dw->wd;
    if (!wd)
	return;

    for (i = 0; i < wd->nQuad; i++)
    {
	computeQuadBox (&wd->decor->quad[i], w->width, w->height,
			&x1, &y1, &x2, &y2, &sx, &sy);

	wd->quad[i].box.x1 = x1 + w->attrib.x;
	wd->quad[i].box.y1 = y1 + w->attrib.y;
	wd->quad[i].box.x2 = x2 + w->attrib.x;
	wd->quad[i].box.y2 = y2 + w->attrib.y;
	wd->quad[i].sx     = sx;
	wd->quad[i].sy     = sy;
    }

    setDecorationMatrices (w);
}

/*static Bool
decorCheckSize (CompWindow *w,
		Decoration *decor)
{
    return (decor->minWidth <= w->width && decor->minHeight <= w->height);
}
*/
static int
decorWindowShiftX (CompWindow *w)
{
    switch (w->sizeHints.win_gravity) {
    case WestGravity:
    case NorthWestGravity:
    case SouthWestGravity:
	return w->input.left;
    case EastGravity:
    case NorthEastGravity:
    case SouthEastGravity:
	return -w->input.right;
    }

    return 0;
}

static int
decorWindowShiftY (CompWindow *w)
{
    switch (w->sizeHints.win_gravity) {
    case NorthGravity:
    case NorthWestGravity:
    case NorthEastGravity:
	return w->input.top;
    case SouthGravity:
    case SouthWestGravity:
    case SouthEastGravity:
	return -w->input.bottom;
    }

    return 0;
}

static Bool
decorWindowUpdate (CompWindow *w,
		   Bool	      allowDecoration)
{
    WindowDecoration *wd;
    Decoration	     *old, *decor = NULL;
    Bool	     decorate = FALSE;
    //CompMatch	     *match;
    int		     moveDx, moveDy;
    int		     oldShiftX = 0;
    int		     oldShiftY  = 0;

    //DECOR_DISPLAY (w->screen->display);
    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    wd = dw->wd;
    old = (wd) ? wd->decor : NULL;

    switch (w->type) {
    case CompWindowTypeDialogMask:
    case CompWindowTypeModalDialogMask:
    case CompWindowTypeUtilMask:
    case CompWindowTypeNormalMask:
      if (w->mwmDecor & (MwmDecorAll | MwmDecorTitle))
	decorate = TRUE;
    default:
	break;
    }
    
    // if (w->attrib.override_redirect)
    //	decorate = FALSE;

  if (w->region->numRects == 1 && !w->alpha)
		decor = ds->decor[DECOR_BARE];

  /* if (decorate)
    {
	match = &dd->opt[DECOR_DISPLAY_OPTION_DECOR_MATCH].value.match;
	if (!matchEval (match, w))
	    decorate = FALSE;
    }

    if (decorate)
    {
	if (dw->decor && decorCheckSize (w, dw->decor))
	{
	    decor = dw->decor;
	}
	else
	{
	    if (w->id == w->screen->display->activeWindow)
		decor = ds->decor[DECOR_ACTIVE];
	    else
		decor = ds->decor[DECOR_NORMAL];
	}
    }
    else
    {
	match = &dd->opt[DECOR_DISPLAY_OPTION_SHADOW_MATCH].value.match;
	if (matchEval (match, w))
	{
	    if (w->region->numRects == 1 && !w->alpha)
		decor = ds->decor[DECOR_BARE];

	    // no decoration on windows with below state 
	    if (w->state & CompWindowStateBelowMask)
		decor = NULL;

	    if (decor)
	    {
		if (!decorCheckSize (w, decor))
		    decor = NULL;
	    }
	}
    }
    */
    /**>**/
    // if (w->region->numRects == 1 && !w->alpha)
    //		decor = ds->decor[DECOR_BARE];
    //else
    //  decor = FALSE;

    /**<**/

    if (!ds->dmWin || !allowDecoration)
	decor = NULL;

    if (decor == old)
	return FALSE;

    damageWindowOutputExtents (w);

    if (old)
    {
	oldShiftX = decorWindowShiftX (w);
	oldShiftY = decorWindowShiftY (w);

	destroyWindowDecoration (w->screen, wd);
    }
    
    if (decor)
    {
	dw->wd = createWindowDecoration (decor);
	if (!dw->wd)
	    return FALSE;

	/*if ((w->state & MAXIMIZE_STATE) == MAXIMIZE_STATE)
	    setWindowFrameExtents (w, &decor->maxInput);
	else
	    setWindowFrameExtents (w, &decor->input);
	*/
	moveDx = decorWindowShiftX (w) - oldShiftX;
	moveDy = decorWindowShiftY (w) - oldShiftY;

	updateWindowOutputExtents (w);
	damageWindowOutputExtents (w);
	updateWindowDecorationScale (w);
    }
    else
    {
        CompWindowExtents emptyInput;

        memset (&emptyInput, 0, sizeof (emptyInput));
        //setWindowFrameExtents (w, &emptyInput);

	dw->wd = NULL;

	moveDx = -oldShiftX;
	moveDy = -oldShiftY;
    }

    if (!w->attrib.override_redirect && (moveDx || moveDy))
    {
	XWindowChanges xwc;
	unsigned int   mask = CWX | CWY;

	xwc.x = w->serverX + moveDx;
	xwc.y = w->serverY + moveDy;

	if (w->state & CompWindowStateFullscreenMask)
	    mask &= ~(CWX | CWY);

	if (w->state & CompWindowStateMaximizedHorzMask)
	    mask &= ~CWX;

	if (w->state & CompWindowStateMaximizedVertMask)
	    mask &= ~CWY;

	if (mask)
	    configureXWindow (w, mask, &xwc);
    }

    return TRUE;
}

static void
decorCheckForDmOnScreen (CompScreen *s,
			 Bool	    updateWindows)
{
    CompDisplay   *d = s->display;
    Atom	  actual;
    int		  result, format;
    unsigned long n, left;
    unsigned char *data;
    Window	  dmWin = None;

    DECOR_DISPLAY (s->display);
    DECOR_SCREEN (s);

    result = XGetWindowProperty (d->display, s->root,
				 dd->supportingDmCheckAtom, 0L, 1L, FALSE,
				 XA_WINDOW, &actual, &format,
				 &n, &left, &data);

    if (result == Success && n && data)
    {
	XWindowAttributes attr;

	memcpy (&dmWin, data, sizeof (Window));
	XFree (data);

	compCheckForError (d->display);

	XGetWindowAttributes (d->display, dmWin, &attr);

	if (compCheckForError (d->display))
	    dmWin = None;
    }

    if (dmWin != ds->dmWin)
    {
	CompWindow *w;
	int	   i;

	if (dmWin)
	{
	    for (i = 0; i < DECOR_NUM; i++)
		ds->decor[i] =
		    decorCreateDecoration (s, s->root, dd->decorAtom[i]);
	}
	else
	{
	    for (i = 0; i < DECOR_NUM; i++)
	    {
		if (ds->decor[i])
		{
		    decorReleaseDecoration (s, ds->decor[i]);
		    ds->decor[i] = 0;
		}
	    }

	    for (w = s->windows; w; w = w->next)
	    {
		DECOR_WINDOW (w);

		if (dw->decor)
		{
		    decorReleaseDecoration (s, dw->decor);
		    dw->decor = 0;
		}
	    }
	}

	ds->dmWin = dmWin;

	if (updateWindows)
	{
	    for (w = s->windows; w; w = w->next)
		decorWindowUpdate (w, TRUE);
	}
    }
}

static void
decorHandleEvent (CompDisplay *d,
		  XEvent      *event)
{
    Window     activeWindow = d->activeWindow;
    CompWindow *w;

    DECOR_DISPLAY (d);

    switch (event->type) {
    case DestroyNotify:
	w = findWindowAtDisplay (d, event->xdestroywindow.window);
	if (w)
	{
	    DECOR_SCREEN (w->screen);

	    if (w->id == ds->dmWin)
		decorCheckForDmOnScreen (w->screen, TRUE);
	}
	break;
    case MapRequest:
	w = findWindowAtDisplay (d, event->xmaprequest.window);
	if (w)
	    decorWindowUpdate (w, TRUE);
	break;
    default:
	if (event->type == d->damageEvent + XDamageNotify)
	{
	    XDamageNotifyEvent *de = (XDamageNotifyEvent *) event;
	    DecorTexture       *t;

	    for (t = dd->textures; t; t = t->next)
	    {
		if (t->pixmap == de->drawable)
		{
		    DecorWindow *dw;
		    DecorScreen *ds;
		    CompScreen  *s;

		    t->texture.oldMipmaps = TRUE;

		    for (s = d->screens; s; s = s->next)
		    {
			ds = GET_DECOR_SCREEN (s, dd);

			for (w = s->windows; w; w = w->next)
			{
			  if (/*w->shaded ||*/ w->mapNum)
			    {
				dw = GET_DECOR_WINDOW (w, ds);

				if (dw->wd && dw->wd->decor->texture == t)
				    damageWindowOutputExtents (w);
			    }
			}
		    }
		    return;
		}
	    }
	}
	break;
    }

    UNWRAP (dd, d, handleEvent);
    (*d->handleEvent) (d, event);
    WRAP (dd, d, handleEvent, decorHandleEvent);

    if (d->activeWindow != activeWindow)
    {
	w = findWindowAtDisplay (d, activeWindow);
	if (w)
	    decorWindowUpdate (w, TRUE);

	w = findWindowAtDisplay (d, d->activeWindow);
	if (w)
	    decorWindowUpdate (w, TRUE);
    }

    switch (event->type) {
    case PropertyNotify:
	if (event->xproperty.atom == dd->winDecorAtom)
	{
	  printf ("winDecorAtom 0x%x\n", (unsigned int) event->xproperty.window);
	  
	    w = findWindowAtDisplay (d, event->xproperty.window);
	    if (w)
	    {
		decorWindowUpdateDecoration (w);
		decorWindowUpdate (w, TRUE);
	    }
	}
	else if (event->xproperty.atom == d->mwmHintsAtom)
	{
	    w = findWindowAtDisplay (d, event->xproperty.window);
	    if (w)
		decorWindowUpdate (w, TRUE);
	}
	else
	{
	    CompScreen *s;

	    s = findScreenAtDisplay (d, event->xproperty.window);
	    if (s)
	    {
		if (event->xproperty.atom == dd->supportingDmCheckAtom)
		{
		    decorCheckForDmOnScreen (s, TRUE);
		}
		else
		{
		    int i;

		    for (i = 0; i < DECOR_NUM; i++)
		    {
			if (event->xproperty.atom == dd->decorAtom[i])
			{
			    DECOR_SCREEN (s);

			    if (ds->decor[i])
				decorReleaseDecoration (s, ds->decor[i]);

			    ds->decor[i] =
				decorCreateDecoration (s, s->root,
						       dd->decorAtom[i]);

			    for (w = s->windows; w; w = w->next)
				decorWindowUpdate (w, TRUE);
			}
		    }
		}
	    }
	}
	break;
    default:
	if (d->shapeExtension && event->type == d->shapeEvent + ShapeNotify)
	{
	    w = findWindowAtDisplay (d, ((XShapeEvent *) event)->window);
	    if (w)
		decorWindowUpdate (w, TRUE);
	}
	break;
    }
}

static Bool
decorDamageWindowRect (CompWindow *w,
		       Bool	  initial,
		       BoxPtr     rect)
{
    Bool status;

    DECOR_SCREEN (w->screen);

    if (initial)
	decorWindowUpdate (w, TRUE);

    UNWRAP (ds, w->screen, damageWindowRect);
    status = (*w->screen->damageWindowRect) (w, initial, rect);
    WRAP (ds, w->screen, damageWindowRect, decorDamageWindowRect);

    return status;
}

static void
decorGetOutputExtentsForWindow (CompWindow	  *w,
				CompWindowExtents *output)
{
    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    UNWRAP (ds, w->screen, getOutputExtentsForWindow);
    (*w->screen->getOutputExtentsForWindow) (w, output);
    WRAP (ds, w->screen, getOutputExtentsForWindow,
	  decorGetOutputExtentsForWindow);

    if (dw->wd)
    {
	CompWindowExtents *e = &dw->wd->decor->output;

	if (e->left > output->left)
	    output->left = e->left;
	if (e->right > output->right)
	    output->right = e->right;
	if (e->top > output->top)
	    output->top = e->top;
	if (e->bottom > output->bottom)
	    output->bottom = e->bottom;
    }
}

static CompOption *
decorGetDisplayOptions (CompPlugin  *plugin,
			CompDisplay *display,
			int	    *count)
{
    DECOR_DISPLAY (display);

    *count = NUM_OPTIONS (dd);
    return dd->opt;
}

static Bool
decorSetDisplayOption (CompPlugin      *plugin,
		       CompDisplay     *display,
		       char	       *name,
		       CompOptionValue *value)
{
    CompOption *o;
    int	       index;

    DECOR_DISPLAY (display);

    o = compFindOption (dd->opt, NUM_OPTIONS (dd), name, &index);
    if (!o)
	return FALSE;

    switch (index) {
      //        case DECOR_DISPLAY_OPTION_COMMAND:
      //	if (compSetStringOption (o, value))
      //	{
      //	    CompScreen *s;
      //
      //	    for (s = display->screens; s; s = s->next)
      //	    {
      //		DECOR_SCREEN (s);
      //
      //		if (!ds->dmWin)
      //		    runCommand (s, o->value.s);
      //	    }
      //
      //	    return TRUE;
      //	}
      //	break;
    case DECOR_DISPLAY_OPTION_DECOR_MATCH:
    case DECOR_DISPLAY_OPTION_SHADOW_MATCH:
	if (compSetMatchOption (o, value))
	{
	    CompScreen *s;
	    CompWindow *w;

	    for (s = display->screens; s; s = s->next)
		for (w = s->windows; w; w = w->next)
		    decorWindowUpdate (w, TRUE);
	}
	break;
    default:
	if (compSetOption (o, value))
	    return TRUE;
	break;
    }

    return FALSE;
}

static void
decorWindowMoveNotify (CompWindow *w,
		       int	  dx,
		       int	  dy,
		       Bool	  immediate)
{
    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    if (dw->wd)
    {
	WindowDecoration *wd = dw->wd;
	int		 i;

	for (i = 0; i < wd->nQuad; i++)
	{
	    wd->quad[i].box.x1 += dx;
	    wd->quad[i].box.y1 += dy;
	    wd->quad[i].box.x2 += dx;
	    wd->quad[i].box.y2 += dy;
	}

	setDecorationMatrices (w);
    }

    UNWRAP (ds, w->screen, windowMoveNotify);
    (*w->screen->windowMoveNotify) (w, dx, dy, immediate);
    WRAP (ds, w->screen, windowMoveNotify, decorWindowMoveNotify);
}

static Bool
decorResizeUpdateTimeout (void *closure)
{
    CompWindow *w = (CompWindow *) closure;

    DECOR_WINDOW (w);

    decorWindowUpdate (w, TRUE);

    dw->resizeUpdateHandle = 0;

    return FALSE;
}

static void
decorWindowResizeNotify (CompWindow *w,
			 int	    dx,
			 int	    dy,
			 int	    dwidth,
			 int	    dheight)
{
    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    /* FIXME: we should not need a timer for calling decorWindowUpdate, and only call
       updateWindowDecorationScale if decorWindowUpdate returns FALSE. Unfortunately,
       decorWindowUpdate may call updateWindowOutputExtents, which may call
       WindowResizeNotify. As we never should call a wrapped function that's currently
       processed, we need the timer for the moment. updateWindowOutputExtents should be
       fixed so that it does not emit a resize notification. */
    dw->resizeUpdateHandle = compAddTimeout (0, decorResizeUpdateTimeout, w);
    updateWindowDecorationScale (w);

    UNWRAP (ds, w->screen, windowResizeNotify);
    (*w->screen->windowResizeNotify) (w, dx, dy, dwidth, dheight);
    WRAP (ds, w->screen, windowResizeNotify, decorWindowResizeNotify);
}

static void
decorWindowStateChangeNotify (CompWindow   *w,
			      unsigned int lastState)
{
    DECOR_SCREEN (w->screen);
    DECOR_WINDOW (w);

    if (!decorWindowUpdate (w, TRUE))
    {
	if (dw->decor)
	{
	  /* if ((w->state & MAXIMIZE_STATE) == MAXIMIZE_STATE)
		setWindowFrameExtents (w, &dw->decor->maxInput);
	    else
	    setWindowFrameExtents (w, &dw->decor->input);*/
	}
    }

    UNWRAP (ds, w->screen, windowStateChangeNotify);
    (*w->screen->windowStateChangeNotify) (w, lastState);
    WRAP (ds, w->screen, windowStateChangeNotify, decorWindowStateChangeNotify);
}

static void
decorMatchPropertyChanged (CompDisplay *d,
			   CompWindow  *w)
{
    DECOR_DISPLAY (d);

    decorWindowUpdate (w, TRUE);

    UNWRAP (dd, d, matchPropertyChanged);
    (*d->matchPropertyChanged) (d, w);
    WRAP (dd, d, matchPropertyChanged, decorMatchPropertyChanged);
}

static void
decorWindowAddNotify (CompWindow *w)
{
    DECOR_SCREEN (w->screen);

    if (/*w->shaded ||*/ w->attrib.map_state == IsViewable)
	decorWindowUpdate (w, TRUE);

    UNWRAP (ds, w->screen, windowAddNotify);
    (*w->screen->windowAddNotify) (w);
    WRAP (ds, w->screen, windowAddNotify, decorWindowAddNotify);
}

static const CompMetadataOptionInfo decorDisplayOptionInfo[] = {
    { "shadow_radius", "float", "<min>0.0</min><max>48.0</max>", 0, 0 },
    { "shadow_opacity", "float", "<min>0.0</min>", 0, 0 },
    { "shadow_color", "color", 0, 0, 0 },
    { "shadow_x_offset", "int", "<min>-16</min><max>16</max>", 0, 0 },
    { "shadow_y_offset", "int", "<min>-16</min><max>16</max>", 0, 0 },
    { "command", "string", 0, 0, 0 },
    { "mipmap", "bool", 0, 0, 0 },
    { "decoration_match", "match", 0, 0, 0 },
    { "shadow_match", "match", 0, 0, 0 }
};

static Bool
decorInitDisplay (CompPlugin  *p,
		  CompDisplay *d)
{
    DecorDisplay *dd;

    dd = malloc (sizeof (DecorDisplay));
    if (!dd)
	return FALSE;

    if (!compInitDisplayOptionsFromMetadata (d,
					     &decorMetadata,
					     decorDisplayOptionInfo,
					     dd->opt,
					     DECOR_DISPLAY_OPTION_NUM))
    {
	free (dd);
	return FALSE;
    }

    dd->screenPrivateIndex = allocateScreenPrivateIndex (d);
    if (dd->screenPrivateIndex < 0)
    {
	compFiniDisplayOptions (d, dd->opt, DECOR_DISPLAY_OPTION_NUM);
	free (dd);
	return FALSE;
    }

    dd->textures = 0;

    dd->supportingDmCheckAtom =
	XInternAtom (d->display, DECOR_SUPPORTING_DM_CHECK_ATOM_NAME, 0);
    dd->winDecorAtom =
	XInternAtom (d->display, DECOR_WINDOW_ATOM_NAME, 0);
    dd->decorAtom[DECOR_BARE] =
	XInternAtom (d->display, DECOR_BARE_ATOM_NAME, 0);
    dd->decorAtom[DECOR_NORMAL] =
	XInternAtom (d->display, DECOR_NORMAL_ATOM_NAME, 0);
    dd->decorAtom[DECOR_ACTIVE] =
	XInternAtom (d->display, DECOR_ACTIVE_ATOM_NAME, 0);

    WRAP (dd, d, handleEvent, decorHandleEvent);
    WRAP (dd, d, matchPropertyChanged, decorMatchPropertyChanged);

    d->privates[displayPrivateIndex].ptr = dd;

    return TRUE;
}

static void
decorFiniDisplay (CompPlugin  *p,
		  CompDisplay *d)
{
    DECOR_DISPLAY (d);

    freeScreenPrivateIndex (d, dd->screenPrivateIndex);

    UNWRAP (dd, d, handleEvent);
    UNWRAP (dd, d, matchPropertyChanged);

    compFiniDisplayOptions (d, dd->opt, DECOR_DISPLAY_OPTION_NUM);

    free (dd);
}

static Bool
decorInitScreen (CompPlugin *p,
		 CompScreen *s)
{
    DecorScreen *ds;

    DECOR_DISPLAY (s->display);

    ds = malloc (sizeof (DecorScreen));
    if (!ds)
	return FALSE;

    ds->windowPrivateIndex = allocateWindowPrivateIndex (s);
    if (ds->windowPrivateIndex < 0)
    {
	free (ds);
	return FALSE;
    }

    memset (ds->decor, 0, sizeof (ds->decor));

    ds->dmWin = None;

    WRAP (ds, s, drawWindow, decorDrawWindow);
    WRAP (ds, s, damageWindowRect, decorDamageWindowRect);
    WRAP (ds, s, getOutputExtentsForWindow, decorGetOutputExtentsForWindow);
    WRAP (ds, s, windowAddNotify, decorWindowAddNotify);
    WRAP (ds, s, windowMoveNotify, decorWindowMoveNotify);
    WRAP (ds, s, windowResizeNotify, decorWindowResizeNotify);
    WRAP (ds, s, windowStateChangeNotify, decorWindowStateChangeNotify);

    s->privates[dd->screenPrivateIndex].ptr = ds;

    decorCheckForDmOnScreen (s, FALSE);

    return TRUE;
}

static void
decorFiniScreen (CompPlugin *p,
		 CompScreen *s)
{
    int i;

    DECOR_SCREEN (s);

    for (i = 0; i < DECOR_NUM; i++)
	if (ds->decor[i])
	    decorReleaseDecoration (s, ds->decor[i]);

    UNWRAP (ds, s, drawWindow);
    UNWRAP (ds, s, damageWindowRect);
    UNWRAP (ds, s, getOutputExtentsForWindow);
    UNWRAP (ds, s, windowAddNotify);
    UNWRAP (ds, s, windowMoveNotify);
    UNWRAP (ds, s, windowResizeNotify);
    UNWRAP (ds, s, windowStateChangeNotify);

    free (ds);
}

static Bool
decorInitWindow (CompPlugin *p,
		 CompWindow *w)
{
    DecorWindow *dw;

    DECOR_SCREEN (w->screen);

    dw = malloc (sizeof (DecorWindow));
    if (!dw)
	return FALSE;

    dw->wd    = NULL;
    dw->decor = NULL;

    dw->resizeUpdateHandle = 0;

    w->privates[ds->windowPrivateIndex].ptr = dw;

    if (!w->attrib.override_redirect)
	decorWindowUpdateDecoration (w);

    if (w->added && (/*w->shaded ||*/ w->attrib.map_state == IsViewable))
	decorWindowUpdate (w, TRUE);

    return TRUE;
}

static void
decorFiniWindow (CompPlugin *p,
		 CompWindow *w)
{
    DECOR_WINDOW (w);

    if (!w->destroyed)
	decorWindowUpdate (w, FALSE);

    if (dw->resizeUpdateHandle)
	compRemoveTimeout (dw->resizeUpdateHandle);

    if (dw->wd)
	destroyWindowDecoration (w->screen, dw->wd);

    if (dw->decor)
	decorReleaseDecoration (w->screen, dw->decor);

    free (dw);
}

static Bool
decorInit (CompPlugin *p)
{
    if (!compInitPluginMetadataFromInfo (&decorMetadata,
					 p->vTable->name,
					 decorDisplayOptionInfo,
					 DECOR_DISPLAY_OPTION_NUM,
					 0, 0))
	return FALSE;

    displayPrivateIndex = allocateDisplayPrivateIndex ();
    if (displayPrivateIndex < 0)
    {
	compFiniMetadata (&decorMetadata);
	return FALSE;
    }

    compAddMetadataFromFile (&decorMetadata, p->vTable->name);

    return TRUE;
}

static void
decorFini (CompPlugin *p)
{
    freeDisplayPrivateIndex (displayPrivateIndex);
    compFiniMetadata (&decorMetadata);
}

static int
decorGetVersion (CompPlugin *plugin,
		 int	    version)
{
    return ABIVERSION;
}

static CompMetadata *
decorGetMetadata (CompPlugin *plugin)
{
    return &decorMetadata;
}

static CompPluginVTable decorVTable = {
    "decoration",
    decorGetVersion,
    decorGetMetadata,
    decorInit,
    decorFini,
    decorInitDisplay,
    decorFiniDisplay,
    decorInitScreen,
    decorFiniScreen,
    decorInitWindow,
    decorFiniWindow,
    decorGetDisplayOptions,
    decorSetDisplayOption,
    0, /* GetScreenOptions */
    0  /* SetScreenOption */
};

CompPluginVTable *
getCompPluginInfo (void)
{
    return &decorVTable;
}
