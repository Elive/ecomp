libfade_la_LDFLAGS = -module -avoid-version -no-undefined
libfade_la_SOURCES = fade.c

#libshadow_la_LDFLAGS = -module -avoid-version -no-undefined
#libshadow_la_SOURCES = shadow.c

libcube_la_LDFLAGS = -module -avoid-version -no-undefined
libcube_la_SOURCES = cube.c

librotate_la_LDFLAGS = -module -avoid-version -no-undefined
librotate_la_SOURCES = rotate.c

libzoom_la_LDFLAGS = -module -avoid-version -no-undefined
libzoom_la_SOURCES = zoom.c

libscale_la_LDFLAGS = -module -avoid-version -no-undefined
libscale_la_SOURCES = scale.c

libwobbly_la_LDFLAGS = -module -avoid-version -no-undefined
libwobbly_la_SOURCES = wobbly.c

libminimize_la_LDFLAGS = -module -avoid-version -no-undefined
libminimize_la_SOURCES = minimize.c

libmove_la_LDFLAGS = -module -avoid-version -no-undefined
libmove_la_SOURCES = move.c

# libresize_la_LDFLAGS = -module -avoid-version -no-undefined
# libresize_la_SOURCES = resize.c

libdecoration_la_DEPENDENCIES = $(top_builddir)/libdecoration/libdecoration.la
libdecoration_la_LDFLAGS = -module -avoid-version -no-undefined
libdecoration_la_LIBADD = $(top_builddir)/libdecoration/libdecoration.la
libdecoration_la_SOURCES = decoration.c

libswitcher_la_LDFLAGS = -module -avoid-version -no-undefined
libswitcher_la_SOURCES = switcher.c

libwater_la_LDFLAGS = -module -avoid-version -no-undefined
libwater_la_SOURCES = water.c

# libscreenshot_la_LDFLAGS = -module -avoid-version -no-undefined
# libscreenshot_la_SOURCES = screenshot.c

libplane_la_LDFLAGS = -module -avoid-version -no-undefined
libplane_la_SOURCES = plane.c

# libclone_la_LDFLAGS = -module -avoid-version -no-undefined
# libclone_la_SOURCES = clone.c

libpng_la_LDFLAGS = -module -avoid-version -no-undefined
libpng_la_LIBADD = @LIBPNG_LIBS@
libpng_la_SOURCES = png.c

libblur_la_DEPENDENCIES = $(top_builddir)/libdecoration/libdecoration.la
libblur_la_LDFLAGS = -module -avoid-version -no-undefined
libblur_la_LIBADD = $(top_builddir)/libdecoration/libdecoration.la -lGLU
libblur_la_SOURCES = blur.c

libregex_la_LDFLAGS = -module -avoid-version -no-undefined
libregex_la_SOURCES = regex.c

libvideo_la_DEPENDENCIES = $(top_builddir)/libdecoration/libdecoration.la
libvideo_la_LDFLAGS = -module -avoid-version -no-undefined
libvideo_la_LIBADD = $(top_builddir)/libdecoration/libdecoration.la
libvideo_la_SOURCES = video.c

# libini_la_LDFLAGS = -module -avoid-version -no-undefined
# libini_la_SOURCES = ini.c

if USE_LIBRSVG
libsvg_la_DEPENDENCIES = $(top_builddir)/libdecoration/libdecoration.la
libsvg_la_LDFLAGS = -module -avoid-version -no-undefined
libsvg_la_LIBADD =				       \
	$(top_builddir)/libdecoration/libdecoration.la \
	@LIBRSVG_LIBS@
libsvg_la_SOURCES = svg.c
libsvg_module = libsvg.la
endif

# if ANNOTATE_PLUGIN
# libannotate_la_LDFLAGS = -module -avoid-version -no-undefined
# libannotate_la_LIBADD = @ANNOTATE_LIBS@
# libannotate_la_SOURCES = annotate.c
# libannotate_module = libannotate.la
# endif

if DBUS_PLUGIN
libdbus_la_LDFLAGS = -module -avoid-version -no-undefined
libdbus_la_LIBADD = @DBUS_LIBS@
libdbus_la_SOURCES = dbus.c
libdbus_module = libdbus.la
endif

# if INOTIFY_PLUGIN
# libinotify_la_LDFLAGS = -module -avoid-version -no-undefined
# libinotify_la_SOURCES = inotify.c
# libinotify_module = libinotify.la
# endif

# if FUSE_PLUGIN
# libfs_la_LDFLAGS = -module -avoid-version -no-undefined
# libfs_la_LIBADD = @FUSE_LIBS@
# libfs_la_SOURCES = fuse.c
# libfs_module = libfs.la
# endif

INCLUDES =					\
	@ECOMP_CFLAGS@				\
	@LIBPNG_CFLAGS@				\
	@LIBRSVG_CFLAGS@			\
	@ANNOTATE_CFLAGS@			\
	@DBUS_CFLAGS@				\
	@FUSE_CFLAGS@				\
	-DFUSE_USE_VERSION=26			\
	-DALL_LINGUAS="\"@ALL_LINGUAS@\""	\
	-DLOCALEDIR="\"@datadir@/locale\""	\
	-DPLUGINDIR=\"$(plugindir)\"		\
	-DIMAGEDIR=\"$(imagedir)\"		\
	-I$(top_srcdir)/include                 \
	-DMETADATADIR=\"$(metadatadir)\"

moduledir = $(plugindir)

module_LTLIBRARIES =		\
	libfade.la		\
	libvideo.la		\
	$(libdbus_module)	\
	libdecoration.la	\
	libscale.la             \
	libblur.la		\
	libminimize.la		\
	libregex.la		\
	libplane.la		\
	libcube.la		\
	librotate.la 		\
	libswitcher.la     	\
	$(libsvg_module)	\
	libwater.la		\
	libpng.la		\
	libmove.la		\
	libwobbly.la		\
	libzoom.la
