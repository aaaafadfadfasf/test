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


echo "开始编译 ospext.vcxproj Debug|Win32"
@set ospextOutCompileInfoFile=..\..\..\10-common\version\CompileInfo\ospext_debug_win32.txt
if exist %ospextOutCompileInfoFile% del %ospextOutCompileInfoFile%
devenv ospext.vcxproj %isRebuild% "Debug|Win32"  /OUT %ospextOutCompileInfoFile%

echo "编译完成 ospext.vcxproj Debug|Win32"


echo "开始编译 ospext.vcxproj Release|Win32"
@set ospextOutCompileInfoFile = ..\..\..\10-common\version\CompileInfo\ospext_relese_win32.txt
if exist %ospextOutCompileInfoFile% del %ospextOutCompileInfoFile%
devenv ospext.vcxproj %isRebuild% "Release|Win32"  /OUT %ospextOutCompileInfoFile%

echo "编译完成 ospext.vcxproj Release|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

