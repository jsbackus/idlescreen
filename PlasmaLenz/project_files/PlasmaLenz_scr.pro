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

win32:SOURCES	=	$$COMMON_PATH/core/win_main.cpp
unix:SOURCES    =       $$COMMON_PATH/core/unix_main.cpp
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
SOURCES		+=	$$COMMON_PATH/gpl_related/gpldialog.cpp
SOURCES		+=	$$COMMON_PATH/plasma/PlasmaAlgorithm.cpp

SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/opengl_main.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/Background.cpp $$COMMON_PATH/2d_bgnd_w_lens/BackgroundProfile.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/configdlg_main.cpp $$COMMON_PATH/2d_bgnd_w_lens/ConfigManager.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/IndexedPaletteBackground.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/MasterProfile.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/PaletteEditor.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/PaletteSelectWidget.cpp

SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensProfile.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/GenericLensObject.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensObject.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensManager.cpp
SOURCES		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/SphericalLensProfile.cpp

SOURCES		+=	$$SOURCE_PATH/ConfigWidget.cpp $$SOURCE_PATH/PlasmaFractal.cpp
SOURCES		+=	$$SOURCE_PATH/PlasmaFractalBackgroundProfile.cpp $$SOURCE_PATH/project_specific_externs.cpp
SOURCES		+=	$$SOURCE_PATH/AboutDialog.cpp
SOURCES		+=	$$SOURCE_PATH/ProfileEditDialog.cpp

HEADERS		=	$$COMMON_PATH/core/win_misc_funcs.h

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
HEADERS		+=	$$COMMON_PATH/gpl_related/gpldialog.h
HEADERS		+=	$$COMMON_PATH/plasma/PlasmaAlgorithm.h
HEADERS		+=	$$COMMON_PATH/core/opengl_main.h

HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/Background.h $$COMMON_PATH/2d_bgnd_w_lens/BackgroundProfile.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/ConfigManager.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/IndexedPaletteBackground.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/MasterProfile.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/globaldefs.h 
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/ConfigWidget.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/project_specific_extern_defs.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/PaletteEditor.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/PaletteSelectWidget.h

HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensProfile.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/GenericLensObject.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensObject.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/LensManager.h
HEADERS		+=	$$COMMON_PATH/2d_bgnd_w_lens/lens_engine/SphericalLensProfile.h

HEADERS		+=	$$SOURCE_PATH/PlasmaFractalBackgroundProfile.h $$SOURCE_PATH/PlasmaFractal.h
HEADERS		+=	$$RESOURCE_PATH/resource.h
HEADERS		+=	$$SOURCE_PATH/AboutDialog.h
HEADERS		+=	$$SOURCE_PATH/ProfileEditDialog.h

TARGET		= PlasmaLenz
RC_FILE		= $$RESOURCE_PATH/win_resource.rc
RESOURCES	= $$RESOURCE_PATH/qt_resource.qrc

win32:LIBS	+= user32.lib shell32.lib Advapi32.lib gdi32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib
//win32:LIBS	+=  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib

unix:DESTDIR	= ../../bin
unix:OBJECTS_DIR = ../compiled_objects
unix:MOC_DIR = ../compiled_objects
unix:LIBS	+= -lGL -lGLU -lX11 -lXmu -lXi -lm
