/*
 * Animation plugin for ecomp/beryl
 *
 * animation.c
 *
 * Copyright : (C) 2006 Erkin Bahceci
 * E-mail    : erkinbah@gmail.com
 *
 * Based on Wobbly and Minimize plugins by
 *           : David Reveman
 * E-mail    : davidr@novell.com>
 *
 * Particle system added by : (C) 2006 Dennis Kasprzyk
 * E-mail                   : onestone@beryl-project.org
 *
 * Beam-Up added by : Florencio Guimaraes
 * E-mail           : florencio@nexcorp.com.br
 *
 * Hexagon tessellator added by : Mike Slegeir
 * E-mail                       : mikeslegeir@mail.utexas.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "animation-internal.h"

// =====================  Effect: Glide  =========================

static void
fxGlideGetParams(AnimScreen *as,
                 AnimWindow *aw,
                 float      *finalDistFac,
                 float      *finalRotAng,
                 float      *thickness)
{
   if (aw->curAnimEffect == AnimEffectGlide3D1)
     {
        *finalDistFac = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE1_AWAY_POS);
        *finalRotAng = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE1_AWAY_ANGLE);
        *thickness = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE1_THICKNESS);
     }
   else
     {
        *finalDistFac = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE2_AWAY_POS);
        *finalRotAng = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE2_AWAY_ANGLE);
        *thickness = animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE2_THICKNESS);
     }
}

Bool
fxGlideIsPolygonBased(AnimScreen *as, AnimWindow *aw)
{
   if (aw->curAnimEffect == AnimEffectGlide3D1)
     return animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE1_THICKNESS) > 1e-5;
   else
     return animGetF(as, aw, ANIM_SCREEN_OPTION_GLIDE2_THICKNESS) > 1e-5;
}

static inline Bool
fxGlideZoomToTaskBar(AnimScreen *as, AnimWindow *aw)
{
   return
     (aw->curWindowEvent == WindowEventMinimize ||
      aw->curWindowEvent == WindowEventUnminimize) &&
     ((aw->curAnimEffect == AnimEffectGlide3D1 &&
       animGetB(as, aw, ANIM_SCREEN_OPTION_GLIDE1_Z2TOM)) ||
      (aw->curAnimEffect == AnimEffectGlide3D2 &&
          animGetB(as, aw, ANIM_SCREEN_OPTION_GLIDE2_Z2TOM)));
}

Bool
fxGlideLetOthersDrawGeoms(CompScreen *s, CompWindow *w)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   return !fxGlideIsPolygonBased(as, aw);
}

float
fxGlideAnimProgress(AnimWindow *aw)
{
   float forwardProgress = defaultAnimProgress(aw);

   return decelerateProgress(forwardProgress);
}

// Scales z by 0 and does perspective distortion so that it
// looks the same wherever on screen
static void
resetAndPerspectiveDistortOnZ(CompTransform *wTransform, float v)
{
   /*
      This does
      wTransform = M * wTransform, where M is
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 0, v,
      0, 0, 0, 1
    */
     float *m = wTransform->m;
     m[8] = v * m[12];
     m[9] = v * m[13];
     m[10] = v * m[14];
     m[11] = v * m[15];
}

static void
applyGlideTransform(CompWindow *w, CompTransform *transform)
{
   ANIM_SCREEN(w->screen);
   ANIM_WINDOW(w);

   float finalDistFac;
   float finalRotAng;
   float thickness;

   fxGlideGetParams(as, aw, &finalDistFac, &finalRotAng, &thickness);

   float forwardProgress;
   if (fxGlideZoomToTaskBar(as, aw))
     {
        float dummy;
        fxZoomAnimProgress(as, aw, &forwardProgress, &dummy, TRUE);
     }
   else
     forwardProgress = fxGlideAnimProgress(aw);

   float finalz = finalDistFac * 0.8 * DEFAULT_Z_CAMERA *
     w->screen->width;

   Vector3d rotAxis = {1, 0, 0};
   Point3d rotAxisOffset =
   {WIN_X(w) + WIN_W(w) / 2.0f,
    WIN_Y(w) + WIN_H(w) / 2.0f,
    0};
   Point3d translation = {0, 0, finalz * forwardProgress};

   float rotAngle = finalRotAng * forwardProgress;
   aw->glideModRotAngle = fmodf(rotAngle + 720, 360.0f);

   // put back to window position
   matrixTranslate (transform, rotAxisOffset.x, rotAxisOffset.y, 0);

   resetAndPerspectiveDistortOnZ (transform, -1.0 / w->screen->width);

   // animation movement
   matrixTranslate (transform, translation.x, translation.y, translation.z);

   // animation rotation
   matrixRotate (transform, rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

   // intentional scaling of z by 0 to prevent weird opacity results and
   // flashing that happen when z coords are between 0 and 1 (bug in ecomp?)
   matrixScale (transform, 1.0f, 1.0f, 0.0f);

   // place window rotation axis at origin
   matrixTranslate (transform, -rotAxisOffset.x, -rotAxisOffset.y, 0);
}

Bool
fxGlideAnimStep(CompScreen *s, CompWindow *w, float time)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased(as, aw))
     return polygonsAnimStep(s, w, time);
   else
     {
        if (!defaultAnimStep(s, w, time))
          return FALSE;

        applyGlideTransform (w, &aw->transform);

        return TRUE;
     }
}

void
fxGlideUpdateWindowAttrib(AnimScreen        *as,
                          CompWindow        *w,
                          WindowPaintAttrib *wAttrib)
{
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased(as, aw))
     return;

   // the effect is CompTransform-based

   if (fxGlideZoomToTaskBar(as, aw))
     {
        fxZoomUpdateWindowAttrib(as, w, wAttrib);
        return;
     }

   float forwardProgress = fxGlideAnimProgress(aw);

   wAttrib->opacity = aw->storedOpacity * (1 - forwardProgress);
}

void
fxGlideUpdateWindowTransform(CompScreen    *s,
                             CompWindow    *w,
                             CompTransform *wTransform)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased (as, aw))
     return;

   // apply the transform
   matmul4 (wTransform->m, wTransform->m, aw->transform.m);
}

void
fxGlideInit(CompScreen *s, CompWindow *w)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   if (fxGlideZoomToTaskBar(as, aw))
     {
        aw->animTotalTime /= ZOOM_PERCEIVED_T;
        aw->animRemainingTime = aw->animTotalTime;
     }

   if (!fxGlideIsPolygonBased(as, aw))
     {
        defaultAnimInit (s, w);
        return; // we're done with CompTransform-based glide initialization
     }

   // for polygon-based glide effect

   float finalDistFac;
   float finalRotAng;
   float thickness;

   fxGlideGetParams(as, aw, &finalDistFac, &finalRotAng, &thickness);

   PolygonSet *pset = aw->polygonSet;

   pset->includeShadows = (thickness < 1e-5);

   if (!tessellateIntoRectangles(w, 1, 1, thickness))
     return;

   PolygonObject *p = pset->polygons;

   int i;

   for (i = 0; i < pset->nPolygons; i++, p++)
     {
        p->rotAxis.x = 1;
        p->rotAxis.y = 0;
        p->rotAxis.z = 0;

        p->finalRelPos.x = 0;
        p->finalRelPos.y = 0;
        p->finalRelPos.z = finalDistFac * 0.8 * DEFAULT_Z_CAMERA * s->width;

        p->finalRotAng = finalRotAng;
     }
   pset->allFadeDuration = 1.0f;
   pset->backAndSidesFadeDur = 0.2f;
   pset->doLighting = TRUE;
   pset->correctPerspective = CorrectPerspectivePolygon;
}

void
fxGlidePrePaintWindow(CompScreen *s,
                      CompWindow *w)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased(as, aw))
     polygonsPrePaintWindow(s, w);
   else if (90 < aw->glideModRotAngle &&
            aw->glideModRotAngle < 270)
     glCullFace(GL_FRONT);
}

void
fxGlidePostPaintWindow(CompScreen *s, CompWindow *w)
{
   ANIM_SCREEN(s);
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased(as, aw))
     polygonsPostPaintWindow(s, w);
   else if (90 < aw->glideModRotAngle &&
            aw->glideModRotAngle < 270)
     glCullFace(GL_BACK);
}

void
fxGlideUpdateBB(CompOutput *output,
                CompWindow *w)
{
   ANIM_SCREEN(w->screen);
   ANIM_WINDOW(w);

   if (fxGlideIsPolygonBased (as, aw))
     polygonsUpdateBB (output, w);
   else
     compTransformUpdateBB (output, w);
}

