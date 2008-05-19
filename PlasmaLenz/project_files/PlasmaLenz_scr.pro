TEMPLATE	= app
win32:CONFIG		+= qt warn_on
unix:CONFIG	+= qt x11 warn_on
QT			+=	xml

SOURCES		=	../../../common/core/win_main.cpp ../../../common/core/win_misc_funcs.cpp

SOURCES		+=	../../../common/IndexedPalette/IndexedPalette.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteDialog.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteDialogEntryButton.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteEditorWidget.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidget.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidgetItem.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidgetSplitDlg.cpp
SOURCES		+=	../../../common/IndexedPalette/IndexedPaletteProfile.cpp
SOURCES		+=	../../../common/utility/misc_funcs.cpp
SOURCES		+=	../../../common/utility/ImportExportDialog.cpp
SOURCES		+=	../../../common/gpl_related/gpldialog.cpp
SOURCES		+=	../../../common/plasma/PlasmaAlgorithm.cpp

SOURCES		+=	../../../common/2d_bgnd_w_lens/opengl_main.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/Background.cpp ../../../common/2d_bgnd_w_lens/BackgroundProfile.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/configdlg_main.cpp ../../../common/2d_bgnd_w_lens/ConfigManager.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/IndexedPaletteBackground.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/MasterProfile.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/PaletteEditor.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/PaletteSelectWidget.cpp

SOURCES		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensProfile.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/lens_engine/GenericLensObject.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensObject.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensManager.cpp
SOURCES		+=	../../../common/2d_bgnd_w_lens/lens_engine/SphericalLensProfile.cpp

SOURCES		+=	../../src/ConfigWidget.cpp ../../src/PlasmaFractal.cpp
SOURCES		+=	../../src/PlasmaFractalBackgroundProfile.cpp ../../src/project_specific_externs.cpp
SOURCES		+=	../../src/AboutDialog.cpp
SOURCES		+=	../../src/ProfileEditDialog.cpp

HEADERS		=	../../../common/core/win_misc_funcs.h

HEADERS		+=	../../../common/IndexedPalette/IndexedPalette.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteDialog.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteDialogEntryButton.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteEditorWidget.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidget.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidgetItem.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteEditorTableWidgetSplitDlg.h
HEADERS		+=	../../../common/IndexedPalette/IndexedPaletteProfile.h
HEADERS		+=	../../../common/utility/misc_funcs.h
HEADERS		+=	../../../common/utility/ImportExportDialog.h
HEADERS		+=	../../../common/gpl_related/gpldialog.h
HEADERS		+=	../../../common/plasma/PlasmaAlgorithm.h
HEADERS		+=	../../../common/core/opengl_main.h

HEADERS		+=	../../../common/2d_bgnd_w_lens/Background.h ../../../common/2d_bgnd_w_lens/BackgroundProfile.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/ConfigManager.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/IndexedPaletteBackground.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/MasterProfile.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/globaldefs.h 
HEADERS		+=	../../../common/2d_bgnd_w_lens/ConfigWidget.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/project_specific_extern_defs.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/PaletteEditor.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/PaletteSelectWidget.h

HEADERS		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensProfile.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/lens_engine/GenericLensObject.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensObject.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/lens_engine/LensManager.h
HEADERS		+=	../../../common/2d_bgnd_w_lens/lens_engine/SphericalLensProfile.h

HEADERS		+=	../../src/PlasmaFractalBackgroundProfile.h ../../src/PlasmaFractal.h
HEADERS		+=	../../src/AboutDialog.h
HEADERS		+=	../../resource_files/resource.h
HEADERS		+=	../../src/ProfileEditDialog.h

TARGET		= PlasmaLenz
RC_FILE		= ../../resource_files/win_resource.rc
RESOURCES	= ../../resource_files/qt_resource.qrc

win32:LIBS	+= user32.lib shell32.lib Advapi32.lib gdi32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib
//win32:LIBS	+=  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib

unix:DESTDIR	= ../../..
unix:LIBS	+= GL GLU X11 Xmu Xi m
