# Add here all programs that should be built
# Process this file with automake to produce Makefile.in

# Add here all programs that should be built and installed in $PREFIX/bin
bin_PROGRAMS = stunts

# Add here all .cpp source files that are needed to build myapp
stunts_SOURCES = \
	Level.cpp \
	FrameListener.cpp \
	GameApplication.cpp \
	stunts.cpp \
	UserInput.cpp \
	OgreTask.cpp \
	Physics.cpp \
	PhysicsWorld.cpp
	CSound.cpp \
	CSoundSystem.cpp

# Add here all headers that are needed to build myapp
# These headers will not be installed, but included in the dist
noinst_HEADERS = \
	Level.hpp \
	FrameListener.hpp \
	GameApplication.hpp \
	stunts.hpp \
	UserInput.hpp \
	OgreTask.hpp \
	Physics.hpp \
	PhysicsWorld.hpp \
	CSound.hpp \
	CSoundSystem.hpp

# Set the include path found by configure
INCLUDES = \
	@NRENGINE_CPPFLAGS@ \
	@OGRE_CPPFLAGS@ \
	@ODE_CPPFLAGS@ \
	@BOOST_CPPFLAGS@ \
	@FMOD_CPPFLAGS@ \
	$(all_includes)

# The library search path for stunts
stunts_LDFLAGS = \
	@NRENGINE_LIBS@ \
	@OGRE_LIBS@ \
	@ODE_LIBS@ \
	@BOOST_LIBS@ \
	@FMOD_LIBS@ \
	$(all_libraries)

# Add here all files that should go into the distribution in addition to the
# default files.
EXTRA_DIST =
