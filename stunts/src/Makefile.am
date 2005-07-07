# Add here all programs that should be built
# Process this file with automake to produce Makefile.in

# Add here all programs that should be built and installed in $PREFIX/bin
bin_PROGRAMS = stunts

# Add here all .cpp source files that are needed to build myapp
stunts_SOURCES = \
	FrameListener.cpp \
	GameApplication.cpp \
	stunts.cpp \
	UserInput.cpp \
	OgreTask.cpp \
	Level.cpp \
	Terrain.cpp \
	BaseObject.cpp \
	InteractiveObject.cpp\
	CarObject.cpp\
	Wheel.cpp\
	Engine.cpp\
	GearBox.cpp\
	BaseControl.cpp\
	AICarControl.cpp\
	HumanControl.cpp\
	Atmosphere.cpp\
	Utils.cpp\
	CGuiTask.cpp\
	CGuiAddInt.cpp\
	CGuiButton.cpp\
	CGuiExitGui.cpp\
	CGuiMulti.cpp\
	CGuiPage.cpp\
	CGuiParser.cpp\
	CGuiStartGame.cpp\
	CGuiTex.cpp \
	Waypoint.cpp \
	FileIO.cpp\
	CFunction.cpp \
	CKI.cpp \
	CNeuralNetwork.cpp \
	CNeurode.cpp \
	LevelManager.cpp \
	tinyxml.cpp\
	tinystr.cpp\
	tinyxmlerror.cpp\
	tinyxmlparser.cpp

#	ObjectInstantiator.cpp
	
# Add here all headers that are needed to build myapp
# These headers will not be installed, but included in the dist
noinst_HEADERS = \
	Level.hpp \
	Terrain.hpp \
	FrameListener.hpp \
	GameApplication.hpp \
	stunts.hpp \
	UserInput.hpp \
	OgreTask.hpp \
	OgreTimer.hpp\
	CSound.hpp \
	CSoundSystem.hpp \
	BaseObject.hpp
	InteractiveObject.hpp\
	CarObject.hpp\
	Wheel.hpp\
	Engine.hpp\
	GearBox.hpp\
	Event.hpp\
	BaseControl.hpp\
	AICarControl.hpp\
	HumanControl.hpp\
	Atmosphere.hpp\
	Utils.hpp\
	ObjectInstantiator.hpp\
	CGuiAction.h\
	CGuiAddInt.h\
	CGuiButton.h\
	CGuiDummy.h\
	CGuiExitGui.h\
	CGuiGfx.h\
	CGuiMulti.h\
	CGuiObject.h\
	CGuiPage.h\
	CGuiParser.h\
	CGuiStartGame.h\
	CGuiTask.h\
	CGuiTex.h\
	GuiActions.h\
	GuiGfx.h\
	GuiObjects.h\
	Waypoint.hpp\
	FileIO.h\
	CFunction.h \
	CKI.h \
	CNeuralNetwork.h \
	CNeurode.h \
	LevelManager.hpp


# Set the include path found by configure
INCLUDES = \
	@NRENGINE_CPPFLAGS@ \
	@OGRE_CPPFLAGS@ \
	@OGREODE_CPPFLAGS@ \
	@ODE_CPPFLAGS@ \
	@BOOST_CPPFLAGS@ \
	@FMOD_CPPFLAGS@ \
	$(all_includes)

# The library search path for stunts
stunts_LDFLAGS = \
	@NRENGINE_LIBS@ \
	@OGRE_LIBS@ \
	@OGREODE_LIBS@ \
	@ODE_LIBS@ \
	@BOOST_LIBS@ \
	@FMOD_LIBS@ \
	$(all_libraries)

# Add here all files that should go into the distribution in addition to the
# default files.
EXTRA_DIST =
