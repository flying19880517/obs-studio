; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "XimalayaFM OBS Studio"
!define APPNAMECN "喜马拉雅FM直播助手"
!define APPVERSION "1.0.0"
; !define APPNAMEANDVERSION "${APPNAME} ${APPVERSION}"
!define COMPANYNAME "Shanghai Zhengda Ximalaya Internet Technology Co., Ltd."
!define COMPANYNAMECN "上海证大喜马拉雅网络科技有限公司"
; !define FULL
; !define REALSENSE_PLUGIN

; Additional script dependencies
!include WinVer.nsh
;!include x64.nsh

; Main Install settings
; Name "${APPNAMEANDVERSION}"
; InstallDir "$PROGRAMFILES32\${APPNAME}"
; InstallDirRegKey HKLM "Software\${APPNAME}" ""
;!ifdef FULL
;OutFile "XimalayaFM-OBS-Studio-${APPVERSION}-Full-Installer.exe"
;!else
OutFile "XimalayaFM-OBS-Studio-${APPVERSION}-Installer.exe"
;!endif

; Use compression
SetCompressor /SOLID LZMA

; Need Admin
RequestExecutionLevel admin

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\32bit\obs32.exe"

!define MUI_PAGE_CUSTOMFUNCTION_LEAVE PreReqCheck

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "data\obs-studio\license\gplv2.txt"
!insertmacro MUI_PAGE_DIRECTORY
;!ifdef FULL
;	!insertmacro MUI_PAGE_COMPONENTS
;!endif
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; !insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_COMPONENTS
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_RESERVEFILE_LANGDLL

LangString LNG_AppName ${LANG_ENGLISH} "XimalayaFM OBS Studio"
LangString LNG_AppName ${LANG_SIMPCHINESE} "喜马拉雅FM直播助手"
LangString LNG_CompanyName ${LANG_ENGLISH} "Shanghai Zhengda Ximalaya Internet Technology Co., Ltd."
LangString LNG_CompanyName ${LANG_SIMPCHINESE} "上海证大喜马拉雅网络科技有限公司"
LangString LNG_Uninstall ${LANG_ENGLISH} "Uninstall"
LangString LNG_Uninstall ${LANG_SIMPCHINESE} "卸载"
LangString LNG_MainProgram ${LANG_ENGLISH} "Main Program"
LangString LNG_MainProgram ${LANG_SIMPCHINESE} "主程序"
LangString LNG_UserData ${LANG_ENGLISH} "Main Data"
LangString LNG_UserData ${LANG_SIMPCHINESE} "用户数据"
LangString LNG_WinXP ${LANG_ENGLISH} "Due to extensive use of DirectX 10 features, $(LNG_AppName) requires Windows Vista SP2 or higher and cannot be installed on this version of Windows."
LangString LNG_WinXP ${LANG_SIMPCHINESE} "由于使用DirectX 10, $(LNG_AppName) 需要 Windows Vista SP2 或更高版本，无法在此版本的Windows安装。"
LangString LNG_WinVistaSP2 ${LANG_ENGLISH} "$(LNG_AppName) requires Service Pack 2 when running on Vista. Would you like to download it?"
LangString LNG_WinVistaSP2 ${LANG_SIMPCHINESE} "$(LNG_AppName) 在Vista上运行需要SP2。您是否要下载？"
LangString LNG_WinVistaPlatform ${LANG_ENGLISH} "$(LNG_AppName) requires the Windows Vista Platform Update. Would you like to download it?"
LangString LNG_WinVistaPlatform ${LANG_SIMPCHINESE} "$(LNG_AppName) 需要 Windows Vista 平台更新。您是否要下载？"
LangString LNG_AlreadyRunning ${LANG_ENGLISH} "$(LNG_AppName) is already running. Please close it first before installing a new version."
LangString LNG_AlreadyRunning ${LANG_SIMPCHINESE} "$(LNG_AppName) 已经运行，在安装新版本前请先关闭。"
LangString LNG_FilesInUse ${LANG_ENGLISH} "Some files were not able to be installed. If this is the first time you are installing OBS, please disable any anti-virus or other security software and try again. If you are re-installing or updating OBS, close any applications that may be have been hooked, or reboot and try again."
LangString LNG_FilesInUse ${LANG_SIMPCHINESE} "某些文件无法安装。如果这是您第一次安装，请禁用病毒防护或其他安全软件，然后重试。如果您是重新安装或升级，请关闭可能影响安装的程序或重启系统后再重试。"
LangString LNG_UnAlreadyRunning ${LANG_ENGLISH} "$(LNG_AppName) is already running. Close it?"
LangString LNG_UnAlreadyRunning ${LANG_SIMPCHINESE} "$(LNG_AppName) 仍在运行，是否关闭？"
LangString LNG_UnMainProgram ${LANG_ENGLISH} "Remove the OBS program files."
LangString LNG_UnMainProgram ${LANG_SIMPCHINESE} "删除程序文件。"
LangString LNG_UnUserData ${LANG_ENGLISH} "Removes all settings, plugins, scenes and sources, profiles, log files and other application data."
LangString LNG_UnUserData ${LANG_SIMPCHINESE} "删除所有程序文件和个人数据。"


Name "$(LNG_AppName) ${APPVERSION}"
InstallDir "$PROGRAMFILES32\${APPNAME}"
InstallDirRegKey HKLM "Software\${APPNAME}" ""

Function PreReqCheck
	; Abort on XP or lower
	${If} ${AtMostWinXP}
		MessageBox MB_OK|MB_ICONSTOP "$(LNG_WinXP)"
		Quit
	${EndIf}

	; Vista specific checks
	${If} ${IsWinVista}
		; Check Vista SP2
		${If} ${AtMostServicePack} 1
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "$(LNG_WinVistaSP2)" IDYES sptrue IDNO spfalse
			sptrue:
				ExecShell "open" "http://windows.microsoft.com/zh-cn/windows-vista/Learn-how-to-install-Windows-Vista-Service-Pack-2-SP2"
			spfalse:
			Quit
		${EndIf}

		; Check Vista Platform Update
		nsexec::exectostack "$SYSDIR\wbem\wmic.exe qfe where HotFixID='KB971512' get HotFixID /Format:list"
		pop $0
		pop $0
		strcpy $1 $0 17 6
		strcmps $1 "HotFixID=KB971512" gotPatch
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "$(LNG_WinVistaPlatform)" IDYES putrue IDNO pufalse
			putrue:
				;${If} ${RunningX64}
				;	; 64 bit
				;	ExecShell "open" "http://www.microsoft.com/zh-cn/download/details.aspx?id=4390"
				;${Else}
				;	; 32 bit
					ExecShell "open" "http://www.microsoft.com/zh-cn/download/details.aspx?id=3274"
				;${EndIf}
			pufalse:
			Quit
		gotPatch:
	${EndIf}

	ClearErrors
	GetDLLVersion "MSVCR140.DLL" $R0 $R1
	IfErrors vs2015Missing vs2015OK
	vs2015Missing:
		File "lib\vc_redist.x86.exe"
		ExecWait '"$INSTDIR\lib\vc_redist.x86.exe" /quiet'
	vs2015OK:
	ClearErrors

	; DirectX Version Check
	ClearErrors
	GetDLLVersion "D3DCompiler_33.dll" $R0 $R1
	IfErrors dxMissing33 dxOK
	dxMissing33:
	ClearErrors
	GetDLLVersion "D3DCompiler_34.dll" $R0 $R1
	IfErrors dxMissing34 dxOK
	dxMissing34:
	ClearErrors
	GetDLLVersion "D3DCompiler_35.dll" $R0 $R1
	IfErrors dxMissing35 dxOK
	dxMissing35:
	ClearErrors
	GetDLLVersion "D3DCompiler_36.dll" $R0 $R1
	IfErrors dxMissing36 dxOK
	dxMissing36:
	ClearErrors
	GetDLLVersion "D3DCompiler_37.dll" $R0 $R1
	IfErrors dxMissing37 dxOK
	dxMissing37:
	ClearErrors
	GetDLLVersion "D3DCompiler_38.dll" $R0 $R1
	IfErrors dxMissing38 dxOK
	dxMissing38:
	ClearErrors
	GetDLLVersion "D3DCompiler_39.dll" $R0 $R1
	IfErrors dxMissing39 dxOK
	dxMissing39:
	ClearErrors
	GetDLLVersion "D3DCompiler_40.dll" $R0 $R1
	IfErrors dxMissing40 dxOK
	dxMissing40:
	ClearErrors
	GetDLLVersion "D3DCompiler_41.dll" $R0 $R1
	IfErrors dxMissing41 dxOK
	dxMissing41:
	ClearErrors
	GetDLLVersion "D3DCompiler_42.dll" $R0 $R1
	IfErrors dxMissing42 dxOK
	dxMissing42:
	ClearErrors
	GetDLLVersion "D3DCompiler_43.dll" $R0 $R1
	IfErrors dxMissing43 dxOK
	dxMissing43:
	ClearErrors
	GetDLLVersion "D3DCompiler_47.dll" $R0 $R1
	IfErrors dxMissing47 dxOK
	dxMissing47:
	File "lib\dxwebsetup.exe"
	ExecWait '"$INSTDIR\lib\dxwebsetup.exe" /Q'
	dxOK:
	ClearErrors

	; Check previous instance
	FindProcDLL::FindProc "obs32.exe"
	IntCmp $R0 1 0 notRunning1
		MessageBox MB_OK|MB_ICONEXCLAMATION "$(LNG_AlreadyRunning)" /SD IDOK
		Quit
	notRunning1:
	;${if} ${RunningX64}
	;	FindProcDLL::FindProc "obs64.exe"
	;	IntCmp $R0 1 0 notRunning2
	;		MessageBox MB_OK|MB_ICONEXCLAMATION "$(LNG_FilesInUse)" /SD IDOK
	;		Quit
	;${endif}
	;notRunning2:

FunctionEnd

Function filesInUse
	MessageBox MB_OK|MB_ICONEXCLAMATION "$(LNG_FilesInUse)"  /SD IDOK
FunctionEnd

Var outputErrors

Section "$(LNG_MainProgram)" SecCore

	SetShellVarContext all
	; Set Section properties
	SectionIn RO
	SetOverwrite on
	AllowSkipFiles off

	KillProcDLL::KillProc "$INSTDIR\obs-plugins\32bit\cef-bootstrap.exe"
	;KillProcDLL::KillProc "$INSTDIR\obs-plugins\64bit\cef-bootstrap.exe"

	SetShellVarContext all

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR"
	File /r "data"
	SetOutPath "$INSTDIR\bin"
	File /r "bin\32bit"
	SetOutPath "$INSTDIR\obs-plugins"
	File /r "obs-plugins\32bit"

	;${if} ${RunningX64}
	;	SetOutPath "$INSTDIR\bin"
	;	File /r "bin\64bit"
	;	SetOutPath "$INSTDIR\obs-plugins"
	;	File /r "obs-plugins\64bit"
	;${endif}

	ClearErrors

	IfErrors 0 +2
		StrCpy $outputErrors "yes"

	WriteUninstaller "$INSTDIR\uninstall.exe"

	SetOutPath "$INSTDIR\bin\32bit"
	CreateShortCut "$DESKTOP\$(LNG_AppName).lnk" "$INSTDIR\bin\32bit\obs32.exe"
	CreateDirectory "$SMPROGRAMS\$(LNG_AppName)"
	CreateShortCut "$SMPROGRAMS\$(LNG_AppName)\$(LNG_AppName).lnk" "$INSTDIR\bin\32bit\obs32.exe"
	CreateShortCut "$SMPROGRAMS\$(LNG_AppName)\$(LNG_Uninstall)$(LNG_AppName).lnk" "$INSTDIR\uninstall.exe"

	;${if} ${RunningX64}
	;	SetOutPath "$INSTDIR\bin\64bit"
	;	CreateShortCut "$SMPROGRAMS\$(LNG_AppName)\$(LNG_AppName).lnk" "$INSTDIR\bin\64bit\obs64.exe"
	;${endif}

	SetOutPath "$INSTDIR\bin\32bit"

	StrCmp $outputErrors "yes" 0 +2
		Call filesInUse
SectionEnd

;!ifdef FULL
;SectionGroup /e "插件" SecPlugins
;	Section "浏览器插件" SecPlugins_Browser
		; Set Section properties
;		SetOverwrite on
;		AllowSkipFiles off
;		SetShellVarContext all

;		SetOutPath "$INSTDIR\obs-plugins"
;		File /r "obs-browser\obs-plugins\32bit"

		;${if} ${RunningX64}
		;	File /r "obs-browser\obs-plugins\64bit"
		;${endif}

;		SetOutPath "$INSTDIR\bin\32bit"
;	SectionEnd

;	!ifdef REALSENSE_PLUGIN
;	Section /o "Realsense plugin" SecPlugins_Realsense
;		SetOverwrite on
;		AllowSkipFiles off
;		SetShellVarContext all

;		SetOutPath "$INSTDIR\obs-plugins"
;		File /r "realsense\32bit"

		;${if} ${RunningX64}
		;	File /r "realsense\64bit"
		;${endif}

;		SetOutPath "$INSTDIR\data\obs-plugins"
;		File /r "realsense\actual_data\obs-plugins\win-ivcam"

;		ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /UnregServer'
;		ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /RegServer'

;		ReadRegStr $0 HKLM "Software\Intel\RSSDK\Dispatch" "Core"
;		${if} ${Errors}
;			ReadRegStr $0 HKLM "Software\Intel\RSSDK\v10\Dispatch" "Core"
;		${endif}

;		${if} ${Errors}
;			InitPluginsDir
;			SetOutPath "$PLUGINSDIR\realsense"

;			File "intel_rs_sdk_runtime_websetup_10.0.26.0396.exe"
;			ExecWait '"$PLUGINSDIR\realsense\intel_rs_sdk_runtime_websetup_10.0.26.0396.exe" --finstall=personify --fnone=all'
;		${endif}

;		SetOutPath "$INSTDIR\bin\32bit"
;	SectionEnd
;	!endif
;SectionGroupEnd
;!endif

Section -FinishSection

	WriteRegStr HKLM "Software\$(LNG_AppName)" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "DisplayName" "$(LNG_AppName)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "ProductID" "06eec120-b789-46c5-9fb3-80f1ea44c45a"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "DisplayIcon" "$INSTDIR\bin\32bit\obs32.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "Publisher" "$(LNG_CompanyName)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "HelpLink" "http://www.ximalaya.com"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)" "DisplayVersion" "${APPVERSION}"

SectionEnd

; Modern install component descriptions
;!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;	!insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "核心 $(LNG_AppName) 文件"
;	!ifdef FULL
;		!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins} "可选插件"
;		!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins_Browser} "浏览器插件（您可以添加到场景显示网页的源）"
;		!ifdef REALSENSE_PLUGIN
;			!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins_Realsense} "Realsense 相机插件"
;		!endif
;	!endif
;!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section "un.$(LNG_MainProgram)" UninstallSection1
	SetShellVarContext all
	SectionIn RO
	
	FindProcDLL::FindProc "obs32.exe"
	IntCmp $R0 1 0 notRunning1
		MessageBox MB_YESNO|MB_ICONEXCLAMATION "$(LNG_UnAlreadyRunning)" IDYES sptrue IDNO spfalse
		sptrue:
				KillProcDLL::KillProc "obs32.exe"
				Goto notRunning1
		spfalse:
	notRunning1:

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(LNG_AppName)"
	DeleteRegKey HKLM "SOFTWARE\$(LNG_AppName)"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\$(LNG_AppName).lnk"
	Delete "$SMPROGRAMS\$(LNG_AppName)\$(LNG_AppName).lnk"
	Delete "$SMPROGRAMS\$(LNG_AppName)\$(LNG_Uninstall)$(LNG_AppName).lnk"
	RMDir /r "$SMPROGRAMS\$(LNG_AppName)"
	;${if} ${RunningX64}
	;	Delete "$SMPROGRAMS\$(LNG_AppName)\$(LNG_AppName).lnk"
	;${endif}

	IfFileExists "$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" UnregisterSegService SkipUnreg
	UnregisterSegService:
	ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /UnregServer'
	SkipUnreg:

	; Clean up OBS Studio
	RMDir /r "$INSTDIR\bin"
	RMDir /r "$INSTDIR\data"
	RMDir /r "$INSTDIR\obs-plugins"
	RMDir /r "$INSTDIR\lib"
	RMDir /r "$INSTDIR"

	; Remove remaining directories
;	RMDir "$SMPROGRAMS\$(LNG_AppName)"
;	RMDir "$INSTDIR\$(LNG_AppName)"
SectionEnd

Section /o "un.$(LNG_UserData)" UninstallSection2
	RMDir /R "$APPDATA\obs-studio"
	DeleteRegKey HKCU "SOFTWARE\XimalayaFM\obs-studio"
SectionEnd

!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${UninstallSection1} "$(LNG_UnMainProgram)"
	!insertmacro MUI_DESCRIPTION_TEXT ${UninstallSection2} "$(LNG_UnUserData)"
!insertmacro MUI_UNFUNCTION_DESCRIPTION_END

; Version information
VIProductVersion "${APPVERSION}.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${APPNAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${COMPANYNAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "(c) 2012-2016"
; FileDescription is what shows in the UAC elevation prompt when signed
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${APPNAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${APPVERSION}"

VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "${APPNAMECN}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "CompanyName" "${COMPANYNAMECN}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "(c) 2012-2016"
; FileDescription is what shows in the UAC elevation prompt when signed
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "${APPNAMECN}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion" "${APPVERSION}"

; eof
