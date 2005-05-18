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
NRENGINE_CFLAGS=""
if test -n "$with_nrengine"; then
  NRENGINE_LIBS="-L$with_nrengine/lib"
  NRENGINE_CFLAGS="-I$with_nrengine/include"
fi

if test "X${no_glu}" != "Xyes"; then
  NRENGINE_LIBS="${NRENGINE_LIBS} ${GLU_LIBS}"
fi

NRENGINE_LIBS="${NRENGINE_LIBS}"
NRENGINE_CFLAGS="${NRENGINE_CFLAGS}"

ax_save_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${NRENGINE_CFLAGS} ${CPPFLAGS}"

AC_CACHE_CHECK([for nrEngine], [ax_cv_check_nrengine_libs],
[ax_cv_check_nrengine_libs="no"
ax_save_LIBS="${LIBS}"
LIBS=""
ax_lib="-lnrFramework -lnrEngine -lnrVFS -lnrUtils -lnr3DEngine -lnrMath -lSDL -ljpeg -lpng"
if test "X$CC" = "Xcl"; then
  ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
else
  ax_try_lib="${ax_lib}"
fi
LIBS="${NRENGINE_LIBS} $GL_LIBS $GLU_LIBS ${ax_try_lib} ${ax_save_LIBS}"
AC_TRY_LINK([
# include <nrEngine/nrEngine.h>
],
[nrFrameworkInit()],
[ax_cv_check_nrengine_libs="${ax_try_lib}"; break])

LIBS=${ax_save_LIBS}
])
CPPFLAGS="${ax_save_CPPFLAGS}"

if test "X${ax_cv_check_nrengine_libs}" = "Xno"; then
  no_cg="yes"
  NRENGINE_CFLAGS=""
  NRENGINE_LIBS=""
else
  NRENGINE_LIBS="${ax_cv_check_nrengine_libs} ${NRENGINE_LIBS}"
fi

AC_SUBST([NRENGINE_CFLAGS])
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
# "ODE_LIBS", respectively. This macro adds the configure option
# "--with-apple-opengl-framework", which users can use to indicate that
# Apple's OpenGL framework should be used on Mac OS X. If Apple's OpenGL
# framework is used, the symbol "HAVE_APPLE_OPENGL_FRAMEWORK" is
# defined. If ODE is not found, "no_ode" is set to "yes".
#
AC_DEFUN([AX_CHECK_ODE],
[
  AC_LANG_SAVE
  AC_LANG_C

  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${ODE_CFLAGS} ${CPPFLAGS}"

  AC_CACHE_CHECK([for ODE library], [ax_cv_check_ode_libode],
  [ax_cv_check_ode_libode="no"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-lode"
  for ax_lib in ${ax_check_libs}; do
    LIBS="-lode"
    AC_TRY_LINK([
# include <ode/ode.h>
# include <ode/collision.h>
  ],
    [dWorldCreate()],
    [ax_cv_check_ode_libode="-lode"; break])

  done
  LIBS=${ax_save_LIBS}
  ])
  CPPFLAGS="${ax_save_CPPFLAGS}"
  AC_LANG_RESTORE

  if test "X${ax_cv_check_ode_libode}" = "Xno"; then
    no_ode="yes"
    ODE_CFLAGS=""
    ODE_LIBS=""
  else
    ODE_LIBS="${ax_cv_check_ode_libode} ${ODE_LIBS}"
  fi

AC_SUBST([ODE_CFLAGS])
AC_SUBST([ODE_LIBS])
])dnl
