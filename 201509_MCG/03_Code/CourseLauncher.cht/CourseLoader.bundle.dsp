# Microsoft Developer Studio Project File - Name="CourseLoader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CourseLoader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CourseLoader.bundle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CourseLoader.bundle.mak" CFG="CourseLoader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CourseLoader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CourseLoader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CourseLoader - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHT" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\elearningtitle.Cht\bin\CourseLauncher_express.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "CourseLoader - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHT" /D "_AFXDLL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\elearningtitle.Cht\bin\CourseLauncher_express.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CourseLoader - Win32 Release"
# Name "CourseLoader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\eLearning.Cht\BmpApi.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\BmpBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmkapi.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\ColGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\CourseLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\CourseLoader.rc
# End Source File
# Begin Source File

SOURCE=.\CourseLoaderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CScorm.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\DataBrowseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAskCourseDup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAskType.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCourseDupFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputProfileName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputScornInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLaunchOK.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRegenIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelType.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransCourse.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTypeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgViewSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\EncodeApi.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\Grdapi.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\Gridctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMediaServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PageWebServer.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\Uigrid.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\eLearning.Cht\BmpApi.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\BmpBtn.h
# End Source File
# Begin Source File

SOURCE=.\Cmkapi.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\ColGrid.h
# End Source File
# Begin Source File

SOURCE=.\CourseLoader.h
# End Source File
# Begin Source File

SOURCE=.\CourseLoaderDlg.h
# End Source File
# Begin Source File

SOURCE=.\CScorm.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\DataBrowseWnd.h
# End Source File
# Begin Source File

SOURCE=.\DlgAskCourseDup.h
# End Source File
# Begin Source File

SOURCE=.\DlgAskType.h
# End Source File
# Begin Source File

SOURCE=.\DlgCourseDupFTP.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputProfileName.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputScornInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgLaunchOK.h
# End Source File
# Begin Source File

SOURCE=.\DlgOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgRegenIndex.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelType.h
# End Source File
# Begin Source File

SOURCE=.\DlgSQL.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransCourse.h
# End Source File
# Begin Source File

SOURCE=.\DlgTypeSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgViewSelect.h
# End Source File
# Begin Source File

SOURCE=.\EncodeApi.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\Grdapi.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\GRIDCTRL.H
# End Source File
# Begin Source File

SOURCE=.\PageMediaServer.h
# End Source File
# Begin Source File

SOURCE=.\PageWebServer.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SheetSetting.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=..\eLearning.Cht\Uigrid.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\check1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\coldtoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CourseLoader.ico
# End Source File
# Begin Source File

SOURCE=.\res\CourseLoader.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR1.CUR
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR_D.CUR
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\hottoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\NOTE_DIS.BMP
# End Source File
# Begin Source File

SOURCE=.\res\NOTE_EN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UNKNOWN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\unknown1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\ZipArchive\Release\ZipArchive.lib
# End Source File
# End Target
# End Project
