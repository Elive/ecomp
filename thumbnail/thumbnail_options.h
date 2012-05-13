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

#ifndef _THUMBNAIL_OPTIONS_H
#define _THUMBNAIL_OPTIONS_H

#ifndef _THUMBNAIL_OPTIONS_INTERNAL
#define getCompPluginInfo thumbnailOptionsGetCompPluginInfo
#endif

#ifdef  __cplusplus
extern "C" {
#endif

CompPluginVTable *thumbnailOptionsGetCompPluginInfo (void);

typedef enum
{
    ThumbnailDisplayOptionNum
} ThumbnailDisplayOptions;

typedef void (*thumbnailDisplayOptionChangeNotifyProc) (CompDisplay *display, CompOption *opt, ThumbnailDisplayOptions num);

CompOption *thumbnailGetDisplayOption (CompDisplay *d, ThumbnailDisplayOptions num);

typedef enum
{
    ThumbnailScreenOptionThumbSize,
    ThumbnailScreenOptionShowDelay,
    ThumbnailScreenOptionBorder,
    ThumbnailScreenOptionThumbColor,
    ThumbnailScreenOptionFadeSpeed,
    ThumbnailScreenOptionCurrentViewport,
    ThumbnailScreenOptionAlwaysOnTop,
    ThumbnailScreenOptionWindowLike,
    ThumbnailScreenOptionMipmap,
    ThumbnailScreenOptionTitleEnabled,
    ThumbnailScreenOptionFontBold,
    ThumbnailScreenOptionFontSize,
    ThumbnailScreenOptionFontColor,
    ThumbnailScreenOptionNum
} ThumbnailScreenOptions;

typedef void (*thumbnailScreenOptionChangeNotifyProc) (CompScreen *screen, CompOption *opt, ThumbnailScreenOptions num);

CompOption *thumbnailGetScreenOption (CompScreen *s, ThumbnailScreenOptions num);

int              thumbnailGetThumbSize (CompScreen *s);
CompOption *     thumbnailGetThumbSizeOption (CompScreen *s);
void             thumbnailSetThumbSizeNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
int              thumbnailGetShowDelay (CompScreen *s);
CompOption *     thumbnailGetShowDelayOption (CompScreen *s);
void             thumbnailSetShowDelayNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
int              thumbnailGetBorder (CompScreen *s);
CompOption *     thumbnailGetBorderOption (CompScreen *s);
void             thumbnailSetBorderNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
unsigned short * thumbnailGetThumbColor (CompScreen *s);
unsigned short   thumbnailGetThumbColorRed (CompScreen *s);
unsigned short   thumbnailGetThumbColorGreen (CompScreen *s);
unsigned short   thumbnailGetThumbColorBlue (CompScreen *s);
unsigned short   thumbnailGetThumbColorAlpha (CompScreen *s);
CompOption *     thumbnailGetThumbColorOption (CompScreen *s);
void             thumbnailSetThumbColorNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
float            thumbnailGetFadeSpeed (CompScreen *s);
CompOption *     thumbnailGetFadeSpeedOption (CompScreen *s);
void             thumbnailSetFadeSpeedNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetCurrentViewport (CompScreen *s);
CompOption *     thumbnailGetCurrentViewportOption (CompScreen *s);
void             thumbnailSetCurrentViewportNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetAlwaysOnTop (CompScreen *s);
CompOption *     thumbnailGetAlwaysOnTopOption (CompScreen *s);
void             thumbnailSetAlwaysOnTopNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetWindowLike (CompScreen *s);
CompOption *     thumbnailGetWindowLikeOption (CompScreen *s);
void             thumbnailSetWindowLikeNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetMipmap (CompScreen *s);
CompOption *     thumbnailGetMipmapOption (CompScreen *s);
void             thumbnailSetMipmapNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetTitleEnabled (CompScreen *s);
CompOption *     thumbnailGetTitleEnabledOption (CompScreen *s);
void             thumbnailSetTitleEnabledNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
Bool             thumbnailGetFontBold (CompScreen *s);
CompOption *     thumbnailGetFontBoldOption (CompScreen *s);
void             thumbnailSetFontBoldNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
int              thumbnailGetFontSize (CompScreen *s);
CompOption *     thumbnailGetFontSizeOption (CompScreen *s);
void             thumbnailSetFontSizeNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
unsigned short * thumbnailGetFontColor (CompScreen *s);
unsigned short   thumbnailGetFontColorRed (CompScreen *s);
unsigned short   thumbnailGetFontColorGreen (CompScreen *s);
unsigned short   thumbnailGetFontColorBlue (CompScreen *s);
unsigned short   thumbnailGetFontColorAlpha (CompScreen *s);
CompOption *     thumbnailGetFontColorOption (CompScreen *s);
void             thumbnailSetFontColorNotify (CompScreen *s, thumbnailScreenOptionChangeNotifyProc notify);
            
#ifdef  __cplusplus
}
#endif

#endif