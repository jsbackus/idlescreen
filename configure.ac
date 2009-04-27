#"compile" w/:
#autoconf -o configure configure.ac
#chmod +x configure
AC_INIT(IdleScreen, version-0.4)

AC_COPYRIGHT([Copyright (c) 2008 Jeff Backus and the IdleScreen Project.
http://sites.google.com/site/idlescreenproject  Released under the GPLv2.])

AC_REVISION([$Revision 1.00$])

# Test validity of srcdir by looking for the MSVC++ solution.
# Note, this file is not necessary for compiling on *NIX systems!
AC_CONFIG_SRCDIR([idlescreen_msvc.sln])

AC_PROG_CXX

# check for update-xscreensaver-hacks
AC_PATH_PROG(UPDATEXSCR, update-xscreensaver-hacks,
			 [@echo ""; echo ""; \
			 echo 'Please run update-xscreensaver-hacks!'])

# check for qmake and check its version
AC_PATH_PROGS(QMAKEBIN, [qmake-qt4 qmake])

#this should check version of Qmake, but it only works in Autoconf >2.62
#AC_CACHE_CHECK([for Qt], [ac_cv_path_QMAKE],
#		    [AC_PATH_PROGS_FEATURE_CHECK([QMAKEBIN], [qmake-qt4 qmake],
#		    [[qmakeout=`$ac_path_QMAKEBIN --version | grep "Qt" | \
#		    sed 's/.*\([0-9]*\)\.\([0-9]*\)\.\([0-9*]\).*/\1.\2.\3/'`
#		    test "x$qmakeout" = x0 \
#		    && ac_cv_path_QMAKEBIN=$ac_path_QMAKEBIN \
#		    ac_path_QMAKEBIN_found=:]],
#		    [AC_MSG_ERROR([could not find qmake for Qt 4!])])])
#AC_SUBST([QMAKEBIN], [$ac_cv_path_QMAKEBIN])

if test "yes" == "yes";
   then enable_glut=no
fi

# enable/disable Qt build targets
# have to do this first, so that we only check for the GLUT
# headers if we are compiling anything that uses GLUT.
AC_ARG_ENABLE([plasmalenz], [AC_HELP_STRING([--enable-plasmalenz],
		      [build PlasmaLenz screen saver default=yes])],
		      [enable_plasmalenz=$enableval],
		      [enable_plasmalenz=yes])

if test "$enable_plasmalenz" == "yes";
   then 
   	[qt_targets="$qt_targets PlasmaLenz"]
	[data_targets="$data_targets PlasmaLenz.xml"]
	[man_targets="$man_targets PlasmaLenz/man_page/PlasmaLenz.1"]
fi

AC_ARG_ENABLE([plasmalenz-glut], [AC_HELP_STRING([--enable-plasmalenz-glut],
		      [build PlasmaLenz GLUT demo default=no])],
		      [enable_plasmalenz_glut=$enableval],
		      [enable_plasmalenz_glut=no])

if test "$enable_plasmalenz_glut" == "yes";
   then
	[qt_targets="$qt_targets PlasmaLenz_glut"]
   	[enable_glut="yes"]
	[data_targets="$data_targets PlasmaLenz.xml"]
	[man_targets="$man_targets PlasmaLenz/man_page/PlasmaLenz.1"]
fi

AC_ARG_ENABLE([crawlies], [AC_HELP_STRING([--enable-crawlies],
		      [build Crawlies screen saver default=yes])],
		      [enable_crawlies=$enableval],
		      [enable_crawlies=yes])

if test "$enable_crawlies" == "yes";
   then 
   	[qt_targets="$qt_targets Crawlies"]
	[data_targets="$data_targets Crawlies.xml"]
	[man_targets="$man_targets Crawlies/man_page/Crawlies.1"]
fi

AC_ARG_ENABLE([crawlies-glut], [AC_HELP_STRING([--enable-crawlies-glut],
		      [build Crawlies GLUT demo default=no])],
		      [enable_crawlies_glut=$enableval],
		      [enable_crawlies_glut=no])

if test "$enable_crawlies_glut" == "yes";
   then
	[qt_targets="$qt_targets Crawlies_glut"]
   	[enable_glut="yes"]
	[data_targets="$data_targets Crawlies.xml"]
	[man_targets="$man_targets Crawlies/man_page/Crawlies.1"]
fi

AC_ARG_ENABLE([acidrain], [AC_HELP_STRING([--enable-acidrain],
		      [build AcidRain screen saver default=yes])],
		      [enable_acidrain=$enableval],
		      [enable_acidrain=yes])

if test "$enable_acidrain" == "yes";
   then 
   	[qt_targets="$qt_targets AcidRain"]
	[data_targets="$data_targets AcidRain.xml"]
	[man_targets="$man_targets AcidRain/man_page/AcidRain.1"]
fi

AC_ARG_ENABLE([acidrain-glut], [AC_HELP_STRING([--enable-acidrain-glut],
		      [build AcidRain GLUT demo default=no])],
		      [enable_acidrain_glut=$enableval],
		      [enable_acidrain_glut=no])

if test "$enable_acidrain_glut" == "yes";
   then
	[qt_targets="$qt_targets AcidRain_glut"]
   	[enable_glut="yes"]
	[data_targets="$data_targets AcidRain.xml"]
	[man_targets="$man_targets AcidRain/man_page/AcidRain.1"]
fi

AC_ARG_ENABLE([plasmagenerator], [AC_HELP_STRING([--enable-plasmagenerator],
		      [build PlasmaGenerator utility default=no])],
		      [enable_plasmagenerator=$enableval],
		      [enable_plasmagenerator=no])

if test "$enable_plasmagenerator" == "yes";
   then 
   	[qt_targets="$qt_targets PlasmaGenerator"]
	[data_targets="$data_targets PlasmaGenerator.xml"]
	[man_targets="$man_targets PlasmaGenerator/man_page/PlasmaGenerator.1"]
fi

AC_ARG_ENABLE([spirallenz], [AC_HELP_STRING([--enable-spirallenz],
		      [build SpiralLenz screen saver default=yes])],
		      [enable_spirallenz=$enableval],
		      [enable_spirallenz=yes])

if test "$enable_spirallenz" == "yes";
   then 
   	[qt_targets="$qt_targets SpiralLenz"]
	[data_targets="$data_targets SpiralLenz.xml"]
	[man_targets="$man_targets SpiralLenz/man_page/SpiralLenz.1"]
fi

AC_ARG_ENABLE([spirallenz-glut], [AC_HELP_STRING([--enable-spirallenz-glut],
		      [build SpiralLenz GLUT demo default=no])],
		      [enable_spirallenz_glut=$enableval],
		      [enable_spirallenz_glut=no])

if test "$enable_spirallenz_glut" == "yes";
   then
	[qt_targets="$qt_targets SpiralLenz_glut"]
   	[enable_glut="yes"]
	[data_targets="$data_targets SpiralLenz.xml"]
	[man_targets="$man_targets SpiralLenz/man_page/SpiralLenz.1"]
fi


# substitute in targets
AC_SUBST(bintrg, $qt_targets)
AC_SUBST(datatrg, $data_targets)
AC_SUBST(mantrg, $man_targets)

# check for doxygen
AC_ARG_ENABLE([doxygen], [AC_HELP_STRING([--enable-doxygen],
		      [build class documentation with Doxygen default=no])],
		      [enable_doxygen=$enableval],
		      [enable_doxygen=no])

if test "$enable_doxygen" == "yes";
    then 
    	 AC_PATH_PROGS(doxygen, [doxygen])
	 AC_SUBST(othrtrg, [doc])
    else AC_SUBST(doxygen, [doxygen])
fi

# check for necessary headers

#math.h
AC_CHECK_HEADER(math.h,,AC_MSG_ERROR([No math.h found.]))
#stdio.h
AC_CHECK_HEADER(stdio.h,,AC_MSG_ERROR([No stdio.h found.]))
#signal.h
AC_CHECK_HEADER(signal.h,,AC_MSG_ERROR([No signal.h found.]))
#time.h
AC_CHECK_HEADER(time.h,,AC_MSG_ERROR([No time.h found.]))
#sys/time.h
AC_CHECK_HEADER(sys/time.h,,AC_MSG_ERROR([No sys/time.h found.]))
#X11/X.h
AC_CHECK_HEADER(X11/X.h,,AC_MSG_ERROR([No X11/X.h found.]))
#X11/Xlib.h
AC_CHECK_HEADER(X11/Xlib.h,,AC_MSG_ERROR([No X11/Xlib.h found.]))
#X11/keysym.h
AC_CHECK_HEADER(X11/keysym.h,,AC_MSG_ERROR([No X11/keysym.h found.]))
#X11/Intrinsic.h
AC_CHECK_HEADER(X11/Intrinsic.h,,AC_MSG_ERROR([No X11/Intrinsic.h found.]))


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
if test "$enable_glut" == "yes";
   then
	#AC_CHECK_HEADER([GL/glut.h GL/freeglut_std.h],,
	AC_CHECK_HEADER([GL/glut.h],,
				   AC_MSG_ERROR([FreeGLUT not found!]))
	AC_CHECK_HEADER([GL/freeglut_std.h],,
				   AC_MSG_ERROR([FreeGLUT not found!]))
fi

AC_PROG_INSTALL

AC_PROG_MAKE_SET

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT

