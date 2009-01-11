TEMPLATE	= app
CONFIG		+= qt warn_on
QT			+=	xml

SOURCES		=	../common/win_main.cpp
SOURCES		+=	opengl_main.cpp configdlg_main.cpp LensManager.cpp ConfigManager.cpp MasterProfile.cpp

SOURCES		+=	../common/misc_funcs.cpp ../common/IndexedPaletteProfile.cpp ../common/win_misc_funcs.cpp
SOURCES		+=	../common/IndexedPalette.cpp ../common/IndexedPaletteDialog.cpp
SOURCES		+=	../common/IndexedPaletteDialogEntryButton.cpp ../common/IndexedPaletteEditorWidget.cpp

SOURCES		+=	backgrounds/Background.cpp backgrounds/ImageBackground.cpp
SOURCES		+=	backgrounds/IndexedPaletteBackground.cpp backgrounds/PlasmaFractal.cpp
SOURCES		+=	backgrounds/ImageBackgroundProfile.cpp backgrounds/PlasmaFractalBackgroundProfile.cpp
SOURCES		+=	backgrounds/BackgroundProfile.cpp

SOURCES		+=	lenses/LensObject.cpp lenses/CircularLensProfile.cpp lenses/SphericalLensProfile.cpp
SOURCES		+=	lenses/GenericLensObject.cpp lenses/LensProfile.cpp lenses/RectangularLensProfile.cpp
SOURCES		+=	lenses/TriangularLensProfile.cpp lenses/LensCollectionProfile.cpp
SOURCES		+=	lenses/DiamondLensProfile.cpp

SOURCES		+=	../common/gpldialog.cpp ConfigWidget.cpp gui/PaletteEditor.cpp gui/MasterProfileEditDialog.cpp
SOURCES		+=	gui/BackgroundProfileEditDialog.cpp gui/BackgroundProfileEditWidget.cpp
SOURCES		+=	gui/PreviewWidget.cpp gui/PlasmaFractalBackgroundWidget.cpp gui/ImageBackgroundWidget.cpp
SOURCES		+=	gui/BackgroundSelectWidget.cpp gui/LensSelectWidget.cpp gui/PaletteSelectWidget.cpp
SOURCES		+=	gui/BackgroundEditor.cpp gui/LensEditor.cpp

HEADERS		=	globaldefs.h resource.h LensManager.h ConfigManager.h MasterProfile.h

HEADERS		+=	../common/misc_funcs.h ../common/IndexedPaletteProfile.h
HEADERS		+=  ../common/opengl_main.h ../common/configdlg_main.h
HEADERS		+=	../common/win_misc_funcs.cpp ../common/IndexedPalette.h ../common/IndexedPaletteDialog.h
HEADERS		+=	../common/IndexedPaletteDialogEntryButton.h ../common/IndexedPaletteEditorWidget.h

HEADERS		+=	backgrounds/Background.h backgrounds/ImageBackground.h
HEADERS		+=	backgrounds/IndexedPaletteBackground.h backgrounds/PlasmaFractal.h
HEADERS		+=	backgrounds/ImageBackgroundProfile.h backgrounds/PlasmaFractalBackgroundProfile.h
HEADERS		+=	backgrounds/BackgroundProfile.h

HEADERS		+=	lenses/LensObject.h lenses/CircularLensProfile.h lenses/SphericalLensProfile.h
HEADERS		+=	lenses/GenericLensObject.h lenses/LensProfile.h lenses/RectangularLensProfile.h
HEADERS		+=	lenses/TriangularLensProfile.h lenses/LensCollectionProfile.h
HEADERS		+=	lenses/DiamondLensProfile.h

HEADERS		+=	../common/gpldialog.h ConfigWidget.h gui/PaletteEditor.h gui/MasterProfileEditDialog.h
HEADERS		+=	gui/BackgroundProfileEditDialog.h gui/BackgroundProfileEditWidget.h
HEADERS		+=	gui/PreviewWidget.h gui/PlasmaFractalBackgroundWidget.h gui/ImageBackgroundWidget.h
HEADERS		+=	gui/BackgroundSelectWidget.h gui/LensSelectWidget.h gui/PaletteSelectWidget.h
HEADERS		+=	gui/BackgroundEditor.h gui/LensEditor.h

TARGET		= Fractal_Lenz
RC_FILE		= win_resource.rc
RESOURCES	= qt_resource.qrc

win32:LIBS	+= user32.lib shell32.lib Advapi32.lib gdi32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib
//win32:LIBS	+=  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib scrnsavw.lib opengl32.lib glu32.lib comctl32.lib
