TEMPLATE	= app
win32:CONFIG	+= qt warn_on
unix:CONFIG	+= qt x11 warn_on
QT			+=	xml

unix:COMMON_PATH = ../../common
win32:COMMON_PATH = ../../../common

unix:SOURCE_PATH = ../src
win32:SOURCE_PATH = ../../src

unix:RESOURCE_PATH = ../resource_files
win32:RESOURCE_PATH = ../../resource_files

INCLUDEPATH = $$COMMON_PATH $$SOURCE_PATH $$RESOURCE_PATH

SOURCES     =   $$COMMON_PATH/core/win_misc_funcs.cpp
SOURCES		+=	$$COMMON_PATH/gpl_related/gpldialog.cpp
SOURCES		+=	$$COMMON_PATH/utility/misc_funcs.cpp
SOURCES		+=	$$COMMON_PATH/utility/AboutDialog.cpp

SOURCES		+=	$$SOURCE_PATH/ConfigManager.cpp $$SOURCE_PATH/ConfigWidget.cpp
SOURCES		+=	$$SOURCE_PATH/HistoryList.cpp $$SOURCE_PATH/SaverRunner.cpp
SOURCES		+=	$$SOURCE_PATH/SearchPathDialog.cpp $$SOURCE_PATH/main.cpp
SOURCES		+=	$$SOURCE_PATH/globaldefs.cpp

HEADERS		=	$$COMMON_PATH/core/win_misc_funcs.h
HEADERS		+=	$$COMMON_PATH/utility/misc_funcs.h
HEADERS		+=	$$COMMON_PATH/gpl_related/gpldialog.h
HEADERS		+=	$$COMMON_PATH/utility/AboutDialog.h

HEADERS		+=	$$RESOURCE_PATH/resource.h

HEADERS		+=	$$SOURCE_PATH/ConfigManager.h $$SOURCE_PATH/ConfigWidget.h
HEADERS		+=	$$SOURCE_PATH/HistoryList.h $$SOURCE_PATH/SaverRunner.h
HEADERS		+=	$$SOURCE_PATH/SearchPathDialog.h $$SOURCE_PATH/globaldefs.h

TARGET		= Randomizer
RESOURCES	= $$RESOURCE_PATH/qt_resource.qrc

win32:LIBS	+= user32.lib shell32.lib Advapi32.lib gdi32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib scr.res

unix:DESTDIR	= ../../../bin
unix:OBJECTS_DIR = ../compiled_objects
unix:MOC_DIR = ../compiled_objects
unix:LIBS	+= -lGL -lGLU -lX11 -lXmu -lXi -lm
