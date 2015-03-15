# Introduction #

Qt is a cross-platform framework.  The Idle Screen project uses it mainly for its GUI and XML libraries, however it is a core part of the configuration side of all of the programs that are part of Idle Screen.  Qt is available under both commercial and open source licenses.  You can obtain more information and the Qt source code from the [Trolltech website](http://www.trolltech.com).

In order to run any of the programs in the Idle Screen project on a Windows machine, all of the requisite DLLs **must** be in the same path as the screen saver and GLUT demo executables.  This is why the redirector framework is required.  The release version of the requisite DLL files are:
  * QtCore4.dll
  * QtGui4.dll
  * QtXml4.dll
  * qgif4.dll
  * qjpeg4.dll
  * qmng4.dll
  * qsvg4.dll

The debug versions simply have a 'd' appended to the root filename.  i.e. QtCore4d.dll.  On `*`NIX systems, the libraries just have to be in the search path for the dynamic linker.