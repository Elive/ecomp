MAINTAINERCLEANFILES = Makefile.in

EET_EET = @eet_eet@

EXTRA_DIST = \
 animation-allscreens.src \
 blur-allscreens.src \
 core-allscreens.src \
 cube-allscreens.src \
 decoration-allscreens.src \
 expo-allscreens.src \
 mblur-allscreens.src \
 move-allscreens.src \
 ring-allscreens.src \
 rotate-allscreens.src \
 scale-allscreens.src \
 switcher-allscreens.src \
 wall-allscreens.src \
 wobbly-allscreens.src \
 animation-screen0.src \
 blur-screen0.src \
 core-screen0.src \
 cube-screen0.src \
 decoration-screen0.src \
 expo-screen0.src \
 mblur-screen0.src \
 move-screen0.src \
 ring-screen0.src \
 rotate-screen0.src \
 scaleaddon-screen0.src \
 scale-screen0.src \
 shift-screen0.src \
 switcher-screen0.src \
 wall-screen0.src \
 wobbly-screen0.src

filesdir = $(imagedir)
files_DATA = ecomp.eet

SUFFIXES = .eet

ecomp.eet: 
	$(EET_EET) -e $(top_builddir)/data/$@ animation-allscreens 	animation-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ blur-allscreens 	mblur-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ core-allscreens 	core-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ cube-allscreens 	cube-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ decoration-allscreens decoration-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ expo-allscreens 	expo-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ mblur-allscreens 	mblur-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ move-allscreens 	move-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ ring-allscreens 	ring-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ rotate-allscreens	rotate-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ scale-allscreens 	scale-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ switcher-allscreens 	switcher-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ wall-allscreens 	wall-allscreens.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ wobbly-allscreens	wobbly-allscreens.src 0  

	$(EET_EET) -e $(top_builddir)/data/$@ animation-screen0	animation-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ blur-screen0 		blur-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ core-screen0 		core-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ cube-screen0 		cube-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ decoration-screen0 	decoration-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ expo-screen0 		expo-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ mblur-screen0 	mblur-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ move-screen0 		move-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ ring-screen0 		ring-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ rotate-screen0 	rotate-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ scaleaddon-screen0 	scaleaddon-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ scale-screen0 	scale-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ shift-screen0 	shift-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ switcher-screen0 	switcher-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ wall-screen0 		wall-screen0.src 0  
	$(EET_EET) -e $(top_builddir)/data/$@ wobbly-screen0 	wobbly-screen0.src 0  

clean-local:
	rm -rf ecomp.eet *~

