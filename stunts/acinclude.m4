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
ax_lib="-lnrFramework -lnrEngine -lnrVFS -lnrUtils -lnr3DEngine -lnrMath -lSDL -lGL -lGLU -ljpeg -lpng"
if test "X$CC" = "Xcl"; then
  ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
else
  ax_try_lib="${ax_lib}"
fi
LIBS="${NRENGINE_LIBS} ${ax_try_lib} ${ax_save_LIBS}"
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
