@echo off
title N'GINE - RPi4 RELEASE CROSS-COMPILER
cls

:: ============================================================================
:: CONFIGURATION
:: ============================================================================
set "IMAGE_NAME=ngn-rpi4-builder_production"
set "DOCKER_FILE=Dockerfile.ngn-rpi4-release"
set "TAR_NAME=ngn-rpi4-builder_production.tar"

echo ============================================================================
echo  N'GINE RPi4 MASTER FORGE (CROSS-COMPILER)
echo  Target: Raspberry Pi 4 (ARM64)
echo  Mode: RELEASE (Optimized -O3, Stripped)
echo ============================================================================
echo.

:: ----------------------------------------------------------------------------
:: STEP 1: BUILD DOCKER IMAGE
:: ----------------------------------------------------------------------------
echo 🐳 [1/2] Building Cross-Compiler Image...
echo    This uses your PC CPU (Fast), not QEMU.
echo.

:: IMPORTANT: No --platform flag. We want native x86 execution for the cross-compiler.
docker build -t %IMAGE_NAME% -f %DOCKER_FILE% .

if errorlevel 1 (
    echo.
    echo ❌ [ERROR] Failed to build the Docker image.
    pause
    exit /b 1
)

:: ----------------------------------------------------------------------------
:: STEP 2: VERIFICATION
:: ----------------------------------------------------------------------------
echo.
echo 🔍 [2/2] Verifying NGN Lib Installation...
docker run --rm %IMAGE_NAME% bash -c "ls -lh /usr/aarch64-linux-gnu/lib/libngn.a"

if errorlevel 1 (
    echo ⚠️  Warning: Library verification failed.
) else (
    echo ✅ NGN Static Lib found and ready.
)

:: ----------------------------------------------------------------------------
:: STEP 3: OPTIONAL IMAGE EXPORT
:: ----------------------------------------------------------------------------
echo.
set "export_choice="
set /p export_choice=">> Do you want to export the Docker Image to a .tar file? (y/N): "

if /i "%export_choice%"=="y" (
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
    echo ⏭️  Skipping export.
)

echo.
echo ============================================================================
echo  READY TO CROSS-COMPILE
echo ============================================================================
pause