# Microsoft Developer Studio Project File - Name="VisionASM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VisionASM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisionASM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisionASM.mak" CFG="VisionASM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisionASM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VisionASM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisionASM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Temp\VisionASM\Release"
# PROP Intermediate_Dir "..\Temp\VisionASM\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Include" /I "../CBBInclude" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 DelayImp.lib VirtualFG.lib VisionGUI6.lib VisionPLUS6.lib vsKeyboard6.lib /nologo /subsystem:windows /machine:I386 /libpath:"../Lib" /DelayLoad:svGui6.dll /DelayLoad:svBlob6.dll
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\Temp\VisionASM\Release\VisionASM.exe ..\Bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "VisionASM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Temp\VisionASM\Debug"
# PROP Intermediate_Dir "..\Temp\VisionASM\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 DelayImp.lib VirtualFG.lib VisionGUI6d.lib VisionPLUS6d.lib vsKeyboard6d.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\Temp\VisionASM\Debug\VisionASMd.exe" /pdbtype:sept /libpath:"../Lib" /DelayLoad:svCore6d.dll /DelayLoad:svGui6d.dll /DelayLoad:svBlob6d.dll
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\Temp\VisionASM\Debug\VisionASMd.exe  ..\Bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "VisionASM - Win32 Release"
# Name "VisionASM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "通信cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommProtocolProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolX1X2Y.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXY1Y2.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXYD.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXYDFast.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolYDPX1X2.cpp
# End Source File
# Begin Source File

SOURCE=.\CommProtocolYX1X2PD.cpp
# End Source File
# Begin Source File

SOURCE=.\vsComm.cpp
# End Source File
# Begin Source File

SOURCE=.\vsSerialComm.cpp
# End Source File
# Begin Source File

SOURCE=.\vsSocketComm.cpp
# End Source File
# End Group
# Begin Group "定位工具cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CircleSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CornerSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CrossSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedPosSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\LineSearch2Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\LineSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\PatCornerSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProCircleSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickSearchTool.cpp
# End Source File
# End Group
# Begin Group "对位工具cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vsAlignToolDef.cpp
# End Source File
# Begin Source File

SOURCE=.\vsBaseAlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsDXYAlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsX1X2YAlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsXY1Y2AlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsXYDAlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsYDPX1X2AlignTool.cpp
# End Source File
# Begin Source File

SOURCE=.\vsYX1X2PDAlignTool.cpp
# End Source File
# End Group
# Begin Group "Vision cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vsBaseVisionAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\vsVisionAlignDef.cpp
# End Source File
# Begin Source File

SOURCE=.\vsX1X2YVisionAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\vsXMLConfigurator.cpp
# End Source File
# Begin Source File

SOURCE=.\vsXY1Y2VisionAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\vsXYDVisionAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\vsYDPX1X2VisionAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\vsYX1X2PDVisionAlign.cpp
# End Source File
# End Group
# Begin Group "其他cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\SHA256.CPP
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThemeHelperST.cpp
# End Source File
# Begin Source File

SOURCE=.\XPStyleButtonST.cpp
# End Source File
# End Group
# Begin Group "采集cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VFGMCamU.cpp
# End Source File
# Begin Source File

SOURCE=.\vsGrab.cpp
# End Source File
# Begin Source File

SOURCE=.\vsGrabGigE.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgBenchMarkAmend.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCircleSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCornerSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCrossSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFixedPosSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLineSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatCornerSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatternCenterSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatternSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProCircleSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProductRename.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuickSearchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetCameraSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysAlignerParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysBaseCommSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysCalibrateParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysCommSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysDataRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysEnvironSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysImgRoiParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysOptionSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformX1X2YParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformXY1Y2Param.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformXYDParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformYDPX1X2Param.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformYX1X2PDParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysSerialCommSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysSocketCommSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysUserLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSysUserPasswordSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVisionView.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemConfigure.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOptionConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionASM.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionASM.rc
# End Source File
# Begin Source File

SOURCE=.\VisionASMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\vsDisplayModule.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "通信 h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommProtocolAnalyzer.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolProcess.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolX1X2Y.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXY1Y2.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXYD.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolXYDFast.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolYDPX1X2.h
# End Source File
# Begin Source File

SOURCE=.\CommProtocolYX1X2PD.h
# End Source File
# Begin Source File

SOURCE=.\RegisterAddressDef.h
# End Source File
# Begin Source File

SOURCE=.\vsComm.h
# End Source File
# Begin Source File

SOURCE=.\vsSerialComm.h
# End Source File
# Begin Source File

SOURCE=.\vsSocketComm.h
# End Source File
# End Group
# Begin Group "定位工具 h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\CircleSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\CornerSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\CrossSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\FixedPosSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\LineSearch2Tool.h
# End Source File
# Begin Source File

SOURCE=.\LineSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\PatCornerSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\PatternSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\ProCircleSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\QuickSearchTool.h
# End Source File
# End Group
# Begin Group "对位工具 h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vsAlignToolDef.h
# End Source File
# Begin Source File

SOURCE=.\vsBaseAlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsDXYAlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsX1X2YAlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsXY1Y2AlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsXYDAlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsYDPX1X2AlignTool.h
# End Source File
# Begin Source File

SOURCE=.\vsYX1X2PDAlignTool.h
# End Source File
# End Group
# Begin Group "Vision h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vsVisionAlignDef.h
# End Source File
# Begin Source File

SOURCE=.\vsX1X2YVisionAlign.h
# End Source File
# Begin Source File

SOURCE=.\vsXY1Y2VisionAlign.h
# End Source File
# Begin Source File

SOURCE=.\vsXYDVisionAlign.h
# End Source File
# Begin Source File

SOURCE=.\vsYDPX1X2VisionAlign.h
# End Source File
# Begin Source File

SOURCE=.\vsYX1X2PDVisionAlign.h
# End Source File
# End Group
# Begin Group "采集 h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VFGMCamU.h
# End Source File
# Begin Source File

SOURCE=.\vsGrab.h
# End Source File
# Begin Source File

SOURCE=.\vsGrabGigE.h
# End Source File
# End Group
# Begin Group "其他 h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\DeletedResource.h
# End Source File
# Begin Source File

SOURCE=.\Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\Language.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\SHA256.H
# End Source File
# Begin Source File

SOURCE=.\ThemeHelperST.h
# End Source File
# Begin Source File

SOURCE=.\XPStyleButtonST.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgBenchMarkAmend.h
# End Source File
# Begin Source File

SOURCE=.\DlgCircleSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgCornerSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgCrossSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgFixedPosSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgLineSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgMaskEdit.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatCornerSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatternCenterSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatternSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgProCircleSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgProductRename.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuickSearchTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetCameraSerial.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysAlignerParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysBaseCommSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysCalibrateParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysCommSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysDataRecord.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysEnvironSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysImgRoiParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysOptionSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformX1X2YParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformXY1Y2Param.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformXYDParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformYDPX1X2Param.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysPlatformYX1X2PDParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysSerialCommSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysSocketCommSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysUserLogin.h
# End Source File
# Begin Source File

SOURCE=.\DlgSysUserPasswordSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgVisionView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysPlatformInfo.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfigure.h
# End Source File
# Begin Source File

SOURCE=.\SystemOptionConfig.h
# End Source File
# Begin Source File

SOURCE=.\VisionASM.h
# End Source File
# Begin Source File

SOURCE=.\VisionASMDlg.h
# End Source File
# Begin Source File

SOURCE=.\vsBaseVisionAlign.h
# End Source File
# Begin Source File

SOURCE=.\vsDisplayModule.h
# End Source File
# Begin Source File

SOURCE=.\vsXMLConfigurator.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Connect32.ico
# End Source File
# Begin Source File

SOURCE=.\res\DisConnect32.ico
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=.\res\ErrLamp16.ico
# End Source File
# Begin Source File

SOURCE=.\res\ErrLamp32.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_fai.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_suc.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconSucceed16.ico
# End Source File
# Begin Source File

SOURCE=.\res\INIT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\left.ico
# End Source File
# Begin Source File

SOURCE=.\res\OKLamp16.ico
# End Source File
# Begin Source File

SOURCE=.\res\OKLamp32.ico
# End Source File
# Begin Source File

SOURCE=.\res\right.ico
# End Source File
# Begin Source File

SOURCE=.\res\up.ico
# End Source File
# Begin Source File

SOURCE=.\res\VisionASM.ico
# End Source File
# Begin Source File

SOURCE=.\res\VisionASM.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\XPStyle.manifest
# End Source File
# End Target
# End Project
