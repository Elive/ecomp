/*
 * This file is autogenerated with bcop:
 * The Ecomp option code generator
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _WALL_OPTIONS_H
#define _WALL_OPTIONS_H

#ifndef _WALL_OPTIONS_INTERNAL
#define getCompPluginInfo wallOptionsGetCompPluginInfo
#endif

#ifdef  __cplusplus
extern "C" {
#endif

CompPluginVTable *wallOptionsGetCompPluginInfo (void);

typedef enum
{
    WallDisplayOptionShowSwitcher,
    WallDisplayOptionMiniscreen,
    WallDisplayOptionPreviewTimeout,
    WallDisplayOptionEdgeRadius,
    WallDisplayOptionOutlineColor,
    WallDisplayOptionBackgroundGradientBaseColor,
    WallDisplayOptionBackgroundGradientHighlightColor,
    WallDisplayOptionBackgroundGradientShadowColor,
    WallDisplayOptionThumbGradientBaseColor,
    WallDisplayOptionThumbGradientHighlightColor,
    WallDisplayOptionThumbHighlightGradientBaseColor,
    WallDisplayOptionThumbHighlightGradientShadowColor,
    WallDisplayOptionArrowBaseColor,
    WallDisplayOptionArrowShadowColor,
    WallDisplayOptionMoveBackground,
    WallDisplayOptionSlideDuration,
    WallDisplayOptionNum
} WallDisplayOptions;

typedef void (*wallDisplayOptionChangeNotifyProc) (CompDisplay *display, CompOption *opt, WallDisplayOptions num);

CompOption *wallGetDisplayOption (CompDisplay *d, WallDisplayOptions num);

typedef enum
{
    WallScreenOptionMmmode,
    WallScreenOptionNum
} WallScreenOptions;

typedef void (*wallScreenOptionChangeNotifyProc) (CompScreen *screen, CompOption *opt, WallScreenOptions num);

CompOption *wallGetScreenOption (CompScreen *s, WallScreenOptions num);

typedef enum
{
    MmmodeSwitchAll = 0,
    MmmodeSwitchSeparately = 1,
} WallMmmodeEnum;

Bool             wallGetShowSwitcher (CompDisplay *d);
CompOption *     wallGetShowSwitcherOption (CompDisplay *d);
void             wallSetShowSwitcherNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
Bool             wallGetMiniscreen (CompDisplay *d);
CompOption *     wallGetMiniscreenOption (CompDisplay *d);
void             wallSetMiniscreenNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
float            wallGetPreviewTimeout (CompDisplay *d);
CompOption *     wallGetPreviewTimeoutOption (CompDisplay *d);
void             wallSetPreviewTimeoutNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
int              wallGetEdgeRadius (CompDisplay *d);
CompOption *     wallGetEdgeRadiusOption (CompDisplay *d);
void             wallSetEdgeRadiusNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetOutlineColor (CompDisplay *d);
unsigned short   wallGetOutlineColorRed (CompDisplay *d);
unsigned short   wallGetOutlineColorGreen (CompDisplay *d);
unsigned short   wallGetOutlineColorBlue (CompDisplay *d);
unsigned short   wallGetOutlineColorAlpha (CompDisplay *d);
CompOption *     wallGetOutlineColorOption (CompDisplay *d);
void             wallSetOutlineColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetBackgroundGradientBaseColor (CompDisplay *d);
unsigned short   wallGetBackgroundGradientBaseColorRed (CompDisplay *d);
unsigned short   wallGetBackgroundGradientBaseColorGreen (CompDisplay *d);
unsigned short   wallGetBackgroundGradientBaseColorBlue (CompDisplay *d);
unsigned short   wallGetBackgroundGradientBaseColorAlpha (CompDisplay *d);
CompOption *     wallGetBackgroundGradientBaseColorOption (CompDisplay *d);
void             wallSetBackgroundGradientBaseColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetBackgroundGradientHighlightColor (CompDisplay *d);
unsigned short   wallGetBackgroundGradientHighlightColorRed (CompDisplay *d);
unsigned short   wallGetBackgroundGradientHighlightColorGreen (CompDisplay *d);
unsigned short   wallGetBackgroundGradientHighlightColorBlue (CompDisplay *d);
unsigned short   wallGetBackgroundGradientHighlightColorAlpha (CompDisplay *d);
CompOption *     wallGetBackgroundGradientHighlightColorOption (CompDisplay *d);
void             wallSetBackgroundGradientHighlightColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetBackgroundGradientShadowColor (CompDisplay *d);
unsigned short   wallGetBackgroundGradientShadowColorRed (CompDisplay *d);
unsigned short   wallGetBackgroundGradientShadowColorGreen (CompDisplay *d);
unsigned short   wallGetBackgroundGradientShadowColorBlue (CompDisplay *d);
unsigned short   wallGetBackgroundGradientShadowColorAlpha (CompDisplay *d);
CompOption *     wallGetBackgroundGradientShadowColorOption (CompDisplay *d);
void             wallSetBackgroundGradientShadowColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetThumbGradientBaseColor (CompDisplay *d);
unsigned short   wallGetThumbGradientBaseColorRed (CompDisplay *d);
unsigned short   wallGetThumbGradientBaseColorGreen (CompDisplay *d);
unsigned short   wallGetThumbGradientBaseColorBlue (CompDisplay *d);
unsigned short   wallGetThumbGradientBaseColorAlpha (CompDisplay *d);
CompOption *     wallGetThumbGradientBaseColorOption (CompDisplay *d);
void             wallSetThumbGradientBaseColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetThumbGradientHighlightColor (CompDisplay *d);
unsigned short   wallGetThumbGradientHighlightColorRed (CompDisplay *d);
unsigned short   wallGetThumbGradientHighlightColorGreen (CompDisplay *d);
unsigned short   wallGetThumbGradientHighlightColorBlue (CompDisplay *d);
unsigned short   wallGetThumbGradientHighlightColorAlpha (CompDisplay *d);
CompOption *     wallGetThumbGradientHighlightColorOption (CompDisplay *d);
void             wallSetThumbGradientHighlightColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetThumbHighlightGradientBaseColor (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientBaseColorRed (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientBaseColorGreen (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientBaseColorBlue (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientBaseColorAlpha (CompDisplay *d);
CompOption *     wallGetThumbHighlightGradientBaseColorOption (CompDisplay *d);
void             wallSetThumbHighlightGradientBaseColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetThumbHighlightGradientShadowColor (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientShadowColorRed (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientShadowColorGreen (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientShadowColorBlue (CompDisplay *d);
unsigned short   wallGetThumbHighlightGradientShadowColorAlpha (CompDisplay *d);
CompOption *     wallGetThumbHighlightGradientShadowColorOption (CompDisplay *d);
void             wallSetThumbHighlightGradientShadowColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetArrowBaseColor (CompDisplay *d);
unsigned short   wallGetArrowBaseColorRed (CompDisplay *d);
unsigned short   wallGetArrowBaseColorGreen (CompDisplay *d);
unsigned short   wallGetArrowBaseColorBlue (CompDisplay *d);
unsigned short   wallGetArrowBaseColorAlpha (CompDisplay *d);
CompOption *     wallGetArrowBaseColorOption (CompDisplay *d);
void             wallSetArrowBaseColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
unsigned short * wallGetArrowShadowColor (CompDisplay *d);
unsigned short   wallGetArrowShadowColorRed (CompDisplay *d);
unsigned short   wallGetArrowShadowColorGreen (CompDisplay *d);
unsigned short   wallGetArrowShadowColorBlue (CompDisplay *d);
unsigned short   wallGetArrowShadowColorAlpha (CompDisplay *d);
CompOption *     wallGetArrowShadowColorOption (CompDisplay *d);
void             wallSetArrowShadowColorNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
Bool             wallGetMoveBackground (CompDisplay *d);
CompOption *     wallGetMoveBackgroundOption (CompDisplay *d);
void             wallSetMoveBackgroundNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
float            wallGetSlideDuration (CompDisplay *d);
CompOption *     wallGetSlideDurationOption (CompDisplay *d);
void             wallSetSlideDurationNotify (CompDisplay *d, wallDisplayOptionChangeNotifyProc notify);
            
int              wallGetMmmode (CompScreen *s);
CompOption *     wallGetMmmodeOption (CompScreen *s);
void             wallSetMmmodeNotify (CompScreen *s, wallScreenOptionChangeNotifyProc notify);
            
#ifdef  __cplusplus
}
#endif

#endif
