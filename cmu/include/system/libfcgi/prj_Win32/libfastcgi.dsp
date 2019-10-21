# Microsoft Developer Studio Project File - Name="libfastcgi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libfastcgi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libfastcgi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libfastcgi.mak" CFG="libfastcgi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libfastcgi - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libfastcgi - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libfastcgi"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libfastcgi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\10-Common\lib\release\win32\libfcgi.lib"

!ELSEIF  "$(CFG)" == "libfastcgi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "LIBFCGI_EXPORTS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\10-Common\lib\debug\win32\libfcgi.lib"

!ENDIF 

# Begin Target

# Name "libfastcgi - Win32 Release"
# Name "libfastcgi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\libfcgi\fcgi_stdio.c
# End Source File
# Begin Source File

SOURCE=..\libfcgi\fcgiapp.c
# End Source File
# Begin Source File

SOURCE=..\libfcgi\fcgio.cpp
# End Source File
# Begin Source File

SOURCE=..\libfcgi\os_unix.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\libfcgi\os_win32.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\fastcgi.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgi_config.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgi_config_x86.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgi_stdio.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgiapp.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgimisc.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgio.h
# End Source File
# Begin Source File

SOURCE=..\include\fcgios.h
# End Source File
# End Group
# End Target
# End Project
