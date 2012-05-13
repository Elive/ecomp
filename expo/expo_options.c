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

#define _EXPO_OPTIONS_INTERNAL
#include "expo_options.h"

static int displayPrivateIndex;

static CompMetadata expoOptionsMetadata;

static CompPluginVTable *expoPluginVTable = NULL;
CompPluginVTable expoOptionsVTable;

#define GET_EXPO_OPTIONS_DISPLAY(d) \
        ((ExpoOptionsDisplay *) (d)->privates[displayPrivateIndex].ptr)

#define EXPO_OPTIONS_DISPLAY(d) \
        ExpoOptionsDisplay *od = GET_EXPO_OPTIONS_DISPLAY (d)

#define GET_EXPO_OPTIONS_SCREEN(s, od) \
        ((ExpoOptionsScreen *) (s)->privates[(od)->screenPrivateIndex].ptr)

#define EXPO_OPTIONS_SCREEN(s) \
        ExpoOptionsScreen *os = GET_EXPO_OPTIONS_SCREEN (s, GET_EXPO_OPTIONS_DISPLAY (s->display))

typedef struct _ExpoOptionsDisplay
{
    int screenPrivateIndex;

    CompOption opt[ExpoDisplayOptionNum];
    expoDisplayOptionChangeNotifyProc notify[ExpoDisplayOptionNum];
} ExpoOptionsDisplay;

typedef struct _ExpoOptionsScreen
{
} ExpoOptionsScreen;

CompAction * expoGetExpo (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionExpo].value.action;
}

void expoSetExpoInitiate (CompDisplay *d, CompActionCallBackProc init)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionExpo].value.action.initiate = init;
}

void expoSetExpoTerminate (CompDisplay *d, CompActionCallBackProc term)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionExpo].value.action.terminate = term;
}

CompOption * expoGetExpoOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionExpo];
}

void expoSetExpoNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionExpo] = notify;
}

CompAction * expoGetNext (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionNext].value.action;
}

void expoSetNextInitiate (CompDisplay *d, CompActionCallBackProc init)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionNext].value.action.initiate = init;
}

void expoSetNextTerminate (CompDisplay *d, CompActionCallBackProc term)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionNext].value.action.terminate = term;
}

CompOption * expoGetNextOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionNext];
}

void expoSetNextNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionNext] = notify;
}

CompAction * expoGetPrev (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionPrev].value.action;
}

void expoSetPrevInitiate (CompDisplay *d, CompActionCallBackProc init)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionPrev].value.action.initiate = init;
}

void expoSetPrevTerminate (CompDisplay *d, CompActionCallBackProc term)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->opt[ExpoDisplayOptionPrev].value.action.terminate = term;
}

CompOption * expoGetPrevOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionPrev];
}

void expoSetPrevNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionPrev] = notify;
}

int expoGetDoubleClickTime (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionDoubleClickTime].value.i;
}

CompOption * expoGetDoubleClickTimeOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionDoubleClickTime];
}

void expoSetDoubleClickTimeNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionDoubleClickTime] = notify;
}

float expoGetZoomTime (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionZoomTime].value.f;
}

CompOption * expoGetZoomTimeOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionZoomTime];
}

void expoSetZoomTimeNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionZoomTime] = notify;
}

Bool expoGetRotate (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionRotate].value.b;
}

CompOption * expoGetRotateOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionRotate];
}

void expoSetRotateNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionRotate] = notify;
}

Bool expoGetExpoImmediateMove (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionExpoImmediateMove].value.b;
}

CompOption * expoGetExpoImmediateMoveOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionExpoImmediateMove];
}

void expoSetExpoImmediateMoveNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionExpoImmediateMove] = notify;
}

int expoGetExpoAnimation (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionExpoAnimation].value.i;
}

CompOption * expoGetExpoAnimationOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionExpoAnimation];
}

void expoSetExpoAnimationNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionExpoAnimation] = notify;
}

Bool expoGetReflection (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionReflection].value.b;
}

CompOption * expoGetReflectionOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionReflection];
}

void expoSetReflectionNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionReflection] = notify;
}

unsigned short * expoGetGroundColor1 (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor1].value.c;
}

unsigned short expoGetGroundColor1Red (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor1].value.c[0];
}

unsigned short expoGetGroundColor1Green (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor1].value.c[1];
}

unsigned short expoGetGroundColor1Blue (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor1].value.c[2];
}

unsigned short expoGetGroundColor1Alpha (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor1].value.c[3];
}

CompOption * expoGetGroundColor1Option (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionGroundColor1];
}

void expoSetGroundColor1Notify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionGroundColor1] = notify;
}

unsigned short * expoGetGroundColor2 (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor2].value.c;
}

unsigned short expoGetGroundColor2Red (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor2].value.c[0];
}

unsigned short expoGetGroundColor2Green (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor2].value.c[1];
}

unsigned short expoGetGroundColor2Blue (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor2].value.c[2];
}

unsigned short expoGetGroundColor2Alpha (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundColor2].value.c[3];
}

CompOption * expoGetGroundColor2Option (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionGroundColor2];
}

void expoSetGroundColor2Notify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionGroundColor2] = notify;
}

float expoGetGroundSize (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionGroundSize].value.f;
}

CompOption * expoGetGroundSizeOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionGroundSize];
}

void expoSetGroundSizeNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionGroundSize] = notify;
}

float expoGetDistance (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionDistance].value.f;
}

CompOption * expoGetDistanceOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionDistance];
}

void expoSetDistanceNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionDistance] = notify;
}

float expoGetScaleFactor (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionScaleFactor].value.f;
}

CompOption * expoGetScaleFactorOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionScaleFactor];
}

void expoSetScaleFactorNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionScaleFactor] = notify;
}

float expoGetAspectRatio (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionAspectRatio].value.f;
}

CompOption * expoGetAspectRatioOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionAspectRatio];
}

void expoSetAspectRatioNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionAspectRatio] = notify;
}

Bool expoGetMipmaps (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionMipmaps].value.b;
}

CompOption * expoGetMipmapsOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionMipmaps];
}

void expoSetMipmapsNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionMipmaps] = notify;
}

int expoGetMultioutputMode (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionMultioutputMode].value.i;
}

CompOption * expoGetMultioutputModeOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionMultioutputMode];
}

void expoSetMultioutputModeNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionMultioutputMode] = notify;
}

Bool expoGetHideDocks (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return od->opt[ExpoDisplayOptionHideDocks].value.b;
}

CompOption * expoGetHideDocksOption (CompDisplay *d)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[ExpoDisplayOptionHideDocks];
}

void expoSetHideDocksNotify (CompDisplay *d, expoDisplayOptionChangeNotifyProc notify)
{
    EXPO_OPTIONS_DISPLAY(d);
    od->notify[ExpoDisplayOptionHideDocks] = notify;
}

CompOption * expoGetDisplayOption (CompDisplay *d, ExpoDisplayOptions num)
{
    EXPO_OPTIONS_DISPLAY(d);
    return &od->opt[num];
}

static const CompMetadataOptionInfo expoOptionsDisplayOptionInfo[] = {
    { "expo", "action", 0, 0, 0 },
    { "next", "action", 0, 0, 0 },
    { "prev", "action", 0, 0, 0 },
    { "double_click_time", "int", "<min>50</min><max>2000</max>", 0, 0 },
    { "zoom_time", "float", "<min>0.1</min><max>5.0</max>", 0, 0 },
    { "rotate", "bool", 0, 0, 0 },
    { "expo_immediate_move", "bool", 0, 0, 0 },
    { "expo_animation", "int", "<min>0</min><max>2</max>", 0, 0 },
    { "reflection", "bool", 0, 0, 0 },
    { "ground_color1", "color", 0, 0, 0 },
    { "ground_color2", "color", 0, 0, 0 },
    { "ground_size", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "distance", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "scale_factor", "float", "<min>0.0</min><max>2.0</max>", 0, 0 },
    { "aspect_ratio", "float", "<min>0.0</min><max>1.0</max>", 0, 0 },
    { "mipmaps", "bool", 0, 0, 0 },
    { "multioutput_mode", "int", "<min>0</min><max>1</max>", 0, 0 },
    { "hide_docks", "bool", 0, 0, 0 },
};

static Bool expoOptionsSetDisplayOption (CompPlugin *plugin, CompDisplay *d, char *name, CompOptionValue *value)
{
    EXPO_OPTIONS_DISPLAY(d);
    CompOption *o;
    int        index;

    o = compFindOption (od->opt, ExpoDisplayOptionNum, name, &index);

    if (!o)
        return FALSE;

    switch (index)
    {
     case ExpoDisplayOptionExpo:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionExpo])
                (*od->notify[ExpoDisplayOptionExpo]) (d, o, ExpoDisplayOptionExpo);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionNext:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionNext])
                (*od->notify[ExpoDisplayOptionNext]) (d, o, ExpoDisplayOptionNext);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionPrev:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionPrev])
                (*od->notify[ExpoDisplayOptionPrev]) (d, o, ExpoDisplayOptionPrev);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionDoubleClickTime:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionDoubleClickTime])
                (*od->notify[ExpoDisplayOptionDoubleClickTime]) (d, o, ExpoDisplayOptionDoubleClickTime);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionZoomTime:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionZoomTime])
                (*od->notify[ExpoDisplayOptionZoomTime]) (d, o, ExpoDisplayOptionZoomTime);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionRotate:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionRotate])
                (*od->notify[ExpoDisplayOptionRotate]) (d, o, ExpoDisplayOptionRotate);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionExpoImmediateMove:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionExpoImmediateMove])
                (*od->notify[ExpoDisplayOptionExpoImmediateMove]) (d, o, ExpoDisplayOptionExpoImmediateMove);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionExpoAnimation:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionExpoAnimation])
                (*od->notify[ExpoDisplayOptionExpoAnimation]) (d, o, ExpoDisplayOptionExpoAnimation);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionReflection:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionReflection])
                (*od->notify[ExpoDisplayOptionReflection]) (d, o, ExpoDisplayOptionReflection);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionGroundColor1:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionGroundColor1])
                (*od->notify[ExpoDisplayOptionGroundColor1]) (d, o, ExpoDisplayOptionGroundColor1);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionGroundColor2:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionGroundColor2])
                (*od->notify[ExpoDisplayOptionGroundColor2]) (d, o, ExpoDisplayOptionGroundColor2);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionGroundSize:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionGroundSize])
                (*od->notify[ExpoDisplayOptionGroundSize]) (d, o, ExpoDisplayOptionGroundSize);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionDistance:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionDistance])
                (*od->notify[ExpoDisplayOptionDistance]) (d, o, ExpoDisplayOptionDistance);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionScaleFactor:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionScaleFactor])
                (*od->notify[ExpoDisplayOptionScaleFactor]) (d, o, ExpoDisplayOptionScaleFactor);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionAspectRatio:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionAspectRatio])
                (*od->notify[ExpoDisplayOptionAspectRatio]) (d, o, ExpoDisplayOptionAspectRatio);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionMipmaps:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionMipmaps])
                (*od->notify[ExpoDisplayOptionMipmaps]) (d, o, ExpoDisplayOptionMipmaps);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionMultioutputMode:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionMultioutputMode])
                (*od->notify[ExpoDisplayOptionMultioutputMode]) (d, o, ExpoDisplayOptionMultioutputMode);
            return TRUE;
        }
        break;
     case ExpoDisplayOptionHideDocks:
        if (compSetDisplayOption (d, o, value))
        {
            if (od->notify[ExpoDisplayOptionHideDocks])
                (*od->notify[ExpoDisplayOptionHideDocks]) (d, o, ExpoDisplayOptionHideDocks);
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static CompOption * expoOptionsGetDisplayOptions (CompPlugin *plugin, CompDisplay *d, int *count)
{
    EXPO_OPTIONS_DISPLAY(d);
    *count = ExpoDisplayOptionNum;
    return od->opt;
}

static Bool expoOptionsInitScreen (CompPlugin *p, CompScreen *s)
{
    ExpoOptionsScreen *os;
    
    EXPO_OPTIONS_DISPLAY (s->display);

    os = calloc (1, sizeof(ExpoOptionsScreen));
    if (!os)
        return FALSE;

    s->privates[od->screenPrivateIndex].ptr = os;

        if (expoPluginVTable && expoPluginVTable->initScreen)
        return expoPluginVTable->initScreen (p, s);
    return TRUE;
}

static void expoOptionsFiniScreen (CompPlugin *p, CompScreen *s)
{
    if (expoPluginVTable && expoPluginVTable->finiScreen)
        return expoPluginVTable->finiScreen (p, s);

    EXPO_OPTIONS_SCREEN (s);

    free (os);
}

static Bool expoOptionsInitDisplay (CompPlugin *p, CompDisplay *d)
{
    ExpoOptionsDisplay *od;
   
    
    od = calloc (1, sizeof(ExpoOptionsDisplay));
    if (!od)
        return FALSE;

    od->screenPrivateIndex = allocateScreenPrivateIndex(d);
    if (od->screenPrivateIndex < 0)
    {
        free(od);
        return FALSE;
    }

    d->privates[displayPrivateIndex].ptr = od;

    if (!compInitDisplayOptionsFromMetadata (d, &expoOptionsMetadata, expoOptionsDisplayOptionInfo, od->opt, ExpoDisplayOptionNum))
    {
        free (od);
        return FALSE;
    }
    if (expoPluginVTable && expoPluginVTable->initDisplay)
        return expoPluginVTable->initDisplay (p, d);
    return TRUE;
}

static void expoOptionsFiniDisplay (CompPlugin *p, CompDisplay *d)
{
    if (expoPluginVTable && expoPluginVTable->finiDisplay)
        return expoPluginVTable->finiDisplay (p, d);

    EXPO_OPTIONS_DISPLAY (d);

    freeScreenPrivateIndex(d, od->screenPrivateIndex);

    compFiniDisplayOptions (d, od->opt, ExpoDisplayOptionNum);

    free (od);
}

static Bool expoOptionsInit (CompPlugin *p)
{
    displayPrivateIndex = allocateDisplayPrivateIndex();
    if (displayPrivateIndex < 0)
        return FALSE;

    if (!compInitPluginMetadataFromInfo (&expoOptionsMetadata, "expo",expoOptionsDisplayOptionInfo, ExpoDisplayOptionNum, 0, 0))
        return FALSE;

    compAddMetadataFromFile (&expoOptionsMetadata, "expo");
    if (expoPluginVTable && expoPluginVTable->init)
        return expoPluginVTable->init (p);
    return TRUE;
}

static void expoOptionsFini (CompPlugin *p)
{
    if (expoPluginVTable && expoPluginVTable->fini)
        return expoPluginVTable->fini (p);

    if (displayPrivateIndex >= 0)
        freeDisplayPrivateIndex(displayPrivateIndex);

    compFiniMetadata (&expoOptionsMetadata);
}

static CompMetadata *
expoOptionsGetMetadata (CompPlugin *plugin)
{
    return &expoOptionsMetadata;
}

CompPluginVTable *getCompPluginInfo (void)
{
    if (!expoPluginVTable)
    {
        expoPluginVTable = expoOptionsGetCompPluginInfo ();
        memcpy(&expoOptionsVTable, expoPluginVTable, sizeof(CompPluginVTable));
        expoOptionsVTable.getMetadata = expoOptionsGetMetadata;
        expoOptionsVTable.init = expoOptionsInit;
        expoOptionsVTable.fini = expoOptionsFini;
        expoOptionsVTable.initDisplay = expoOptionsInitDisplay;
        expoOptionsVTable.finiDisplay = expoOptionsFiniDisplay;
        expoOptionsVTable.initScreen = expoOptionsInitScreen;
        expoOptionsVTable.finiScreen = expoOptionsFiniScreen;
        expoOptionsVTable.getDisplayOptions = expoOptionsGetDisplayOptions;
	expoOptionsVTable.setDisplayOption = expoOptionsSetDisplayOption;
	
    }
    return &expoOptionsVTable;
}
