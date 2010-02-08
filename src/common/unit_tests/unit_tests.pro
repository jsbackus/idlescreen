TEMPLATE = app
CONFIG += qt qtestlib warn_on
#win32:CONFIG	+=
unix:CONFIG	+= x11 release
QT		+=	xml

DEPENDPATH += .
INCLUDEPATH += .

unix:COMMON_PATH = ..
win32:COMMON_PATH = ..

#unix:SOURCE_PATH = ../src
#win32:SOURCE_PATH = ../../src

#unix:RESOURCE_PATH = ../resource_files
#win32:RESOURCE_PATH = ../../resource_files

INCLUDEPATH = $$COMMON_PATH #$$SOURCE_PATH $$RESOURCE_PATH

# Input
SOURCES  = unit_tests_main.cpp
SOURCES += $$COMMON_PATH/utility/misc_funcs.cpp
SOURCES += $$COMMON_PATH/utility/unit_tests/Test_MiscFuncs.cpp
SOURCES += $$COMMON_PATH/utility/Point2D.cpp
SOURCES += $$COMMON_PATH/utility/unit_tests/Test_Point2D.cpp
SOURCES += $$COMMON_PATH/utility/Vector2D.cpp
SOURCES += $$COMMON_PATH/utility/unit_tests/Test_Vector2D.cpp

HEADERS  = $$COMMON_PATH/utility/misc_funcs.h
HEADERS += $$COMMON_PATH/utility/unit_tests/Test_MiscFuncs.h
HEADERS += $$COMMON_PATH/utility/Point2D.h
HEADERS += $$COMMON_PATH/utility/unit_tests/Test_Point2D.h
HEADERS += $$COMMON_PATH/utility/Vector2D.h
HEADERS += $$COMMON_PATH/utility/unit_tests/Test_Vector2D.h

TARGET		= common_unit_tests
#RC_FILE	= $$RESOURCE_PATH/win_resource.rc
#RESOURCES	= $$RESOURCE_PATH/qt_resource.qrc

win32:LIBS      += user32.lib shell32.lib Advapi32.lib gdi32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib $$IDLSCR_RES_LOC

unix:DESTDIR	= ../../../bin
#unix:OBJECTS_DIR = ../compiled_objects
unix:OBJECTS_DIR = .
#unix:MOC_DIR = ../compiled_objects
unix:MOC_DIR = .
unix:LIBS	+= -lGL -lGLU -lX11 -lXmu -lXi -lm

