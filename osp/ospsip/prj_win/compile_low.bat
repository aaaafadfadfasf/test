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


echo "��ʼ���� ospsip.vcxproj Debug_low|Win32"
@set ospsipOutCompileInfoFile=..\..\..\10-common\version\CompileInfo\ospsip_low_debug_win32.txt
if exist %ospsipOutCompileInfoFile% del %ospsipOutCompileInfoFile%
devenv ospsip.vcxproj %isRebuild% "Debug_low|Win32"  /OUT %ospsipOutCompileInfoFile%

echo "������� ospsip.vcxproj Debug_low|Win32"


echo "��ʼ���� ospsip.vcxproj Release_low|Win32"
@set ospsipOutCompileInfoFile = ..\..\..\10-common\version\CompileInfo\ospsip_low_relese_win32.txt
if exist %ospsipOutCompileInfoFile% del %ospsipOutCompileInfoFile%
devenv ospsip.vcxproj %isRebuild% "Release_low|Win32"  /OUT %ospsipOutCompileInfoFile%

echo "������� ospsip.vcxproj Release_low|Win32"

if not %errorlevel% == 0 ( 
	goto :eof
)

