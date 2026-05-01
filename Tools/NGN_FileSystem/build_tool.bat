@echo off
setlocal
title N'GINE SDK - Universal Utility Builder
cls

:: --- [1] CONFIGURATION ---
set "TOOL_NAME=NGN_FileSystem"
set "LINUX_IMAGE=ngn-linux-pc-builder_production"
set "RPI_IMAGE=ngn-rpi4-builder_production"

echo ========================================================
echo  N'GINE SDK - Multi-Platform Utility Builder
echo  Tool: %TOOL_NAME%
echo ========================================================
echo.
echo Select Target Platform:
echo [1] Windows (Local MinGW)
echo [2] Linux x86_64 (Docker)
echo [3] Raspberry Pi (Docker)
echo.
set /p CHOICE="Selection (1-3): "

if "%CHOICE%"=="1" goto :BUILD_WIN
if "%CHOICE%"=="2" goto :BUILD_LINUX
if "%CHOICE%"=="3" goto :BUILD_RPI
goto :EOF

:: --- [2] TARGETS ---

:BUILD_WIN
echo [INFO] Building for Windows...
if not exist "build_win" mkdir build_win
cd build_win
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_ARCH_TYPE=Windows_x86_64 -DTOOL_NAME="%TOOL_NAME%"
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
goto :FINISH

:BUILD_LINUX
echo [INFO] Launching Linux x86_64 Docker Build...
docker run --rm --platform linux/amd64 -v "%cd%:/workspace" %LINUX_IMAGE% bash scripts/internal_build.sh "%TOOL_NAME%" "Linux_x86_64"
goto :FINISH

:BUILD_RPI
echo [INFO] Launching Raspberry Pi Docker Build...
docker run --rm -v "%cd%:/workspace" %RPI_IMAGE% bash scripts/internal_build.sh "%TOOL_NAME%" "Raspberry"
goto :FINISH

:FINISH
echo.
echo ========================================================
echo Process Complete. Check /exports/ folder.
pause
exit /b 0