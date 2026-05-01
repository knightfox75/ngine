@echo off
setlocal enableextensions enabledelayedexpansion

rem ==============================================================================
rem N'GINE - RPi4 RELEASE CROSS-COMPILER (Windows Host)
rem Target: Raspberry Pi 4 (ARM64)
rem Mode: RELEASE (Optimized -O3, Stripped)
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
title N'GINE - RPi4 RELEASE CROSS-COMPILER
cls

rem Configuration Constants
set "IMAGE_NAME=ngn-rpi4-builder_production"
set "DOCKER_FILE=Dockerfile.ngn-rpi4-release"
set "TAR_NAME=ngn-rpi4-builder_production.tar"

echo ============================================================================
echo  N'GINE RPi4 MASTER FORGE (CROSS-COMPILER)
echo  Target: Raspberry Pi 4 (ARM64)
echo  Mode: RELEASE (Optimized -O3, Stripped)
echo ============================================================================
echo.

rem --- STEP 1: BUILD DOCKER IMAGE ---
echo 🐳 [1/2] Building Cross-Compiler Image...
echo    This uses your PC CPU (Fast), not QEMU.
echo.

rem IMPORTANT: No --platform flag. We want native x86 execution for the cross-compiler.
docker build -t %IMAGE_NAME% -f %DOCKER_FILE% .

if errorlevel 1 (
    echo.
    echo ❌ [ERROR] Failed to build the Docker image.
    goto :PAUSE_EXIT
)

rem --- STEP 2: VERIFICATION ---
echo.
echo 🔍 [2/2] Verifying NGN Lib Installation...

rem Check if the static library exists within the cross-compilation toolchain path
docker run --rm %IMAGE_NAME% bash -c "ls -lh /usr/aarch64-linux-gnu/lib/libngn.a"

if errorlevel 1 (
    echo.
    echo ⚠️  [WARNING] Library verification failed.
) else (
    echo ✅ NGN Static Lib found and ready.
)

rem --- STEP 3: OPTIONAL IMAGE EXPORT ---
echo.
echo 📦 [EXTRA] Docker Image Export
set /p EXPORT_CHOICE=">> Do you want to export the Docker Image to a .tar file? (y/N): "

if /i "%EXPORT_CHOICE%"=="y" (
    echo.
    echo 💾 Exporting Docker Image: %TAR_NAME% ...
    echo    Please wait...
    docker save -o %TAR_NAME% %IMAGE_NAME%

    if not errorlevel 1 (
        echo ✅ Image exported successfully.
    ) else (
        echo ❌ Failed to export Docker image.
    )
) else (
    echo.
    echo ⏭️  Skipping export.
)

echo.
echo ============================================================================
echo  READY TO CROSS-COMPILE
echo ============================================================================

:PAUSE_EXIT
echo.
echo Press any key to close...
pause >nul
exit /b