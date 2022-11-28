@echo off
setlocal
prompt $G$S
set _=%1
if defined _ set _=_%_: =_%
set name=Virtual.Desktop.Plugin.Demo%_%

rem echo on
rem msbuild VirtualDesktop.sln /p:Configuration=Release /p:Platformt=x86 /t:Rebuild || exit /b
rem msbuild VirtualDesktop.sln /p:Configuration=Release /p:Platformt=x64 /t:Rebuild || exit /b

echo on
@call :safemkdir Plugins
@call :safemkdir Plugins\32Bit
@call :safemkdir Plugins\64Bit
copy Bin\Win32Release\VirtualDesktop.dll Plugins\32Bit || exit /b
copy Bin\x64Release\VirtualDesktop.dll Plugins\64Bit || exit /b
if exist %name%.zip del %name%.zip
7z a %name%.zip Plugins Skins RMSKIN.ini
del %name%.rmskin
powershell -File %~dp0MakeRmsSkin.ps1 %name%.zip
@goto :eof

:safemkdir %1
if not exist %1 mkdir %1
@goto :eof
