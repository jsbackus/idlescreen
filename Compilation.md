# Introduction #

This page contains instructions and details on how to compile the screen savers, including
installation of the requisite libraries.

There are two versions of each screen saver, the actual screen saver and the GLUT version which supports features like screen shots, and doesn't go away on the first key press.  Both versions will require the Qt development libraries available from [Trolltech](http://www.trolltech.com) and the OpenGL libraries, which should already be installed.  If not, install the latest version of your video card's drivers from the manufacturer, **NOT** Microsoft.  The GLUT versions will also require a copy of [the GLUT](http://www.opengl.org/resources/libraries/glut) or [FreeGLUT](http://freeglut.sourceforge.net/).  Currently, the Windows versions do not work with the FreeGLUT, though that will be fixed in the near future.  Most people will not need to compile the GLUT version anyway.

For specific instructions for your operating system, please see below.


# Windows 2000/XP/Vista #

To compile the IdleScreen Project on a Windows machine with Microsoft Visual Studio, you will first need to install the latest version of Qt 4.x.  You'll have to Google for instructions on how to use Qt with MSVC.  Currently, the project has only been compiled with MSVC, but it should work with MinGW.  In the future, I plan to add in support for MinGW.  After installing and/or compiling Qt, set the environment variable QTDIR to the base path of the Qt install.  In other words, if you install Qt to C:\Qt\4.4.0 (which means the libraries are in C:\Qt\4.4.0\lib, binaries in C:\Qt\4.4.0\bin, and headers in C:\Qt\4.4.0\include), QTDIR should be set to C:\Qt\4.4.0\.  To build the installer you will also need to install [NSIS](http://nsis.sourceforge.net/Main_Page).  Next, download the source, either from the [downloads page](http://code.google.com/p/idlescreen/downloads/list) or from the [Subversion repository](http://code.google.com/p/idlescreen/source/checkout).  There is a MSVC solution for each screen saver, which should include the projects for the GLUT and screen saver("scr") versions, as well as the redirector, which is a hack to get around Windows' goofy screen saver mechanism.  There is also an "uber solution" that contains the projects for each screen saver, it's redirector, and the installer.  You should be able to build the scr and redirector without much problem.  Once built, copy the redirector for each screen saver to %WINDOWS%\system32 and the scr target + [Qt DLLs and plugins](Qt.md) to an appropriate install directory, rename the scr target so that it ends in .scr instead of .exe, then set up the appropriate [registry settings](Win32Registry.md).  Viola!  You should be able to manipulate the screen saver like other screen savers from the Screen Saver tab of the Display Settings dialog box.  The screen saver will create a suitable configuration file on first run, or you can check out the Wiki page on [config files](ConfigFiles.md) for more information.


# Linux/FreeBSD/Mac OS X #

First off, make sure that [X-Screensaver](http://www.jwz.org/xscreensaver/) and the latest Qt 4.x **development** libraries have been installed.  To compile from the repository, you will need GNU's autoconf installed.  To build the GLUT version, you will also need to install the [FreeGLUT](http://freeglut.sourceforge.net/) libraries.  Next, download the source, either from the [downloads page](http://code.google.com/p/idlescreen/downloads/list) or from the [Subversion repository](http://code.google.com/p/idlescreen/source/checkout).  After downloading and unpacking (if using the TAR-ball) the source, change to the root project directory.  If you got the source from the repository, first execute autoconf.  In either case, now do the following:
```
./configure
make
make install
```

You may need to sudo the "make install" for everyone one the system to use it and for the XScreenSaver system files to be updated.  If you just install to your home directory, ~/.xscreensaver will need to be updated.  The only [command-line option](CommandLineOptions.md) that you should need is "-root".  Be sure to set them up as OpenGL screen savers.  Please refer to the XScreenSaver documentation for more information on how to update ~/.xscreensaver.  To access the configuration dialog of the screen saver, you can execute the binary with the command-line option "-c".  For instance, to configure PlasmaLenz:
```
PlasmaLenz -c &
```
You can pass any Qt command-line options here as well.


# Need More Information? #

Please look in the FAQs then try the mailing lists for help.