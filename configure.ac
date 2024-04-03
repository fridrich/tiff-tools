dnl                                               -*- Autoconf -*-
dnl Tag Image File Format (TIFF) Software
dnl
dnl Copyright (C) 2004, Andrey Kiselev <dron@ak4719.spb.edu>
dnl
dnl Permission to use, copy, modify, distribute, and sell this software and 
dnl its documentation for any purpose is hereby granted without fee, provided
dnl that (i) the above copyright notices and this permission notice appear in
dnl all copies of the software and related documentation, and (ii) the names of
dnl Sam Leffler and Silicon Graphics may not be used in any advertising or
dnl publicity relating to the software without the specific, prior written
dnl permission of Sam Leffler and Silicon Graphics.
dnl 
dnl THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
dnl EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
dnl WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
dnl 
dnl IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
dnl ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
dnl OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
dnl WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
dnl LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
dnl OF THIS SOFTWARE.

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([TIFF Tools],[4.6.0],[tiff@lists.maptools.org],[tiff-tools])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(m4)
AC_LANG(C)

dnl Compute the canonical host (run-time) system type variable
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([dist-xz tar-ustar])
dnl Do not rebuild generated files every time
AM_MAINTAINER_MODE

dnl Versioning.
dnl Don't fill the ALPHA_VERSION field, if not applicable.
TIFF_TOOLS_MAJOR_VERSION=4
TIFF_TOOLS_MINOR_VERSION=6
TIFF_TOOLS_MICRO_VERSION=0
TIFF_TOOLS_ALPHA_VERSION=
TIFF_TOOLS_VERSION=$TIFF_TOOLS_MAJOR_VERSION.$TIFF_TOOLS_MINOR_VERSION.$TIFF_TOOLS_MICRO_VERSION$TIFF_TOOLS_ALPHA_VERSION

AC_SUBST(TIFF_TOOLS_MAJOR_VERSION)
AC_SUBST(TIFF_TOOLS_MINOR_VERSION)
AC_SUBST(TIFF_TOOLS_MICRO_VERSION)
AC_SUBST(TIFF_TOOLS_ALPHA_VERSION)
AC_SUBST(TIFF_TOOLS_VERSION)

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

dnl We want warnings. As many warnings as possible.
VL_PROG_CC_WARNINGS()

dnl Checks for programs
AC_PROG_INSTALL
AC_PROG_LN_S
# Used only for validating the source distribution during distcheck
AC_PATH_PROG(CMAKE, cmake)

dnl Tests for Windows
AC_EXEEXT
AC_OBJEXT

# Enable support for silent build rules
AM_SILENT_RULES

dnl We don't need to add math library to all targets
case "${host_os}" in
    cygwin* | mingw32* | beos* | darwin*)
        ;;
    *)
	AC_CHECK_LIB(m,sin,[libm_lib=yes], [libm_lib=no],)
	if test "x$libm_lib" = "xyes" ; then
	  LIBS="-lm $LIBS"
	  tiff_libs_private="-lm ${tiff_libs_private}"
	fi
        ;;
esac

dnl Checks for header files.
AC_CHECK_HEADERS([assert.h fcntl.h io.h search.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_BIGENDIAN
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_HEADERS(sys/time.h)
AC_STRUCT_TM

dnl If the `fseeko' function is available, define `HAVE_FSEEKO'. Define
dnl `_LARGEFILE_SOURCE' if necessary.
AC_FUNC_FSEEKO

dnl Check if optarg (and presumably related externs) already declared in headers
AC_CHECK_DECLS([optarg])

dnl ---------------------------------------------------------------------------
dnl Compute sized types for current CPU and compiler options
dnl ---------------------------------------------------------------------------

# Obtain the size of 'size_t' and define as SIZEOF_SIZE_T.  Result is
# available in ac_cv_sizeof_size_t
AC_CHECK_SIZEOF([size_t])


# C99 fixed-size integer types
INT8_T='int8_t'
UINT8_T='uint8_t'

INT16_T='int16_t'
UINT16_T='uint16_t'

INT32_T='int32_t'
UINT32_T='uint32_t'

INT64_T='int64_t'
UINT64_T='uint64_t'

if test $ac_cv_sizeof_size_t -eq 4
then
  SSIZE_T='int32_t'
elif test $ac_cv_sizeof_size_t -eq 8
then
  SSIZE_T='int64_t'
else
  AC_MSG_ERROR([Unsupported size_t size ${ac_cv_sizeof_size_t}; please add support])
fi

dnl ---------------------------------------------------------------------------
dnl Support large files.
dnl ---------------------------------------------------------------------------

AC_SYS_LARGEFILE

dnl ---------------------------------------------------------------------------
dnl Find libtiff.
dnl ---------------------------------------------------------------------------

PKG_CHECK_MODULES([LIBTIFF],[libtiff-4])
AC_SUBST([LIBTIFF_CFLAGS])
AC_SUBST([LIBTIFF_LIBS])

dnl ---------------------------------------------------------------------------
dnl Check for OpenGL and GLUT.
dnl ---------------------------------------------------------------------------

HAVE_OPENGL=no

AC_ARG_ENABLE(opengl,
	      AS_HELP_STRING([--disable-opengl],
			     [disable OpenGL usage (required by tiffgt, enabled by default)]),,)

if test "x$enable_opengl" != "xno" ; then

  AC_PATH_XTRA

  dnl AX_CHECK_GL sets GL_CFLAGS & GL_LIBS.  Also PTHREAD_LIBS,
  dnl PTHREAD_CFLAGS, & PTHREAD_CC as a side-effect
  AX_CHECK_GL

  dnl AX_CHECK_GLU sets GLU_CFLAGS & GLU_LIBS
  AX_CHECK_GLU

  dnl AX_CHECK_GLUT sets GLUT_CFLAGS & GLUT_LIBS
  AX_CHECK_GLUT

  if test "$no_x" != "yes" -a "$no_gl" != "yes" \
      -a "$no_glu" != "yes" -a "$no_glut" != "yes" ; then
    HAVE_OPENGL=yes
  fi
fi

AM_CONDITIONAL(HAVE_OPENGL, test "$HAVE_OPENGL" = "yes")

dnl ---------------------------------------------------------------------------
dnl Check for X Athena Widgets
dnl ---------------------------------------------------------------------------

dnl HAVE_XAW=no

dnl ICE_FIND_ATHENA

dnl if test "$no_xaw" != "yes" ; then
dnl   HAVE_XAW=yes
dnl fi

dnl AM_CONDITIONAL(HAVE_XAW, test "$HAVE_XAW" = "yes")


dnl ---------------------------------------------------------------------------
dnl Check for X Sphinx Build to generate manpages
dnl ---------------------------------------------------------------------------


for python in python3.9 python3.8 python3.7 python3.6 python3.5 python3.4 python3.3 python3.2 python3.1 python3.0 python3 python; do
  AC_CHECK_PROGS(PYTHON_BIN, [$python])
  if test "x$PYTHON_BIN" != "x"; then
    break
  fi
done

AC_ARG_ENABLE(sphinx,
              AS_HELP_STRING([--disable-sphinx],
                             [disable building of the sphinx documentation]),
              [BUILD_SPHINX=$enableval],
              [BUILD_SPHINX=yes])


AC_PATH_PROG(SPHINX_BUILD, [sphinx-build], [no])

AM_CONDITIONAL([SPHINX], [test "$BUILD_SPHINX" = yes -a "$SPHINX_BUILD" != no -a "x$PYTHON_BIN" != "x"])

if test "x$PYTHON_BIN" != "x"; then
  AC_MSG_CHECKING([generated manual pages])
  TIFF_MAN1_GEN=$(echo $("$PYTHON_BIN" "${srcdir}/list-manpages.py" "${srcdir}/doc" man | grep "\.1$"))
  TIFF_MAN_RULES=$("$PYTHON_BIN" "${srcdir}/list-manpages.py" "${srcdir}/doc" man | sed -e 's;$;: all-local;')
  AC_SUBST([TIFF_MAN1_GEN])
  AC_SUBST([TIFF_MAN_RULES])
  AM_SUBST_NOTMAKE([TIFF_MAN_RULES])
  AC_MSG_RESULT([done])
fi

if test -d "${srcdir}/doc/man-prebuilt"; then
  AC_MSG_CHECKING([prebuilt manual pages])
  TIFF_MAN1_PREBUILT=$(echo $(cd "${srcdir}/doc" && ls -1 man-prebuilt/*.1))
  AC_SUBST([TIFF_MAN1_PREBUILT])
  AC_MSG_RESULT([done])
fi

dnl ---------------------------------------------------------------------------

AC_CONFIG_HEADERS([config/config.h tiff_tools_config.h])

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT

dnl ---------------------------------------------------------------------------
dnl Display configuration status
dnl ---------------------------------------------------------------------------

LOC_MSG()
LOC_MSG([Libtiff is now configured for ${host}])
LOC_MSG()
LOC_MSG([  Installation directory:             ${prefix}])
LOC_MSG([  C compiler:                         ${CC} ${CFLAGS}])
LOC_MSG()
dnl LOC_MSG([  X Athena Widgets support:           ${HAVE_XAW}])
LOC_MSG([  OpenGL support:                     ${HAVE_OPENGL}])
LOC_MSG()

