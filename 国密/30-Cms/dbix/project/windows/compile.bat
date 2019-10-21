echo on
echo ע��VC�Ļ�������

call "%VS100COMNTOOLS%vsvars32.bat"


@set isRebuild=
@set PRJNAME=dbix
@set COMPILELOG=..\..\..\..\10-Common\version\CompileInfo\

if /I "%1" == "noclean" (
		@set isRebuild=/build 
    echo build no clean
) else (
    @set isRebuild=/REBUILD
    echo build clean
)

if not exist %COMPILELOG% mkdir %COMPILELOG%

echo "��ʼ���� %PRJNAME% Debug|Win32"
@set DebugOutCompileInfoFile=%COMPILELOG%%PRJNAME%_debug_win32.txt
if exist %DebugOutCompileInfoFile% del %DebugOutCompileInfoFile%
devenv %PRJNAME%.vcxproj %isRebuild% "Debug|Win32"  /OUT %DebugOutCompileInfoFile%

echo "������� %PRJNAME% Debug|Win32"


echo "��ʼ���� %PRJNAME% Release|Win32"
@set ReleaseOutCompileInfoFile=%COMPILELOG%%PRJNAME%_relese_win32.txt
if exist %ReleaseOutCompileInfoFile% del %ReleaseOutCompileInfoFile%
devenv %PRJNAME%.vcxproj %isRebuild% "Release|Win32"  /OUT %ReleaseOutCompileInfoFile%

echo "������� %PRJNAME% Release|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

