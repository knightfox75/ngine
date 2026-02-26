@echo off
:: -----------------------------------------------------------------------------
:: N'GINE SDK - RPi4 Production Builder (Windows Launcher)
:: -----------------------------------------------------------------------------
:: Description: Cross-compiles project for Raspberry Pi 4 (ARM64) using
::              the N'GINE Master Forge Docker container.
:: Target:      Raspberry Pi 4 (Cortex-A72 / ARM64)
:: -----------------------------------------------------------------------------

setlocal
title N'GINE - RPi4 Production Builder
cls

:: Configuration Constants
set "PROJECT_NAME=Sample_Project"
set "PROJECT_VERSION=1.0.0"
:: EXACT match for your new Docker Image
set "DOCKER_IMAGE=ngn-rpi4-builder_production"

echo ========================================================
echo  N'GINE SDK - RPi4 Production Build Launcher
echo ========================================================
echo.

:: PRE-FLIGHT CHECK
if not exist "scripts\build_internal.sh" (
    echo [ERROR] Build script 'scripts\build_internal.sh' not found.
    echo Ensure you are running this from the project root.
    goto :BUILD_FAILED
)

:: 1. LAUNCH BUILD CONTAINER
:: We use normal docker run. The container is x86, executing cross-compilers.
echo [1/2] Initializing Cross-Compilation Environment...
docker run --rm -v "%cd%:/workspace" %DOCKER_IMAGE% bash scripts/build_internal.sh "%PROJECT_NAME%" "%PROJECT_VERSION%"

if errorlevel 1 goto :BUILD_FAILED

:: 2. SUCCESS REPORTING
echo.
echo [2/2] Build Successful! 
echo RPi4 binaries and assets are located in the 'export/' directory.
goto :FINISH

:BUILD_FAILED
echo.
echo [CRITICAL] Pipeline failed. Review the Docker logs above.
pause
exit /b 1

:FINISH
echo.
echo --------------------------------------------------------
echo Process Complete.
pause
exit /b 0