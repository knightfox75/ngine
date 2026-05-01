@echo off
setlocal enableextensions enabledelayedexpansion

rem ==============================================================================
rem N'GINE SDK - LINUX PC BUILDER (Production/Release)
rem Target: x86_64 Linux / SteamDeck
rem Feature: Build + Verify + Optional Image Export
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
title N'GINE SDK - Builder Image Production Launcher
cls

rem Configuration Constants
set "BUILDER_IMAGE=ngn-linux-pc-builder_production"
set "DOCKER_FILE=Dockerfile.ngine-linux-pc"
set "IMAGE_TAR_NAME=ngn-linux-pc-builder_production.tar"

echo ========================================================
echo  N'GINE SDK - LINUX PC BUILDER (Production)
echo ========================================================

rem --- STEP 1: BUILD IMAGE ---
echo.
echo 🐳 [1/3] Initializing Docker Build Process (Production)...

rem Building for linux/amd64 explicitly to ensure compatibility
docker build --platform linux/amd64 -t %BUILDER_IMAGE% -f %DOCKER_FILE% .

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ [CRITICAL ERROR] Docker build failed. Inspect the logs above.
    goto :PAUSE_EXIT
)

rem --- STEP 2: VERIFY ENVIRONMENT ---
echo.
echo 🔍 [2/3] Verifying Production Environment Integrity...

rem Check if the 'ngn' library is correctly registered within the container
docker run --rm --platform linux/amd64 %BUILDER_IMAGE% pkg-config --modversion ngn

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ [VERIFICATION ERROR] 'ngn' package not found within the built image.
    goto :PAUSE_EXIT
)

rem --- STEP 3: OPTIONAL EXPORT ---
echo.
echo 📦 [3/3] Export Docker Image to .tar for distribution?
set /p EXPORT_CHOICE=">> Do you want to export the builder image? (y/N): "

if /i "%EXPORT_CHOICE%"=="y" (
    echo.
    echo 💾 Exporting Docker Image to filesystem...
    echo This may take a moment...
    
    docker save -o %IMAGE_TAR_NAME% %BUILDER_IMAGE%
    
    if not errorlevel 1 (
        echo ✅ SUCCESS: Image exported to: %IMAGE_TAR_NAME%
    ) else (
        echo ⚠️ [EXPORT ERROR] Failed to save .tar file. Check disk space.
    )
) else (
    echo.
    echo ⏭️  Export phase skipped by user choice.
)

echo.
echo ✅ Pipeline completed successfully.

:PAUSE_EXIT
echo.
echo Press any key to close...
pause >nul
exit /b