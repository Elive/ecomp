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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ecomp.h>

#define _SHIFT_OPTIONS_INTERNAL
#include "shift_options.h"

static int displayPrivateIndex;

static CompMetadata shiftOptionsMetadata;

static CompPluginVTable *shiftPluginVTable = NULL;
CompPluginVTable shiftOptionsVTable;

#define GET_SHIFT_OPTIONS_DISPLAY(d) \
        ((ShiftOptionsDisplay *) (d)->privates[displayPrivateIndex].ptr)

#define SHIFT_OPTIONS_DISPLAY(d) \
        ShiftOptionsDisplay *od = GET_SHIFT_OPTIONS_DISPLAY (d)

#define GET_SHIFT_OPTIONS_SCREEN(s, od) \
        ((ShiftOptionsScreen *) (s)->privates[(od)->screenPrivateIndex].ptr)

#define SHIFT_OPTIONS_SCREEN(s) \
        ShiftOptionsScreen *os = GET_SHIFT_OPTIONS_SCREEN (s, GET_SHIFT_OPTIONS_DISPLAY (s->display))

typedef struct _ShiftOptionsDisplay
{
    int screenPrivateIndex;

} ShiftOptionsDisplay;

typedef struct _ShiftOptionsScreen
{
    CompOption opt[ShiftScreenOptionNum];
    shiftScreenOptionChangeNotifyProc notify[ShiftScreenOptionNum];
} ShiftOptionsScreen;

float shiftGetSpeed (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionSpeed].value.f;
}

CompOption * shiftGetSpeedOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionSpeed];
}

void shiftSetSpeedNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionSpeed] = notify;
}

float shiftGetShiftSpeed (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionShiftSpeed].value.f;
}

CompOption * shiftGetShiftSpeedOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionShiftSpeed];
}

void shiftSetShiftSpeedNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionShiftSpeed] = notify;
}

float shiftGetTimestep (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTimestep].value.f;
}

CompOption * shiftGetTimestepOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTimestep];
}

void shiftSetTimestepNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTimestep] = notify;
}

CompMatch * shiftGetWindowMatch (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionWindowMatch].value.match;
}

CompOption * shiftGetWindowMatchOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionWindowMatch];
}

void shiftSetWindowMatchNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionWindowMatch] = notify;
}

Bool shiftGetMinimized (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionMinimized].value.b;
}

CompOption * shiftGetMinimizedOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionMinimized];
}

void shiftSetMinimizedNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionMinimized] = notify;
}

float shiftGetMouseSpeed (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionMouseSpeed].value.f;
}

CompOption * shiftGetMouseSpeedOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionMouseSpeed];
}

void shiftSetMouseSpeedNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionMouseSpeed] = notify;
}

int shiftGetClickDuration (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionClickDuration].value.i;
}

CompOption * shiftGetClickDurationOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionClickDuration];
}

void shiftSetClickDurationNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionClickDuration] = notify;
}

int shiftGetMode (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionMode].value.i;
}

CompOption * shiftGetModeOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionMode];
}

void shiftSetModeNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionMode] = notify;
}

int shiftGetSize (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionSize].value.i;
}

CompOption * shiftGetSizeOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionSize];
}

void shiftSetSizeNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionSize] = notify;
}

float shiftGetBackgroundIntensity (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionBackgroundIntensity].value.f;
}

CompOption * shiftGetBackgroundIntensityOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionBackgroundIntensity];
}

void shiftSetBackgroundIntensityNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionBackgroundIntensity] = notify;
}

Bool shiftGetHideAll (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionHideAll].value.b;
}

CompOption * shiftGetHideAllOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionHideAll];
}

void shiftSetHideAllNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionHideAll] = notify;
}

Bool shiftGetReflection (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionReflection].value.b;
}

CompOption * shiftGetReflectionOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionReflection];
}

void shiftSetReflectionNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionReflection] = notify;
}

unsigned short * shiftGetGroundColor1 (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor1].value.c;
}

unsigned short shiftGetGroundColor1Red (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor1].value.c[0];
}

unsigned short shiftGetGroundColor1Green (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor1].value.c[1];
}

unsigned short shiftGetGroundColor1Blue (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor1].value.c[2];
}

unsigned short shiftGetGroundColor1Alpha (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor1].value.c[3];
}

CompOption * shiftGetGroundColor1Option (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionGroundColor1];
}

void shiftSetGroundColor1Notify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionGroundColor1] = notify;
}

unsigned short * shiftGetGroundColor2 (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor2].value.c;
}

unsigned short shiftGetGroundColor2Red (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor2].value.c[0];
}

unsigned short shiftGetGroundColor2Green (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor2].value.c[1];
}

unsigned short shiftGetGroundColor2Blue (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor2].value.c[2];
}

unsigned short shiftGetGroundColor2Alpha (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundColor2].value.c[3];
}

CompOption * shiftGetGroundColor2Option (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionGroundColor2];
}

void shiftSetGroundColor2Notify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionGroundColor2] = notify;
}

float shiftGetGroundSize (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionGroundSize].value.f;
}

CompOption * shiftGetGroundSizeOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionGroundSize];
}

void shiftSetGroundSizeNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionGroundSize] = notify;
}

float shiftGetIntensity (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionIntensity].value.f;
}

CompOption * shiftGetIntensityOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionIntensity];
}

void shiftSetIntensityNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionIntensity] = notify;
}

int shiftGetFlipRotation (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionFlipRotation].value.i;
}

CompOption * shiftGetFlipRotationOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionFlipRotation];
}

void shiftSetFlipRotationNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionFlipRotation] = notify;
}

float shiftGetCoverOffset (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionCoverOffset].value.f;
}

CompOption * shiftGetCoverOffsetOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionCoverOffset];
}

void shiftSetCoverOffsetNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionCoverOffset] = notify;
}

int shiftGetOverlayIcon (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionOverlayIcon].value.i;
}

CompOption * shiftGetOverlayIconOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionOverlayIcon];
}

void shiftSetOverlayIconNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionOverlayIcon] = notify;
}

Bool shiftGetMipmaps (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionMipmaps].value.b;
}

CompOption * shiftGetMipmapsOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionMipmaps];
}

void shiftSetMipmapsNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionMipmaps] = notify;
}

int shiftGetMultioutputMode (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionMultioutputMode].value.i;
}

CompOption * shiftGetMultioutputModeOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionMultioutputMode];
}

void shiftSetMultioutputModeNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionMultioutputMode] = notify;
}

Bool shiftGetWindowTitle (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionWindowTitle].value.b;
}

CompOption * shiftGetWindowTitleOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionWindowTitle];
}

void shiftSetWindowTitleNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionWindowTitle] = notify;
}

Bool shiftGetTitleFontBold (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontBold].value.b;
}

CompOption * shiftGetTitleFontBoldOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTitleFontBold];
}

void shiftSetTitleFontBoldNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTitleFontBold] = notify;
}

int shiftGetTitleFontSize (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontSize].value.i;
}

CompOption * shiftGetTitleFontSizeOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTitleFontSize];
}

void shiftSetTitleFontSizeNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTitleFontSize] = notify;
}

unsigned short * shiftGetTitleBackColor (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleBackColor].value.c;
}

unsigned short shiftGetTitleBackColorRed (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleBackColor].value.c[0];
}

unsigned short shiftGetTitleBackColorGreen (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleBackColor].value.c[1];
}

unsigned short shiftGetTitleBackColorBlue (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleBackColor].value.c[2];
}

unsigned short shiftGetTitleBackColorAlpha (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleBackColor].value.c[3];
}

CompOption * shiftGetTitleBackColorOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTitleBackColor];
}

void shiftSetTitleBackColorNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTitleBackColor] = notify;
}

unsigned short * shiftGetTitleFontColor (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontColor].value.c;
}

unsigned short shiftGetTitleFontColorRed (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontColor].value.c[0];
}

unsigned short shiftGetTitleFontColorGreen (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontColor].value.c[1];
}

unsigned short shiftGetTitleFontColorBlue (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontColor].value.c[2];
}

unsigned short shiftGetTitleFontColorAlpha (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleFontColor].value.c[3];
}

CompOption * shiftGetTitleFontColorOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTitleFontColor];
}

void shiftSetTitleFontColorNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTitleFontColor] = notify;
}

int shiftGetTitleTextPlacement (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return os->opt[ShiftScreenOptionTitleTextPlacement].value.i;
}

CompOption * shiftGetTitleTextPlacementOption (CompScreen *s)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[ShiftScreenOptionTitleTextPlacement];
}

void shiftSetTitleTextPlacementNotify (CompScreen *s, shiftScreenOptionChangeNotifyProc notify)
{
    SHIFT_OPTIONS_SCREEN(s);
    os->notify[ShiftScreenOptionTitleTextPlacement] = notify;
}

CompOption * shiftGetScreenOption (CompScreen *s, ShiftScreenOptions num)
{
    SHIFT_OPTIONS_SCREEN(s);
    return &os->opt[num];
}

static const CompMetadataOptionInfo shiftOptionsScreenOptionInfo[] = {
    { "speed", "float", "<min>0.1</min><max>10.0</max>", 0, 0 },
    { "shift_speed", "float", "<min>0.1</min><max>5.0</max>", 0, 0 },
    { "timestep", "float", "<min>0.1</min><max>50.0</max>", 0, 0 },
    { "window_match", "match", 0, 0, 0 },
    { "minimized", "bool", 0, 0, 0 },
    { "mouse_speed", "float", "<min>0.1</min><max>50.0</max>", 0, 0 },
    { "click_duration", "int", "<min>10</min><max>2000</max>", 0, 0 },
    { "mode", "int", "<min>0</min><max>1</max>", 0, 0 },
    { "size", "int", "<min>1</min><max>100</max>", 0, 0 },
    { "background_intensity", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "hide_all", "bool", 0, 0, 0 },
    { "reflection", "bool", 0, 0, 0 },
    { "ground_color1", "color", 0, 0, 0 },
    { "ground_color2", "color", 0, 0, 0 },
    { "ground_size", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "intensity", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "flip_rotation", "int", "<min>-80</min><max>80</max>", 0, 0 },
    { "cover_offset", "float", "<min>-0.4</min><max>0.4</max>", 0, 0 },
    { "overlay_icon", "int", "<min>0</min><max>2</max>", 0, 0 },
    { "mipmaps", "bool", 0, 0, 0 },
    { "multioutput_mode", "int", "<min>0</min><max>2</max>", 0, 0 },
    { "window_title", "bool", 0, 0, 0 },
    { "title_font_bold", "bool", 0, 0, 0 },
    { "title_font_size", "int", "<min>6</min><max>96</max>", 0, 0 },
    { "title_back_color", "color", 0, 0, 0 },
    { "title_font_color", "color", 0, 0, 0 },
    { "title_text_placement", "int", "<min>0</min><max>2</max>", 0, 0 },
};

static Bool shiftOptionsSetScreenOption (CompPlugin *plugin, CompScreen *s, char *name, CompOptionValue *value)
{
    SHIFT_OPTIONS_SCREEN(s);
    CompOption *o;
    int        index;

    o = compFindOption (os->opt, ShiftScreenOptionNum, name, &index);

    if (!o)
        return FALSE;

    switch (index)
    {
     case ShiftScreenOptionSpeed:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionSpeed])
                (*os->notify[ShiftScreenOptionSpeed]) (s, o, ShiftScreenOptionSpeed);
            return TRUE;
        }
        break;
     case ShiftScreenOptionShiftSpeed:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionShiftSpeed])
                (*os->notify[ShiftScreenOptionShiftSpeed]) (s, o, ShiftScreenOptionShiftSpeed);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTimestep:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTimestep])
                (*os->notify[ShiftScreenOptionTimestep]) (s, o, ShiftScreenOptionTimestep);
            return TRUE;
        }
        break;
     case ShiftScreenOptionWindowMatch:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionWindowMatch])
                (*os->notify[ShiftScreenOptionWindowMatch]) (s, o, ShiftScreenOptionWindowMatch);
            return TRUE;
        }
        break;
     case ShiftScreenOptionMinimized:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionMinimized])
                (*os->notify[ShiftScreenOptionMinimized]) (s, o, ShiftScreenOptionMinimized);
            return TRUE;
        }
        break;
     case ShiftScreenOptionMouseSpeed:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionMouseSpeed])
                (*os->notify[ShiftScreenOptionMouseSpeed]) (s, o, ShiftScreenOptionMouseSpeed);
            return TRUE;
        }
        break;
     case ShiftScreenOptionClickDuration:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionClickDuration])
                (*os->notify[ShiftScreenOptionClickDuration]) (s, o, ShiftScreenOptionClickDuration);
            return TRUE;
        }
        break;
     case ShiftScreenOptionMode:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionMode])
                (*os->notify[ShiftScreenOptionMode]) (s, o, ShiftScreenOptionMode);
            return TRUE;
        }
        break;
     case ShiftScreenOptionSize:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionSize])
                (*os->notify[ShiftScreenOptionSize]) (s, o, ShiftScreenOptionSize);
            return TRUE;
        }
        break;
     case ShiftScreenOptionBackgroundIntensity:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionBackgroundIntensity])
                (*os->notify[ShiftScreenOptionBackgroundIntensity]) (s, o, ShiftScreenOptionBackgroundIntensity);
            return TRUE;
        }
        break;
     case ShiftScreenOptionHideAll:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionHideAll])
                (*os->notify[ShiftScreenOptionHideAll]) (s, o, ShiftScreenOptionHideAll);
            return TRUE;
        }
        break;
     case ShiftScreenOptionReflection:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionReflection])
                (*os->notify[ShiftScreenOptionReflection]) (s, o, ShiftScreenOptionReflection);
            return TRUE;
        }
        break;
     case ShiftScreenOptionGroundColor1:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionGroundColor1])
                (*os->notify[ShiftScreenOptionGroundColor1]) (s, o, ShiftScreenOptionGroundColor1);
            return TRUE;
        }
        break;
     case ShiftScreenOptionGroundColor2:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionGroundColor2])
                (*os->notify[ShiftScreenOptionGroundColor2]) (s, o, ShiftScreenOptionGroundColor2);
            return TRUE;
        }
        break;
     case ShiftScreenOptionGroundSize:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionGroundSize])
                (*os->notify[ShiftScreenOptionGroundSize]) (s, o, ShiftScreenOptionGroundSize);
            return TRUE;
        }
        break;
     case ShiftScreenOptionIntensity:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionIntensity])
                (*os->notify[ShiftScreenOptionIntensity]) (s, o, ShiftScreenOptionIntensity);
            return TRUE;
        }
        break;
     case ShiftScreenOptionFlipRotation:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionFlipRotation])
                (*os->notify[ShiftScreenOptionFlipRotation]) (s, o, ShiftScreenOptionFlipRotation);
            return TRUE;
        }
        break;
     case ShiftScreenOptionCoverOffset:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionCoverOffset])
                (*os->notify[ShiftScreenOptionCoverOffset]) (s, o, ShiftScreenOptionCoverOffset);
            return TRUE;
        }
        break;
     case ShiftScreenOptionOverlayIcon:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionOverlayIcon])
                (*os->notify[ShiftScreenOptionOverlayIcon]) (s, o, ShiftScreenOptionOverlayIcon);
            return TRUE;
        }
        break;
     case ShiftScreenOptionMipmaps:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionMipmaps])
                (*os->notify[ShiftScreenOptionMipmaps]) (s, o, ShiftScreenOptionMipmaps);
            return TRUE;
        }
        break;
     case ShiftScreenOptionMultioutputMode:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionMultioutputMode])
                (*os->notify[ShiftScreenOptionMultioutputMode]) (s, o, ShiftScreenOptionMultioutputMode);
            return TRUE;
        }
        break;
     case ShiftScreenOptionWindowTitle:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionWindowTitle])
                (*os->notify[ShiftScreenOptionWindowTitle]) (s, o, ShiftScreenOptionWindowTitle);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTitleFontBold:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTitleFontBold])
                (*os->notify[ShiftScreenOptionTitleFontBold]) (s, o, ShiftScreenOptionTitleFontBold);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTitleFontSize:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTitleFontSize])
                (*os->notify[ShiftScreenOptionTitleFontSize]) (s, o, ShiftScreenOptionTitleFontSize);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTitleBackColor:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTitleBackColor])
                (*os->notify[ShiftScreenOptionTitleBackColor]) (s, o, ShiftScreenOptionTitleBackColor);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTitleFontColor:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTitleFontColor])
                (*os->notify[ShiftScreenOptionTitleFontColor]) (s, o, ShiftScreenOptionTitleFontColor);
            return TRUE;
        }
        break;
     case ShiftScreenOptionTitleTextPlacement:
        if (compSetScreenOption (s, o, value))
        {
            if (os->notify[ShiftScreenOptionTitleTextPlacement])
                (*os->notify[ShiftScreenOptionTitleTextPlacement]) (s, o, ShiftScreenOptionTitleTextPlacement);
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static CompOption * shiftOptionsGetScreenOptions (CompPlugin *plugin, CompScreen *s, int *count)
{
    SHIFT_OPTIONS_SCREEN(s);
    *count = ShiftScreenOptionNum;
    return os->opt;
}

static Bool shiftOptionsInitScreen (CompPlugin *p, CompScreen *s)
{
    ShiftOptionsScreen *os;
    
    SHIFT_OPTIONS_DISPLAY (s->display);

    os = calloc (1, sizeof(ShiftOptionsScreen));
    if (!os)
        return FALSE;

    s->privates[od->screenPrivateIndex].ptr = os;

    if (!compInitScreenOptionsFromMetadata (s, &shiftOptionsMetadata, shiftOptionsScreenOptionInfo, os->opt, ShiftScreenOptionNum))
    {
        free (os);
        return FALSE;
    }
    if (shiftPluginVTable && shiftPluginVTable->initScreen)
        return shiftPluginVTable->initScreen (p, s);
    return TRUE;
}

static void shiftOptionsFiniScreen (CompPlugin *p, CompScreen *s)
{
    if (shiftPluginVTable && shiftPluginVTable->finiScreen)
        return shiftPluginVTable->finiScreen (p, s);

    SHIFT_OPTIONS_SCREEN (s);


    compFiniScreenOptions (s, os->opt, ShiftScreenOptionNum);

    free (os);
}

static Bool shiftOptionsInitDisplay (CompPlugin *p, CompDisplay *d)
{
    ShiftOptionsDisplay *od;
   
    
    od = calloc (1, sizeof(ShiftOptionsDisplay));
    if (!od)
        return FALSE;

    od->screenPrivateIndex = allocateScreenPrivateIndex(d);
    if (od->screenPrivateIndex < 0)
    {
        free(od);
        return FALSE;
    }

    d->privates[displayPrivateIndex].ptr = od;

        if (shiftPluginVTable && shiftPluginVTable->initDisplay)
        return shiftPluginVTable->initDisplay (p, d);
    return TRUE;
}

static void shiftOptionsFiniDisplay (CompPlugin *p, CompDisplay *d)
{
    if (shiftPluginVTable && shiftPluginVTable->finiDisplay)
        return shiftPluginVTable->finiDisplay (p, d);

    SHIFT_OPTIONS_DISPLAY (d);

    freeScreenPrivateIndex(d, od->screenPrivateIndex);
    free (od);
}

static Bool shiftOptionsInit (CompPlugin *p)
{
    displayPrivateIndex = allocateDisplayPrivateIndex();
    if (displayPrivateIndex < 0)
        return FALSE;

    if (!compInitPluginMetadataFromInfo (&shiftOptionsMetadata, "shift",0, 0, shiftOptionsScreenOptionInfo, ShiftScreenOptionNum))
        return FALSE;

    compAddMetadataFromFile (&shiftOptionsMetadata, "shift");
    if (shiftPluginVTable && shiftPluginVTable->init)
        return shiftPluginVTable->init (p);
    return TRUE;
}

static void shiftOptionsFini (CompPlugin *p)
{
    if (shiftPluginVTable && shiftPluginVTable->fini)
        return shiftPluginVTable->fini (p);

    if (displayPrivateIndex >= 0)
        freeDisplayPrivateIndex(displayPrivateIndex);

    compFiniMetadata (&shiftOptionsMetadata);
}

static CompMetadata *
shiftOptionsGetMetadata (CompPlugin *plugin)
{
    return &shiftOptionsMetadata;
}

CompPluginVTable *getCompPluginInfo (void)
{
    if (!shiftPluginVTable)
    {
        shiftPluginVTable = shiftOptionsGetCompPluginInfo ();
        memcpy(&shiftOptionsVTable, shiftPluginVTable, sizeof(CompPluginVTable));
        shiftOptionsVTable.getMetadata = shiftOptionsGetMetadata;
        shiftOptionsVTable.init = shiftOptionsInit;
        shiftOptionsVTable.fini = shiftOptionsFini;
        shiftOptionsVTable.initDisplay = shiftOptionsInitDisplay;
        shiftOptionsVTable.finiDisplay = shiftOptionsFiniDisplay;
        shiftOptionsVTable.initScreen = shiftOptionsInitScreen;
        shiftOptionsVTable.finiScreen = shiftOptionsFiniScreen;
        shiftOptionsVTable.getScreenOptions = shiftOptionsGetScreenOptions;
	shiftOptionsVTable.setScreenOption = shiftOptionsSetScreenOption;
	
    }
    return &shiftOptionsVTable;
}
