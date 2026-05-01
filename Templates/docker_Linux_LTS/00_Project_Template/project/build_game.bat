@echo off
setlocal
title N'GINE SDK - Game Production Builder

rem ==============================================================================
rem N'GINE SDK - GAME PRODUCTION BUILDER (Windows Launcher)
rem Target:      x86_64 Linux / SteamDeck (Release)
rem Description: Cross-compilation orchestrator via Docker container.
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
rem 1. Define ANSI Colors (Windows 10/11 Compatible)
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "ESC=%%b"
)
set "GREEN=%ESC%[1;32m"
set "BLUE=%ESC%[1;34m"
set "YELLOW=%ESC%[1;33m"
set "RED=%ESC%[0;31m"
set "NC=%ESC%[0m"

rem 2. Configuration Constants
set "PROJECT_NAME=Project_Sample"
set "PROJECT_VERSION=0.1.0.0"
set "DOCKER_IMAGE=ngn-linux-pc-builder_production"

cls
echo %BLUE%========================================================%NC%
echo %GREEN%  N'GINE SDK - LINUX PRODUCTION BUILDER (WIN HOST)      %NC%
echo %BLUE%========================================================%NC%
echo.

rem --- STEP 1: PRE-FLIGHT CHECK ---
echo 🔍 %BLUE%[1/2] Verifying Workspace...%NC%
if not exist "scripts\build_internal.sh" (
    echo %RED%❌ [CRITICAL] Build script 'scripts\build_internal.sh' not found.%NC%
    echo %RED%   Ensure you are running this from the project root.%NC%
    goto :BUILD_FAILED
)

rem --- STEP 2: LAUNCH BUILD CONTAINER ---
echo.
echo 🐳 %BLUE%[2/2] Initializing Production Environment...%NC%
echo    Target Image: %DOCKER_IMAGE%
echo.

rem Mounting %cd% to /workspace. Passing Args: Name, Version
docker run --rm --platform linux/amd64 -v "%cd%:/workspace" %DOCKER_IMAGE% bash scripts/build_internal.sh "%PROJECT_NAME%" "%PROJECT_VERSION%"

if %ERRORLEVEL% neq 0 goto :BUILD_FAILED

rem --- STEP 3: SUCCESS REPORTING ---
echo.
echo %GREEN%========================================================%NC%
echo %GREEN%✅ BUILD SUCCESSFUL!                                    %NC%
echo %GREEN%   Standalone binaries located in: 'export/'            %NC%
echo %GREEN%========================================================%NC%
goto :FINISH

:BUILD_FAILED
echo.
echo %RED%========================================================%NC%
echo %RED%⚠️ [CRITICAL] PIPELINE FAILED                            %NC%
echo %RED%   Review the Docker logs above for compilation errors. %NC%
echo %RED%========================================================%NC%
echo %BLUE%Press any key to close...%NC%
pause >nul
exit /b 1

:FINISH
echo.
echo %BLUE%Press any key to close...%NC%
pause >nul
exit /b 0