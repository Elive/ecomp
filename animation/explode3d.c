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

void
fxExplode3DInit(CompScreen *s, CompWindow *w)
{
   ANIM_WINDOW(w);
   ANIM_SCREEN(s);

   switch (animGetI(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_TESS))
     {
      case PolygonTessRect:
        if (!tessellateIntoRectangles(w,
                                      animGetI(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_GRIDSIZE_X),
                                      animGetI(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_GRIDSIZE_Y),
                                      animGetF(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_THICKNESS)))
          return;
        break;

      case PolygonTessHex:
        if (!tessellateIntoHexagons(w,
                                    animGetI(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_GRIDSIZE_X),
                                    animGetI(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_GRIDSIZE_Y),
                                    animGetF(as, aw, ANIM_SCREEN_OPTION_EXPLODE3D_THICKNESS)))
          return;
        break;

      default:
        return;
     }

   PolygonSet *pset = aw->polygonSet;
   PolygonObject *p = pset->polygons;
   double sqrt2 = sqrt(2);

   int i;

   for (i = 0; i < pset->nPolygons; i++, p++)
     {
        p->rotAxis.x = RAND_FLOAT();
        p->rotAxis.y = RAND_FLOAT();
        p->rotAxis.z = RAND_FLOAT();

        float screenSizeFactor = (0.8 * DEFAULT_Z_CAMERA * s->width);
        float speed = screenSizeFactor / 10 * (0.2 + RAND_FLOAT());

        float xx = 2 * (p->centerRelPos.x - 0.5);
        float yy = 2 * (p->centerRelPos.y - 0.5);

        float x = speed * 2 * (xx + 0.5 * (RAND_FLOAT() - 0.5));
        float y = speed * 2 * (yy + 0.5 * (RAND_FLOAT() - 0.5));

        float distToCenter = sqrt(xx * xx + yy * yy) / sqrt2;
        float moveMult = 1 - distToCenter;
        moveMult = moveMult < 0 ? 0 : moveMult;
        float zbias = 0.1;
        float z = speed * 10 *
          (zbias + RAND_FLOAT() *
           pow(moveMult, 0.5));

        p->finalRelPos.x = x;
        p->finalRelPos.y = y;
        p->finalRelPos.z = z;
        p->finalRotAng = RAND_FLOAT() * 540 - 270;
     }
   pset->allFadeDuration = 0.3f;
   pset->doDepthTest = TRUE;
   pset->doLighting = TRUE;
   pset->correctPerspective = CorrectPerspectivePolygon;
   pset->backAndSidesFadeDur = 0.2f;

   aw->animTotalTime /= EXPLODE_PERCEIVED_T;
   aw->animRemainingTime = aw->animTotalTime;
}

