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


echo "��ʼ���� cmu.vcxproj Debug|Win32"
@set cmuOutCompileInfoFile=..\..\..\10-common\version\CompileInfo\cmu_debug_win32.txt
if exist %cmuOutCompileInfoFile% del %cmuOutCompileInfoFile%
devenv cmu.vcxproj %isRebuild% "Debug|Win32"  /OUT %cmuOutCompileInfoFile%

echo "������� cmu.vcxproj Debug|Win32"


echo "��ʼ���� cmu.vcxproj Release|Win32"
@set cmuOutCompileInfoFile = ..\..\..\10-common\version\CompileInfo\cmu_relese_win32.txt
if exist %cmuOutCompileInfoFile% del %cmuOutCompileInfoFile%
devenv cmu.vcxproj %isRebuild% "Release|Win32"  /OUT %cmuOutCompileInfoFile%

echo "������� cmu.vcxproj Release|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

