@echo off
rem ==============================================================================
rem N'GINE SDK - WINDOWS LAUNCHER (Release Mode)
rem Target: RG35XX (Cortex-A53)
rem Feature: Build + Optional Image Export
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
title N'GINE - RG35XX ARM64 Builder [RELEASE]
cls

echo ========================================
echo    N'GINE - RG35XX ARM64 BUILDER    
echo ========================================

rem --- STEP 1: BUILD THE IMAGE ---
echo.
echo 🐳 [1/2] Building/Verifying Forge image...

rem Attempting build.
docker build -t ngn-rg35xx-builder -f Dockerfile.rg35xx .

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Failed to build the Docker image.
    pause
    exit /b %ERRORLEVEL%
)

rem --- STEP 2: COMPILE THE PROJECT ---
echo.
echo 🚀 [2/2] Compiling N'GINE Project (RELEASE MODE)...
echo Workdir: %cd%

rem Runtime Flags: --rm (cleanup), -v (mount volume).
docker run --rm -v "%cd%:/workspace" ngn-rg35xx-builder bash -c "mkdir -p build_rg35xx && cd build_rg35xx && cmake .. -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu -DCMAKE_BUILD_TYPE=Release && make -j%NUMBER_OF_PROCESSORS%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Compilation has failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ✅ Process completed. Optimized binary ready in /build_rg35xx

rem --- STEP 3: OPTIONAL IMAGE EXPORT ---
echo.
echo 📦 [EXTRA] Image Export Utility
set /p EXPORT_CHOICE="Do you want to export the builder image to a .tar file? (y/N): "

if /i "%EXPORT_CHOICE%"=="y" (
    echo.
    echo 💾 Exporting 'ngn-rg35xx-builder' to 'ngn-rg35xx-builder.tar'...
    echo This may take a moment depending on your disk speed...
    docker save -o ngn-rg35xx-builder.tar ngn-rg35xx-builder
    
    if not errorlevel 1 (
        echo ✅ Image exported successfully as ngn-rg35xx-builder.tar
    ) else (
        echo ⚠️ Error: Failed to export image.
    )
) else (
    echo.
    echo ⏭️  Skipping image export.
)

echo.
echo Press any key to close...
pause >nul