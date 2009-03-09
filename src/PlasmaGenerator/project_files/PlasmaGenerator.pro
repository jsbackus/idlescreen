TEMPLATE	= app
win32:CONFIG	+= qt warn_on
unix:CONFIG	+= qt x11 warn_on release
QT		+= xml

unix:COMMON_PATH = ../../common
win32:COMMON_PATH = ../../../common

unix:SOURCE_PATH = ../src
win32:SOURCE_PATH = ../../src

unix:RESOURCE_PATH = ../resource_files
win32:RESOURCE_PATH = ../../resource_files

INCLUDEPATH = $$COMMON_PATH $$SOURCE_PATH $$RESOURCE_PATH

SOURCES		=	$$SOURCE_PATH/PlasmaGenerator.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaGeneratorMain.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaGeneratorProfile.cpp
SOURCES		+=	$$SOURCE_PATH/ConfigManager.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaGeneratorDialog.cpp
SOURCES		+=	$$SOURCE_PATH/project_specific_externs.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaGeneratorPaletteEditor.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaGeneratorPaletteSelectWidget.cpp

SOURCES         +=      $$COMMON_PATH/core/win_misc_funcs.cpp

SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPalette.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteDialog.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteDialogEntryButton.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorWidget.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidget.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidgetItem.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidgetSplitDlg.cpp
SOURCES		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteProfile.cpp
SOURCES		+=	$$COMMON_PATH/utility/misc_funcs.cpp
SOURCES		+=	$$COMMON_PATH/utility/ImportExportDialog.cpp
SOURCES		+=	$$COMMON_PATH/utility/HelpDialog.cpp
SOURCES		+=	$$COMMON_PATH/gpl_related/gpldialog.cpp
SOURCES		+=	$$COMMON_PATH/plasma/PlasmaAlgorithm.cpp

SOURCES		+=	$$COMMON_PATH/utility/AboutDialog.cpp

HEADERS		=	$$SOURCE_PATH/ConfigManager.h
HEADERS		+=	$$SOURCE_PATH/PlasmaGenerator.h
HEADERS		+=	$$SOURCE_PATH/PlasmaGeneratorProfile.h
HEADERS		+=	$$SOURCE_PATH/PlasmaGeneratorDialog.h
HEADERS		+=	$$SOURCE_PATH/PlasmaGeneratorPaletteEditor.h
HEADERS		+=	$$SOURCE_PATH/PlasmaGeneratorPaletteSelectWidget.h

HEADERS		+=	$$COMMON_PATH/core/win_misc_funcs.h

HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPalette.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteDialog.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteDialogEntryButton.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorWidget.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidget.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidgetItem.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteEditorTableWidgetSplitDlg.h
HEADERS		+=	$$COMMON_PATH/IndexedPalette/IndexedPaletteProfile.h
HEADERS		+=	$$COMMON_PATH/utility/misc_funcs.h
HEADERS		+=	$$COMMON_PATH/utility/ImportExportDialog.h
HEADERS		+=	$$COMMON_PATH/utility/HelpDialog.h
HEADERS		+=	$$COMMON_PATH/gpl_related/gpldialog.h
HEADERS		+=	$$COMMON_PATH/plasma/PlasmaAlgorithm.h

HEADERS		+=	$$RESOURCE_PATH/resource.h
HEADERS		+=	$$COMMON_PATH/utility/AboutDialog.h

TARGET		= 	PlasmaGenerator
RESOURCES	= 	$$RESOURCE_PATH/qt_resource.qrc

win32:LIBS      += user32.lib shell32.lib Advapi32.lib gdi32.lib comctl32.lib $$IDLSCR_RES_LOC

unix:DESTDIR	= ../../../bin
unix:OBJECTS_DIR = ../compiled_objects
unix:MOC_DIR = ../compiled_objects
unix:LIBS	+= -lX11 -lXmu -lXi -lm
