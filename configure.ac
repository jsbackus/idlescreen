#"compile" w/:
#autoconf -o configure configure.ac
#chmod +x configure
AC_INIT(IdleScreen, version-0.4)

AC_COPYRIGHT([Copyright (c) 2008 Jeff Backus and the IdleScreen Project.
http://idlescreen.googlepages.com  Released under the GPLv2.])

AC_REVISION([$Revision 1.00$])

# Test validity of srcdir by looking for the MSVC++ solution.
# Note, this file is not necessary for compiling on *NIX systems!
AC_CONFIG_SRCDIR([idlescreen_msvc.sln])

AC_PROG_CXX

# check for qmake and check its version
#AC_MSG_CHECKING([for qmake])
AC_PATH_PROGS(QMAKEBIN, [qmake-qt4 qmake])
#AC_PATH_PROG(QMAKE, [qmake-qt4], [qmake-qt4],[0])
#if test $QMAKE == 0 then
#   AC_PATH_PROG(QMAKE, [qmake], [qmake], [0])
#   if test $QMAKE == 0 then
#      AC_MSG_ERROR([Qt 4 is required!  Please visit www.trolltech.com.])
#   fi
#fi
#if which qmake-qt4 > /dev/null
#then
	#m4_divert_text(1, [qmake-qt4 --version | grep "Qt version" |sed 's/\Using Qt version \([\.0-9]*\).*/\1/'])
#	AC_MSG_RESULT([yes])
#	AC_SUBST(QMAKE, qmake-qt4)
#else
#	AC_MSG_RESULT([no])
#	AC_MSG_CHECKING([for qmake])
#	if which qmake
#	then
#		AC_MSG_RESULT([yes])
#		AC_SUBST(QMAKE, qmake)
#	else
#		AC_MSG_RESULT([no])
#		AC_MSG_ERROR([Qt 4.0.0 or greater required!])
#	fi
#fi

# check Qt version(?)
AC_SUBST(qmake, $QMAKEBIN)

#temporary!!
AC_SUBST(bintrg, [PlasmaLenz])
AC_SUBST(othrtrg, [])
#end temporary

# check for doxygen
AC_ARG_ENABLE([doxygen], [AC_HELP_STRING([--enable-doxygen],
		      [build class documentation with Doxygen default=no])],
#		      [if test "$enableval" = "yes";
#		      then AC_PATH_PROGS(doxygen, [doxygen])
#		      fi])
		      [enable_doxygen=$enableval],
		      [enable_doxygen=no])

if test "$enable_doxygen" == "yes";
    then 
    	 AC_PATH_PROGS(doxygen, [doxygen])
	 AC_SUBST(othrtrg, [doc])
    else AC_SUBST(doxygen, [doxygen])
fi
#AC_PATH_PROGS(doxygen, [doxygen])

# check for necessary headers
# NOTE:  Needs updated!
AC_CHECK_HEADERS([getopt.h bzlib.h])

# check for OpenGL (taken from configure.in packaged with rss-glx.
AC_CHECK_HEADER([GL/gl.h])

if test "x$ac_cv_header_GL_gl_h" = "xno" ; then
	CFLAGS="$CFLAGS -I/usr/openwin/share/include"
	CPPFLAGS="$CPPFLAGS -I/usr/openwin/share/include"

	echo "Hrm, maybe GL/gl.h is in /usr/openwin/share/include";
	unset ac_cv_header_GL_gl_h
	AC_CHECK_HEADER(GL/gl.h,,AC_MSG_ERROR([[No gl.h found.]]))
fi

AC_CHECK_HEADER([GL/glx.h],,AC_MSG_ERROR([[No glx.h found.]]))

AC_CHECK_LIB([GL],[glNewList])
if test "x$ac_cv_lib_GL_glNewList" = "xno" ; then
        dnl Check for MesaGL.
        AC_CHECK_LIB([MesaGL],[glNewList],,AC_MSG_ERROR(GL library was not found.))
fi

# Check for GLU headers.
AC_CHECK_HEADER(GL/glu.h,,AC_MSG_ERROR([[No glu.h found.]]))

AC_CHECK_LIB([GLU],[gluLookAt])
if test "x$ac_cv_lib_GLU_gluLookAt" = "xno"; then
        dnl Check for MesaGLU.
        AC_CHECK_LIB([MesaGLU],[gluLookAt],,AC_MSG_ERROR(GLU library was not found.))
fi

# check for freeglut


AC_PROG_INSTALL

AC_PROG_MAKE_SET

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT

