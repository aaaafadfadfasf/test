echo on
echo ע��VC�Ļ�������

call "%VS100COMNTOOLS%vsvars32.bat"


@set isRebuild=

if /I "%1" == "noclean" (
		@set isRebuild=/build 
    echo build no clean
) else (
    @set isRebuild=/REBUILD
    echo build clean
)

if not exist ..\..\..\10-common\version\CompileInfo mkdir ..\..\..\10-common\version\CompileInfo


echo "��ʼ���� ospext.vcxproj Debug|Win32"
@set ospextOutCompileInfoFile=..\..\..\10-common\version\CompileInfo\ospext_debug_win32.txt
if exist %ospextOutCompileInfoFile% del %ospextOutCompileInfoFile%
devenv ospext.vcxproj %isRebuild% "Debug|Win32"  /OUT %ospextOutCompileInfoFile%

echo "������� ospext.vcxproj Debug|Win32"


echo "��ʼ���� ospext.vcxproj Release|Win32"
@set ospextOutCompileInfoFile = ..\..\..\10-common\version\CompileInfo\ospext_relese_win32.txt
if exist %ospextOutCompileInfoFile% del %ospextOutCompileInfoFile%
devenv ospext.vcxproj %isRebuild% "Release|Win32"  /OUT %ospextOutCompileInfoFile%

echo "������� ospext.vcxproj Release|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

