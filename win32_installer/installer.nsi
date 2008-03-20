# Globals
!define VERSION "0.2"
Name "Idle Screen Installer ${VERSION}"
OutFile "idlescreen_${VERSION}.exe"
SetCompressor /FINAL lzma
Icon "installer_icon.ico"

# variables
Var TARGET_DIR
Var REDIRECTOR_TARGET
Var DATA_DIR
Var UNINST_NAME
#Var QTDIR

# Functions
Function .onInit
  StrCpy $TARGET_DIR "$PROGRAMFILES\Blargh"
  StrCpy $REDIRECTOR_TARGET "$SYSDIR"
  StrCpy $DATA_DIR "$APPDATA\idlescreen_inst"
  StrCpy $UNINST_NAME "idlescreen_uninstaller.exe"
  #ExpandEnvStrings $QTDIR "%QTDIR%"
FunctionEnd

# page definitions
PageEx license
  LicenseText "The Idle Screen Project"
  LicenseData "..\common\gpl_related\gplv2.txt"
  LicenseForceSelection checkbox
  #Caption "This is the caption"
PageExEnd

PageEx components
  #Caption "Please selection something to install - PLEASE!"
PageExEnd

PageEx directory
  DirVar $TARGET_DIR
PageExEnd

PageEx instfiles
PageExEnd

#UninstPage uninstConfirm
#UninstPageEnd

#UninstPage instfiles
#UninstPageEnd

# section definitions
section "Base Files"
  #ExpandEnvStrings $QTDIR "%QTDIR%"
  SectionIn 1 RO
  MessageBox MB_OK "The installer will now install the required Qt libraries."

  # general docs
  SetOutPath "$TARGET_DIR\doc"
  File "..\common\gpl_related\gplv2.txt"

  SetOutPath "$TARGET_DIR\bin"
  # include files
  File "${QTDIR}\lib\QtCore4.dll"
  File "${QTDIR}\lib\QtGui4.dll"
  File "${QTDIR}\lib\QtXml4.dll"

  # plug ins
  File "${QTDIR}\plugins\imageformats\qgif1.dll"
  File "${QTDIR}\plugins\imageformats\qjpeg1.dll"
  File "${QTDIR}\plugins\imageformats\qmng1.dll"
  File "${QTDIR}\plugins\imageformats\qsvg1.dll"

  # uninstaller
  WriteUninstaller $TARGET_DIR\$UNINST_NAME
sectionEnd

SectionGroup "Screen Savers"
Section "PlasmaLenz"
  # main scr file
  SetOutPath "$TARGET_DIR\bin"
  File /oname=PlasmaLenz.scr "..\PlasmaLenz\msvc\scr\release\PlasmaLenz.exe"

  # redirector file
  SetOutPath "$REDIRECTOR_TARGET"
  File /oname=PlasmaLenz.scr "..\PlasmaLenz\msvc\redirector\Release\redirector.exe"

  # registry entry for the redirector
  WriteRegStr SHCTX "Software\IdleScreen\Redirector" "PlasmaLenz" "$TARGET_DIR\bin"
SectionEnd
SectionGroupEnd

#SectionGroup "Tools/Utilities"

#SectionGroupEnd

# uninstaller sections
#Section "un.PlasmaLenz"

  #remove files
#  Delete $INSTDIR\bin\PlasmaLenz.scr
#  Delete $REDIRECTOR_TARGET\PlasmaLenz.scr

#SectionEnd

Section "Uninstall"
  StrCpy $REDIRECTOR_TARGET "$SYSDIR"

  MessageBox MB_YESNO "Are you sure you want to uninstall the Idle Screen Project?" /SD IDYES IDNO dontremove

  #remove reg keys
  #DeleteRegValue SHCTX "Software\IdleScreen\Redirector" "PlasmaLenz"
  DeleteRegKey SHCTX "Software\IdleScreen"

  # delete files and directories
  Delete $REDIRECTOR_TARGET\PlasmaLenz.scr
  Delete "$INSTDIR\doc\*"
  RMDir $INSTDIR\doc
  Delete "$INSTDIR\bin\*"
  RMDir $INSTDIR\bin
  Delete "$INSTDIR\$UNINST_NAME"
  Delete "$INSTDIR\*"  #don't know why this is necessary...
  RMDir $INSTDIR
  dontremove:
SectionEnd
