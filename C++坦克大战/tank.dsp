# Microsoft Developer Studio Project File - Name="tank" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tank - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tank.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tank.mak" CFG="tank - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tank - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib winmm.lib /nologo /subsystem:windows /machine:I386
# Begin Target

# Name "tank - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BeginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Egg.cpp
# End Source File
# Begin Source File

SOURCE=.\Explode.cpp
# End Source File
# Begin Source File

SOURCE=.\FireBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\ICEBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\LaserBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\MIDI.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tank.cpp
# End Source File
# Begin Source File

SOURCE=.\tank.rc
# End Source File
# Begin Source File

SOURCE=.\Tank1.cpp
# End Source File
# Begin Source File

SOURCE=.\tankDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TankMap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BeginDlg.h
# End Source File
# Begin Source File

SOURCE=.\Bullet.h
# End Source File
# Begin Source File

SOURCE=.\DirectSound.h
# End Source File
# Begin Source File

SOURCE=.\Egg.h
# End Source File
# Begin Source File

SOURCE=.\Explode.h
# End Source File
# Begin Source File

SOURCE=.\FireBullet.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\ICEBullet.h
# End Source File
# Begin Source File

SOURCE=.\LaserBullet.h
# End Source File
# Begin Source File

SOURCE=.\MIDI.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# Begin Source File

SOURCE=.\NormalBullet.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tank.h
# End Source File
# Begin Source File

SOURCE=.\Tank1.h
# End Source File
# Begin Source File

SOURCE=.\Tank_Defs.h
# End Source File
# Begin Source File

SOURCE=.\tankDlg.h
# End Source File
# Begin Source File

SOURCE=.\TankMap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\3dwarro.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bullet3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bullet4.bmp
# End Source File
# Begin Source File

SOURCE=.\CANCEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CANCEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\email.bmp
# End Source File
# Begin Source File

SOURCE=.\res\empty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grab.cur
# End Source File
# Begin Source File

SOURCE=.\res\harrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\ice.bmp
# End Source File
# Begin Source File

SOURCE=.\LOGONOK.bmp
# End Source File
# Begin Source File

SOURCE=.\LOGONOUT.bmp
# End Source File
# Begin Source File

SOURCE=.\OK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\soil.bmp
# End Source File
# Begin Source File

SOURCE=.\res\steel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\supertan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tank.ico
# End Source File
# Begin Source File

SOURCE=.\res\tank.rc2
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\url.bmp
# End Source File
# Begin Source File

SOURCE=.\res\water.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wave1.bin
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bang.wav
# End Source File
# Begin Source File

SOURCE=.\res\Bang.wav
# End Source File
# Begin Source File

SOURCE=.\res\bkmusic.mid
# End Source File
# Begin Source File

SOURCE=.\res\CONTACT.WAV
# End Source File
# Begin Source File

SOURCE=.\res\Egg.wav
# End Source File
# Begin Source File

SOURCE=.\res\move.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\Shoot.wav
# End Source File
# End Target
# End Project
