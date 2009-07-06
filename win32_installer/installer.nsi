# Globals
!define VERSION_MAJOR "0"
!define VERSION_MINOR "8"
!define VERSION "${VERSION_MAJOR}.${VERSION_MINOR}_Beta"
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
  # attempt to install as admin
  SetShellVarContext all

  #set variables
  StrCpy $TARGET_DIR "C:\IdleScreen"
  StrCpy $REDIRECTOR_TARGET "$SYSDIR"
  StrCpy $DATA_DIR "$APPDATA\idlescreen_inst"
  StrCpy $UNINST_NAME "idlescreen_uninstaller.exe"
  #ExpandEnvStrings $QTDIR "%QTDIR%"
FunctionEnd

# page definitions
PageEx license
  LicenseText "The Idle Screen Project"
  LicenseData "..src\\common\gpl_related\gplv2.txt"
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
  #MessageBox MB_OK "The installer will now install the required Qt libraries."

  #icon
  SetOutPath "$TARGET_DIR"
  File /oname=idlescreen.ico "installer_icon.ico"

  # general docs
  SetOutPath "$TARGET_DIR\doc"
  File "..\src\common\gpl_related\gplv2.txt"

  # uninstaller
  WriteUninstaller $TARGET_DIR\$UNINST_NAME

  # Add registry entry for target path
  WriteRegStr SHCTX "Software\IdleScreen" "InstallPath" "$TARGET_DIR"

  # Add entry to add/remove programs
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"DisplayName" "Idle Screen Project"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"DisplayIcon" "$TARGET_DIR\idlescreen.ico"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"UninstallString" "$TARGET_DIR\$UNINST_NAME"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"InstallLocation" "$TARGET_DIR"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"HelpLink"  "http://sites.google.com/site/idlescreenproject"

  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"URLUpdateInfo" "http://sites.google.com/site/idlescreenproject"

  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"URLInfoAbout" "http://sites.google.com/site/idlescreenproject"

  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"DisplayVersion" ${VERSION}

  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"NoModify" "1"

  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"NoRepair" "1"

  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"VersionMajor" ${VERSION_MAJOR}

  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \
	"VersionMinor" ${VERSION_MINOR}

sectionEnd

Section "Qt Libraries"
  SectionIn 1 RO

  SetOutPath "$TARGET_DIR\bin"
  # include files
  File "${QTDIR}\lib\QtCore4.dll"
  File "${QTDIR}\lib\QtGui4.dll"
  File "${QTDIR}\lib\QtXml4.dll"
  # these files apparently aren't on all windows installations...
  File "${SYSDIR}\msvcp71.dll"
  File "${SYSDIR}\msvcr71.dll"


  # plug ins
  File "${QTDIR}\plugins\imageformats\qgif?.dll"
  File "${QTDIR}\plugins\imageformats\qjpeg?.dll"
  File "${QTDIR}\plugins\imageformats\qmng?.dll"
  File "${QTDIR}\plugins\imageformats\qsvg?.dll"
SectionEnd

SectionGroup "Screen Savers"

#List alphabetically!

Section "AcidRain"
  # main scr file
  SetOutPath "$TARGET_DIR\bin"
  File /oname=AcidRain.scr "..\src\AcidRain\msvc\scr\release\AcidRain.exe"

  # redirector file
  SetOutPath "$REDIRECTOR_TARGET"
  File /oname=AcidRain.scr "..\src\AcidRain\msvc\redirector\Release\redirector.exe"

  # machine defaults file
  SetOutPath "$TARGET_DIR\defaults"
  File /oname=AcidRain.xml "..\config_files\AcidRain.xml"

  # registry entry for the redirector
  WriteRegStr SHCTX "Software\IdleScreen\Redirector" "AcidRain" "$TARGET_DIR\bin\AcidRain.scr"
SectionEnd

Section "Crawlies"
  # main scr file
  SetOutPath "$TARGET_DIR\bin"
  File /oname=Crawlies.scr "..\src\Crawlies\msvc\scr\release\Crawlies.exe"

  # redirector file
  SetOutPath "$REDIRECTOR_TARGET"
  File /oname=Crawlies.scr "..\src\Crawlies\msvc\redirector\Release\redirector.exe"

  # machine defaults file
  SetOutPath "$TARGET_DIR\defaults"
  File /oname=Crawlies.xml "..\config_files\Crawlies.xml"

  # registry entry for the redirector
  WriteRegStr SHCTX "Software\IdleScreen\Redirector" "Crawlies" "$TARGET_DIR\bin\Crawlies.scr"
SectionEnd

Section "PlasmaLenz"
  # main scr file
  SetOutPath "$TARGET_DIR\bin"
  File /oname=PlasmaLenz.scr "..\src\PlasmaLenz\msvc\scr\release\PlasmaLenz.exe"

  # redirector file
  SetOutPath "$REDIRECTOR_TARGET"
  File /oname=PlasmaLenz.scr "..\src\PlasmaLenz\msvc\redirector\Release\redirector.exe"

  # machine defaults file
  SetOutPath "$TARGET_DIR\defaults"
  File /oname=PlasmaLenz.xml "..\config_files\PlasmaLenz.xml"

  # registry entry for the redirector
  WriteRegStr SHCTX "Software\IdleScreen\Redirector" "PlasmaLenz" "$TARGET_DIR\bin\PlasmaLenz.scr"
SectionEnd

Section "Randomizer"
  # main scr file
  SetOutPath "$TARGET_DIR\bin"
  File /oname=Randomizer.scr "..\src\Randomizer\msvc\scr\release\Randomizer.exe"

  # redirector file
  SetOutPath "$REDIRECTOR_TARGET"
  File /oname=Randomizer.scr "..\src\Randomizer\msvc\redirector\Release\redirector.exe"

  # machine defaults file
  SetOutPath "$TARGET_DIR\defaults"
  File /oname=Randomizer.xml "..\config_files\Randomizer.xml"

  # registry entry for the redirector
  WriteRegStr SHCTX "Software\IdleScreen\Redirector" "Randomizer" "$TARGET_DIR\bin\Randomizer.scr"
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
  # attempt to install as admin
  SetShellVarContext all

  StrCpy $REDIRECTOR_TARGET "$SYSDIR"

  MessageBox MB_YESNO "Are you sure you want to uninstall the Idle Screen Project?" /SD IDYES IDNO dontremove

  #remove reg keys
  #DeleteRegValue SHCTX "Software\IdleScreen\Redirector" "PlasmaLenz"
  DeleteRegKey SHCTX "Software\IdleScreen"
  DeleteRegKey SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\IdleScreen" \

  # delete files and directories
  Delete $REDIRECTOR_TARGET\AcidRain.scr
  Delete $REDIRECTOR_TARGET\Crawlies.scr
  Delete $REDIRECTOR_TARGET\PlasmaLenz.scr
  Delete $REDIRECTOR_TARGET\Redirector.scr
  Delete "$INSTDIR\doc\*"
  RMDir $INSTDIR\doc
  Delete "$INSTDIR\bin\*"
  RMDir $INSTDIR\bin
  Delete "$INSTDIR\defaults\*"
  RMDir $INSTDIR\defaults
  Delete "$INSTDIR\$UNINST_NAME"
  Delete "$INSTDIR\*"  #don't know why this is necessary...
  RMDir $INSTDIR
  dontremove:
SectionEnd
