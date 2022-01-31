@setlocal
echo off
prompt $G$S
set name=Virtual.Desktop.Plugin.Demo_1.1

echo on
@call :safemkdir Plugins
@call :safemkdir Plugins\32Bit
@call :safemkdir Plugins\64Bit
copy Bin\Win32Release\VirtualDesktop.dll Plugins\32Bit
copy Bin\x64Release\VirtualDesktop.dll Plugins\64Bit
if exist %name%.zip del %name%.zip
7z a %name%.zip Plugins Skins RMSKIN.ini
del %name%.rmskin
powershell -File MakeRmsSkin.ps1 %name%.zip
@goto :eof

:safemkdir %1
if not exist %1 mkdir %1
@goto :eof
