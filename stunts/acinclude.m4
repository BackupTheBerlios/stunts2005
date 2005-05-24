#
# Synopsis
# 
# ACX_PTHREAD([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
# 
# Description
# 
# This macro figures out how to build C programs using POSIX
# threads. It sets the PTHREAD_LIBS output variable to the threads
# library and linker flags, and the PTHREAD_CFLAGS output variable to
# any special C compiler flags that are needed. (The user can also force
# certain compiler flags/libs to be tested by setting these environment
# variables.)
# 
# Also sets PTHREAD_CC to any special C compiler that is needed for
# multi-threaded programs (defaults to the value of CC otherwise). (This
# is necessary on AIX to use the special cc_r compiler alias.)
# 
# NOTE: You are assumed to not only compile your program with these
# flags, but also link it with them as well. e.g. you should link with
# $PTHREAD_CC $CFLAGS $PTHREAD_CFLAGS $LDFLAGS ... $PTHREAD_LIBS $LIBS
# 
# If you are only building threads programs, you may wish to use these
# variables in your default LIBS, CFLAGS, and CC:
# 
#        LIBS="$PTHREAD_LIBS $LIBS"
#        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
#        CC="$PTHREAD_CC"
# 
# In addition, if the PTHREAD_CREATE_JOINABLE thread-attribute
# constant has a nonstandard name, defines PTHREAD_CREATE_JOINABLE to
# that name (e.g. PTHREAD_CREATE_UNDETACHED on AIX).
# 
# ACTION-IF-FOUND is a list of shell commands to run if a threads
# library is found, and ACTION-IF-NOT-FOUND is a list of commands to run
# it if it is not found. If ACTION-IF-FOUND is not specified, the
# default action will define HAVE_PTHREAD.
# 
# Please let the authors know if this macro fails on any platform, or
# if you have any other suggestions or comments. This macro was based on
# work by SGJ on autoconf scripts for FFTW (www.fftw.org) (with help
# from M. Frigo), as well as ac_pthread and hb_pthread macros posted by
# Alejandro Forero Cuervo to the autoconf macro repository. We are also
# grateful for the helpful feedback of numerous users.
# 
# Version
# 
# 1.10 (last modified: 2004-06-16)
# 
# Author
# 
# Steven G. Johnson <stevenj@alum.mit.edu>
#
AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all, and "pthread-config"
# which is a program returning the flags for the Pth emulation library.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt pthread-config"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++
# pthread-config: use pthread-config program (for GNU Pth library)

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                pthread-config)
                AC_CHECK_PROG(acx_pthread_config, pthread-config, yes, no)
                if test x"$acx_pthread_config" = xno; then continue; fi
                PTHREAD_CFLAGS="`pthread-config --cflags`"
                PTHREAD_LIBS="`pthread-config --ldflags` `pthread-config --libs`"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd* | *-darwin*) flag="-D_THREAD_SAFE";;
                *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD

# 
# Synopsis
# 
# AX_CHECK_GL
# 
# Description
# 
# Check for an OpenGL implementation. If GL is found, the required
# compiler and linker flags are included in the output variables
# "GL_CFLAGS" and "GL_LIBS", respectively. This macro adds the configure
# option "--with-apple-opengl-framework", which users can use to
# indicate that Apple's OpenGL framework should be used on Mac OS X. If
# Apple's OpenGL framework is used, the symbol
# "HAVE_APPLE_OPENGL_FRAMEWORK" is defined. If no GL implementation is
# found, "no_gl" is set to "yes".
# 
# Version
# 
# 1.6
# 
# Author
# 
# Braden McDaniel <braden@endoframe.com>
# 
# M4 Source Code
#
AC_DEFUN([AX_CHECK_GL],
[AC_REQUIRE([AC_PATH_X])dnl
AC_REQUIRE([ACX_PTHREAD])dnl

#
# There isn't a reliable way to know we should use the Apple OpenGL framework
# without a configure option.  A Mac OS X user may have installed an
# alternative GL implementation (e.g., Mesa), which may or may not depend on X.
#
AC_ARG_WITH([apple-opengl-framework],
            [AC_HELP_STRING([--with-apple-opengl-framework],
                            [use Apple OpenGL framework (Mac OS X only)])])
if test "X$with_apple_opengl_framework" = "Xyes"; then
  AC_DEFINE([HAVE_APPLE_OPENGL_FRAMEWORK], [1],
            [Use the Apple OpenGL framework.])
  GL_LIBS="-framework OpenGL"
else
  GL_CFLAGS="${PTHREAD_CFLAGS}"
  GL_LIBS="${PTHREAD_LIBS} -lm"

  #
  # Use x_includes and x_libraries if they have been set (presumably by
  # AC_PATH_X).
  #
  if test "X$no_x" != "Xyes"; then
    if test -n "$x_includes"; then
      GL_CFLAGS="-I${x_includes} ${GL_CFLAGS}"
    fi
    if test -n "$x_libraries"; then
      GL_LIBS="-L${x_libraries} -lX11 ${GL_LIBS}"
    fi
  fi

  AC_LANG_PUSH(C)

  AC_CHECK_HEADERS([windows.h])

  AC_CACHE_CHECK([for OpenGL library], [ax_cv_check_gl_libgl],
  [ax_cv_check_gl_libgl="no"
  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${GL_CFLAGS} ${CPPFLAGS}"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-lopengl32 -lGL"
    for ax_lib in ${ax_check_libs}; do
    if test "X$CC" = "Xcl"; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${GL_LIBS} ${ax_save_LIBS}"
    AC_TRY_LINK([
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include <GL/gl.h>
],
    [glBegin(0)],
    [ax_cv_check_gl_libgl="${ax_try_lib}"; break])
  done
  LIBS=${ax_save_LIBS}
  CPPFLAGS=${ax_save_CPPFLAGS}])

  if test "X${ax_cv_check_gl_libgl}" = "Xno"; then
    no_gl="yes"
    GL_CFLAGS=""
    GL_LIBS=""
  else
    GL_LIBS="${ax_cv_check_gl_libgl} ${GL_LIBS}"
  fi
  AC_LANG_POP(C)
fi

AC_SUBST([GL_CFLAGS])
AC_SUBST([GL_LIBS])
])dnl


#
# Synopsis
# 
# AX_CHECK_GLU
# 
# Description
#
# Check for GLU. If GLU is found, the required preprocessor and linker
# flags are included in the output variables "GLU_CFLAGS" and
# "GLU_LIBS", respectively. This macro adds the configure option
# "--with-apple-opengl-framework", which users can use to indicate that
# Apple's OpenGL framework should be used on Mac OS X. If Apple's OpenGL
# framework is used, the symbol "HAVE_APPLE_OPENGL_FRAMEWORK" is
# defined. If no GLU implementation is found, "no_glu" is set to "yes".
# 
# Version
#
# 1.1
# 
# Author
# 
# Braden McDaniel <braden@endoframe.com>
#
AC_DEFUN([AX_CHECK_GLU],
[AC_REQUIRE([AX_CHECK_GL])dnl
GLU_CFLAGS="${GL_CFLAGS}"
if test "X${with_apple_opengl_framework}" != "Xyes"; then
  AC_CACHE_CHECK([for OpenGL Utility library], [ax_cv_check_glu_libglu],
  [ax_cv_check_glu_libglu="no"
  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${GL_CFLAGS} ${CPPFLAGS}"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-lglu32 -lGLU"
  for ax_lib in ${ax_check_libs}; do
    if test "X$CC" = "Xcl"; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${GL_LIBS} ${ax_save_LIBS}"
    #
    # libGLU typically links with libstdc++ on POSIX platforms. However,
    # setting the language to C++ means that test program source is named
    # "conftest.cc"; and Microsoft cl doesn't know what to do with such a
    # file.
    #
    if test "X$CXX" != "Xcl"; then
      AC_LANG_PUSH([C++])
    fi
    AC_TRY_LINK([
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include <GL/glu.h>
],
    [gluBeginCurve(0)],
    [ax_cv_check_glu_libglu="${ax_try_lib}"; break])
    if test "X$CXX" != "Xcl"; then
      AC_LANG_POP([C++])
    fi
  done
  LIBS=${ax_save_LIBS}
  CPPFLAGS=${ax_save_CPPFLAGS}])
  if test "X${ax_cv_check_glu_libglu}" = "Xno"; then
    no_glu="yes"
    GLU_CFLAGS=""
    GLU_LIBS=""
  else
    GLU_LIBS="${ax_cv_check_glu_libglu} ${GL_LIBS}"
  fi
fi
AC_SUBST([GLU_CFLAGS])
AC_SUBST([GLU_LIBS])
])

#
# Synopsis
#
# AX_CHECK_NRENGINE
# 
# Version
# 
# 1.0     0.0.00   :   fw
# 
# Author
# 
# Florian Winter <fw@graphics.cs.uni-sb.de> 
#  copyright: (C) 2003 Florian Winter
#  license: Wayne?
# 
# Description
# 
# Check for nrEngine
#
AC_DEFUN([AX_CHECK_NRENGINE],
[
AC_REQUIRE([AX_CHECK_GL])
AC_REQUIRE([AX_CHECK_GLU])
AC_ARG_WITH([nrengine],
            [AC_HELP_STRING([--with-nrengine],
                            [specify nrEngine path])])

NRENGINE_LIBS=""
NRENGINE_CPPFLAGS=""
if test -n "$with_nrengine"; then
  NRENGINE_LIBS="-L$with_nrengine/lib"
  NRENGINE_CPPFLAGS="-I$with_nrengine/include"
fi

if test "X${no_glu}" != "Xyes"; then
  NRENGINE_LIBS="${NRENGINE_LIBS} ${GLU_LIBS}"
fi

NRENGINE_LIBS="${NRENGINE_LIBS} -lSDL -lGL -lGLU -lpng -ljpeg"
NRENGINE_CPPFLAGS="${NRENGINE_CPPFLAGS}"

ax_save_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${NRENGINE_CPPFLAGS} ${CPPFLAGS}"

AC_CACHE_CHECK([for nrEngine], [ax_cv_check_nrengine_libs],
[ax_cv_check_nrengine_libs="no"
ax_save_LIBS="${LIBS}"
LIBS="-lnrEngine  -lSDL -lGL -lGLU -lpng -ljpeg"
ax_lib="-lnrEngine -lSDL -lGL -lGLU -lpng -ljpeg"
if test "X$CC" = "Xcl"; then
  ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
else
  ax_try_lib="${ax_lib}"
fi
LIBS="${NRENGINE_LIBS} $GLU_LIBS ${ax_try_lib} ${ax_save_LIBS}"
AC_TRY_LINK([
# include <nrEngine/nrEngine.h>
],
[nrEngineInit()],
[ax_cv_check_nrengine_libs="${ax_try_lib}"; break])

LIBS=${ax_save_LIBS}
])
CPPFLAGS="${ax_save_CPPFLAGS}"

if test "X${ax_cv_check_nrengine_libs}" = "Xno"; then
  no_cg="yes"
  NRENGINE_CPPFLAGS=""
  NRENGINE_LIBS=""
else
  NRENGINE_LIBS="${ax_cv_check_nrengine_libs} ${NRENGINE_LIBS}"
fi

AC_SUBST([NRENGINE_CPPFLAGS])
AC_SUBST([NRENGINE_LIBS])
])dnl


#
# Synopsis
#
# AX_CHECK_ODE
# 
# Version
# 
# 1.6     0.5.58   :   braden 
# 
# Author
# 
# Braden McDaniel <braden@endoframe.com> 
#  copyright: (C) 2003 Braden McDaniel 
#  license: GNU GPL 
# 
# Description
# 
# Check for ODE. If ODE is found, the required compiler and linker
# flags are included in the output variables "ODE_CFLAGS" and
# "ODE_LIBS", respectively.If ODE is not found, "no_ode" is set to "yes".
#
AC_DEFUN([AX_CHECK_ODE],
[
  AC_ARG_WITH([ode],
            [AC_HELP_STRING([--with-ode],
                            [specify ODE path])])
  ODE_LIBS=""
  ODE_CPPFLAGS=""
  if test -n "$with_ode"; then
    ODE_LIBS="-L$with_ode/lib"
    ODE_CPPFLAGS="-I$with_ode/include"
  fi

  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${ODE_CPPFLAGS} ${CPPFLAGS}"

  AC_CACHE_CHECK([for ODE library], [ax_cv_check_ode_libode],
  [ax_cv_check_ode_libode="no"
  ax_save_LIBS="${LIBS}"
  LIBS="${ODE_LIBS} -lode"
    AC_TRY_LINK([
# include <ode/ode.h>
# include <ode/collision.h>
  ],
    [dWorldCreate()],
    [ax_cv_check_ode_libode="${ODE_LIBS} -lode"; break])

  LIBS=${ax_save_LIBS}
  ])
  CPPFLAGS="${ax_save_CPPFLAGS}"

  if test "X${ax_cv_check_ode_libode}" = "Xno"; then
    no_ode="yes"
    ODE_CPPFLAGS=""
    ODE_LIBS=""
  else
    ODE_LIBS="${ax_cv_check_ode_libode} ${ODE_LIBS}"
  fi

AC_SUBST([ODE_CPPFLAGS])
AC_SUBST([ODE_LIBS])
])dnl


#
# Synopsis
#
# AX_CHECK_FMOD
# 
# Version
# 
# 1.6     0.5.58   :   fw
# 
# Author
# 
# Braden McDaniel <braden@endoframe.com> 
#  copyright: (C) 2003 Braden McDaniel 
#  license: GNU GPL 
# 
# Description
# 
#
AC_DEFUN([AX_CHECK_FMOD],
[
  AC_ARG_WITH([fmod],
            [AC_HELP_STRING([--with-fmod],
                            [specify FMOD path])])
  FMOD_LIBS=""
  FMOD_CPPFLAGS=""
  if test -n "$with_fmod"; then
    FMOD_LIBS="-L$with_fmod"
    FMOD_CPPFLAGS="-I$with_fmod/inc"
  fi

  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${FMOD_CPPFLAGS} ${CPPFLAGS}"

  AC_CACHE_CHECK([for FMOD library], [ax_cv_check_fmod_libfmod],
  [ax_cv_check_fmod_libfmod="no"
  ax_save_LIBS="${LIBS}"
  LIBS="${FMOD_LIBS} -lfmod-3.74"
    AC_TRY_LINK([
# include <fmod.h>
  ],
    [FSOUND_Init(44100, 32, 0);],
    [ax_cv_check_fmod_libfmod="${FMOD_LIBS} -lfmod-3.74"; break])

  LIBS=${ax_save_LIBS}
  ])
  CPPFLAGS="${ax_save_CPPFLAGS}"

  if test "X${ax_cv_check_fmod_libfmod}" = "Xno"; then
    no_fmod="yes"
    FMOD_CPPFLAGS=""
    FMOD_LIBS=""
  else
    FMOD_LIBS="${ax_cv_check_fmod_libfmod} ${FMOD_LIBS}"
  fi

AC_SUBST([FMOD_CPPFLAGS])
AC_SUBST([FMOD_LIBS])
])dnl


dnl RS_BOOST([MINIMUM-VERSION], [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost C++ libraries of a particular version (or newer)
dnl Defines:
dnl   BOOST_CPPFLAGS to the set of flags required to compiled Boost
AC_DEFUN([RS_BOOST], 
[
  AC_SUBST(BOOST_CPPFLAGS)
  AC_SUBST(BOOST_LIBS)
  BOOST_CPPFLAGS=""
  path_given="no"

dnl Extract the path name from a --with-boost=PATH argument
  AC_ARG_WITH(boost,
    AC_HELP_STRING([--with-boost=PATH],[absolute path name where the Boost C++ libraries reside, or `int', for internal library. Alternatively, the BOOST_ROOT environment variable will be used]),
    [
    if test "$withval" != yes ; then
        BOOST_ROOT="$withval"
    fi
    ]
  )

  if test "x$BOOST_ROOT" = x ; then
    BOOST_ROOT="/usr"
  fi

  boost_min_version=ifelse([$1], ,1.20.0,[$1])
  WANT_BOOST_MAJOR=`expr $boost_min_version : '\([[0-9]]\+\)'`
  WANT_BOOST_MINOR=`expr $boost_min_version : '[[0-9]]\+\.\([[0-9]]\+\)'`
  WANT_BOOST_SUB_MINOR=`expr $boost_min_version : '[[0-9]]\+\.[[0-9]]\+\.\([[0-9]]\+\)'`

  BOOST_CPPFLAGS="-I$BOOST_ROOT/include/boost"
  dnl -${WANT_BOOST_MAJOR}_$WANT_BOOST_MINOR"

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
  AC_MSG_CHECKING([for the Boost C++ libraries, version $boost_min_version or newer])
  AC_TRY_COMPILE(
    [
#include <boost/version.hpp>
],
    [],
    [
      have_boost="yes"
    ],
    [
      AC_MSG_RESULT(no)
      have_boost="no"
      ifelse([$3], , :, [$3])
    ])

  if test "$have_boost" = "yes"; then
    WANT_BOOST_VERSION=`expr $WANT_BOOST_MAJOR \* 100000 \+ $WANT_BOOST_MINOR \* 100 \+ $WANT_BOOST_SUB_MINOR`

    AC_TRY_COMPILE(
      [
#include <boost/version.hpp>
],
      [
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
#  error Boost version is too old
#endif

],
      [
	AC_MSG_RESULT(yes)
	if test "$target_os" = "mingw32"; then
	   boost_libsuff=mgw
	else
	   boost_libsuff=gcc
	fi
	boost_libsuff_r=$boost_libsuff-mt;
	if test "x$enable_debug" = xyes ; then
	    boost_libsuff=$boost_libsuff-d;
	    boost_libsuff_r=$boost_libsuff_r-d;
	fi
	boost_libsuff=$boost_libsuff-${WANT_BOOST_MAJOR}_$WANT_BOOST_MINOR
	boost_libsuff_r=$boost_libsuff_r-${WANT_BOOST_MAJOR}_$WANT_BOOST_MINOR
        ifelse([$2], , :, [$2])
      ],
      [
        AC_MSG_RESULT([no, version of installed Boost libraries is too old])
        ifelse([$3], , :, [$3])
      ])
  fi
  CPPFLAGS="$OLD_CPPFLAGS"
  AC_LANG_RESTORE
])

dnl RS_BOOST_THREAD([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost thread library
dnl Defines
dnl   BOOST_LDFLAGS to the set of flags required to compile boost_thread
AC_DEFUN([RS_BOOST_THREAD], 
[
    AC_REQUIRE([RS_BOOST])
  AC_MSG_CHECKING([whether we can use boost_thread library])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS -D_REENTRANT"
  OLD_LIBS="$LIBS"
  LIBS="-lboost_thread-$boost_libsuff_r"
    AC_TRY_COMPILE(
	    [ 
		#include <boost/thread.hpp> 
		bool bRet = 0;
		void thdfunc() { bRet = 1; }
	    ],
	    [
		boost::thread thrd(&thdfunc);
		thrd.join();
		return bRet == 1;
	    ], 
	    [
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
	    ],
	    [ 
		AC_MSG_RESULT([no])
		ifelse([$2], , :, [$2])
	    ])

    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LIBS_R)
    BOOST_CPPFLAGS="$CPPFLAGS"
    BOOST_LIBS_R="$LIBS $BOOST_LIBS_R"
    CPPFLAGS="$OLD_CPPFLAGS"
    LIBS="$OLD_LIBS"
    AC_LANG_RESTORE
])
	    
dnl RS_BOOST_DATETIME([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost datetime library
dnl Defines
dnl   BOOST_LDFLAGS to the set of flags required to compile boost_datetime
AC_DEFUN([RS_BOOST_DATETIME], 
[
    AC_REQUIRE([RS_BOOST])
  AC_MSG_CHECKING([whether we can use boost_datetime library])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS"
  OLD_LIBS="$LIBS"
  LIBS="-lboost_date_time-$boost_libsuff"
    AC_TRY_COMPILE(
	    [ 
		#include <boost/date_time/gregorian/gregorian.hpp> 
	    ],
	    [
		using namespace boost::gregorian;
		date d = from_string("1978-01-27");
		return d == boost::gregorian::date(1978, Jan, 27);
	    ], 
	    [
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
	    ],
	    [ 
		AC_MSG_RESULT([no])
		ifelse([$2], , :, [$2])
	    ])

    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LIBS)
    AC_SUBST(BOOST_LIBS_R)
    BOOST_CPPFLAGS="$CPPFLAGS"
    BOOST_LIBS="$LIBS $BOOST_LIBS"
    BOOST_LIBS_R="$LIBS $BOOST_LIBS_R"
    CPPFLAGS="$OLD_CPPFLAGS"
    LIBS=$OLD_LIBS
    AC_LANG_RESTORE
])
	    
dnl RS_BOOST_REGEX([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost regex library
dnl Defines
dnl   BOOST_LDFLAGS to the set of flags required to compile boost_datetime
AC_DEFUN([RS_BOOST_REGEX], 
[
    AC_REQUIRE([RS_BOOST])
  AC_MSG_CHECKING([whether we can use boost_regex library])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS"
  OLD_LIBS="$LIBS"
  LIBS="-lboost_regex-$boost_libsuff_r"
    AC_TRY_COMPILE(
	    [ 
		#include <boost/regex.hpp> 
	    ],
	    [
		using namespace boost;
		cmatch what;
		if(!regex_match("27/01/78",what,regex("(\\\d+)/(\\\d+)/(\\\d+)")))
		    return 0;

		return what[1]=="27" && what[2]=="01" && what[3]=="78";
	    ], 
	    [
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
	    ],
	    [ 
		AC_MSG_RESULT([no])
		ifelse([$2], , :, [$2])
	    ])

    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LIBS)
    AC_SUBST(BOOST_LIBS_R)
    BOOST_CPPFLAGS="$CPPFLAGS"
    BOOST_LIBS="$LIBS $BOOST_LIBS"
    BOOST_LIBS_R="$LIBS $BOOST_LIBS_R"
    CPPFLAGS="$OLD_CPPFLAGS"
    LIBS="$OLD_LIBS"
    AC_LANG_RESTORE
])

dnl RS_BOOST_PROGRAM_OPTIONS([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost program options library
dnl Defines
dnl   BOOST_LDFLAGS to the set of flags required to compile boost_datetime
AC_DEFUN([RS_BOOST_PROGRAM_OPTIONS], 
[
    AC_REQUIRE([RS_BOOST])
  AC_MSG_CHECKING([whether we can use boost_program_options library])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS"
  OLD_LIBS="$LIBS"
  LIBS="-lboost_program_options-$boost_libsuff_r"
    AC_TRY_COMPILE(
	    [ 
		#include <boost/program_options.hpp> 
	    ],
	    [
		using namespace boost::program_options;
		return 0;
	    ], 
	    [
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
	    ],
	    [ 
		AC_MSG_RESULT([no])
		ifelse([$2], , :, [$2])
	    ])

    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LIBS)
    AC_SUBST(BOOST_LIBS_R)
    BOOST_CPPFLAGS="$CPPFLAGS"
    BOOST_LIBS="$LIBS $BOOST_LIBS"
    BOOST_LIBS_R="$LIBS $BOOST_LIBS_R"
    CPPFLAGS="$OLD_CPPFLAGS"
    LIBS="$OLD_LIBS"
    AC_LANG_RESTORE
])

dnl RS_BOOST_SERIALIZATION([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl Test for the Boost serialization library
dnl Defines
dnl   BOOST_LDFLAGS to the set of flags required to compile boost_datetime
AC_DEFUN([RS_BOOST_SERIALIZATION], 
[
    AC_REQUIRE([RS_BOOST])
  AC_MSG_CHECKING([whether we can use boost_serialization library])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  OLD_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS"
  OLD_LIBS="$LIBS"
  LIBS="-lboost_serialization"
    AC_TRY_COMPILE(
	    [ 
		#include <boost/serialization/export.hpp> 
	    ],
	    [
		using namespace boost::serialization;
		return 0;
	    ], 
	    [
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
	    ],
	    [ 
		AC_MSG_RESULT([no])
		ifelse([$2], , :, [$2])
	    ])

    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LIBS)
    AC_SUBST(BOOST_LIBS_R)
    BOOST_CPPFLAGS="$CPPFLAGS"
    BOOST_LIBS="$LIBS $BOOST_LIBS"
    BOOST_LIBS_R="$LIBS $BOOST_LIBS_R"
    CPPFLAGS="$OLD_CPPFLAGS"
    LIBS="$OLD_LIBS"
    AC_LANG_RESTORE
])

#
# Synopsis
#
# AX_CHECK_OGRE
# 
# Version
# 
# 1.6     0.5.58   :   fw
# 
# Author
# 
# Braden McDaniel <fw@graphics.cs.uni-sb.de> 
#  copyright: (C) 2003 Florian Winter
#  license: GNU GPL 
# 
# Description
# 
# Check for Ogre
#
AC_DEFUN([AX_CHECK_OGRE],
[
  AC_ARG_WITH([ogre],
            [AC_HELP_STRING([--with-ogre],
                            [specify OGRE path])])
  OGRE_LIBS=""
  OGRE_CPPFLAGS="-DGCC_3_1 -DEXT_HASH"
  if test -n "$with_ogre"; then
    OGRE_LIBS="-L$with_ogre/lib"
    OGRE_CPPFLAGS="${OGRE_CPPFLAGS} -I$with_ogre/include"
  fi

  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${OGRE_CPPFLAGS} ${CPPFLAGS}"

  AC_CACHE_CHECK([for OGRE library], [ax_cv_check_ogre_libogre],
  [ax_cv_check_ogre_libogre="no"
  ax_save_LIBS="${LIBS}"
  LIBS="${OGRE_LIBS} -lOgreMain"
    AC_TRY_LINK([
# include <OGRE/Ogre.h>
  ],
    [Ogre::Root* root = new Ogre::Root()],
    [ax_cv_check_ogre_libogre="${OGRE_LIBS} -lOgreMain"; break])

  LIBS=${ax_save_LIBS}
  ])
  CPPFLAGS="${ax_save_CPPFLAGS}"

  if test "X${ax_cv_check_ogre_libogre}" = "Xno"; then
    no_ogre="yes"
    OGRE_CPPFLAGS=""
    OGRE_LIBS=""
  else
    OGRE_LIBS="${ax_cv_check_ogre_libogre} ${OGRE_LIBS}"
  fi

AC_SUBST([OGRE_CPPFLAGS])
AC_SUBST([OGRE_LIBS])
])dnl
