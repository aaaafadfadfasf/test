echo on
echo 注册VC的环境变量

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


echo "开始编译 ospsip.vcxproj Debug|Win32"
@set ospsipOutCompileInfoFile=..\..\..\10-common\version\CompileInfo\ospsip_debug_win32.txt
if exist %ospsipOutCompileInfoFile% del %ospsipOutCompileInfoFile%
devenv ospsip.vcxproj %isRebuild% "Debug|Win32"  /OUT %ospsipOutCompileInfoFile%

echo "编译完成 ospsip.vcxproj Debug|Win32"


echo "开始编译 ospsip.vcxproj Release|Win32"
@set ospsipOutCompileInfoFile = ..\..\..\10-common\version\CompileInfo\ospsip_relese_win32.txt
if exist %ospsipOutCompileInfoFile% del %ospsipOutCompileInfoFile%
devenv ospsip.vcxproj %isRebuild% "Release|Win32"  /OUT %ospsipOutCompileInfoFile%

echo "编译完成 ospsip.vcxproj Release|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

