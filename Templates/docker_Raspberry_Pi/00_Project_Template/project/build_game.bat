@echo off
setlocal
title N'GINE SDK - RPi4 Production Builder

rem ==============================================================================
rem N'GINE SDK - RPI4 PRODUCTION BUILDER (Windows Host)
rem Target:      Raspberry Pi 4 (Cortex-A72 / ARM64)
rem Description: Cross-compilation via Docker container.
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
rem 1. Define ANSI Colors
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "ESC=%%b"
)
set "GREEN=%ESC%[1;32m"
set "BLUE=%ESC%[1;34m"
set "YELLOW=%ESC%[1;33m"
set "RED=%ESC%[0;31m"
set "NC=%ESC%[0m"

rem 2. Configuration Constants
set "PROJECT_NAME=Sample_Project"
set "PROJECT_VERSION=0.1.0.0"
set "DOCKER_IMAGE=ngn-rpi4-builder_production"

cls
echo %BLUE%========================================================%NC%
echo %GREEN%   N'GINE SDK - RPI4 PRODUCTION BUILDER (WIN HOST)      %NC%
echo %BLUE%========================================================%NC%
echo(
echo    Target:  %YELLOW%Raspberry Pi 4 (ARM64)%NC%
echo    Project: %PROJECT_NAME% v%PROJECT_VERSION%
echo(

rem --- STEP 1: PRE-FLIGHT CHECK ---
echo 🔍 %BLUE%[1/2] Verifying Workspace...%NC%

if not exist "scripts\build_internal.sh" (
    echo    %RED%❌ Error: Build script 'scripts\build_internal.sh' not found.%NC%
    echo    %YELLOW%Ensure you are running this from the project root.%NC%
    goto :ERROR
)
echo    - Internal script found.

rem --- STEP 2: LAUNCH BUILD CONTAINER ---
echo(
echo 🚀 %BLUE%[2/2] Initializing Cross-Compilation Environment...%NC%
echo    - Target Image: %DOCKER_IMAGE%
echo    - Workspace:    %cd%

rem We pass exactly 2 arguments: Name, Version
docker run --rm -v "%cd%:/workspace" %DOCKER_IMAGE% bash scripts/build_internal.sh "%PROJECT_NAME%" "%PROJECT_VERSION%"

if %ERRORLEVEL% NEQ 0 (
    echo(
    echo %RED%❌ Compilation Error inside Docker container.%NC%
    goto :ERROR
)

rem --- STEP 3: SUCCESS REPORTING ---
echo(
echo %GREEN%========================================================%NC%
echo %GREEN%✅ BUILD COMPLETE!                                      %NC%
echo %GREEN%   RPi4 binaries located in: export/                    %NC%
echo %GREEN%========================================================%NC%
echo(
echo %BLUE%Press any key to close...%NC%
pause >nul
exit /b 0

:ERROR
echo(
echo %RED%========================================================%NC%
echo %RED%❌ [CRITICAL ERROR] Build Pipeline Failed                 %NC%
echo %RED%   Review the Docker logs above for details.            %NC%
echo %RED%========================================================%NC%
echo %BLUE%Press any key to close...%NC%
pause >nul
exit /b 1