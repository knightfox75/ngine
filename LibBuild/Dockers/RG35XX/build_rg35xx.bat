@echo off
rem ==============================================================================
rem N'GINE SDK - WINDOWS LAUNCHER (Release Mode)
rem Target: RG35XX (Cortex-A53)
rem Feature: Build + Optional Image Export
rem ==============================================================================

title N'GINE - RG35XX ARM64 Builder [RELEASE]
cls

echo 🐳 [1/2] Building/Verifying Forge image...
rem Attempting build. To minimize layers if you have experimental features enabled:
rem docker build --squash -t ngn-rg35xx-builder -f Dockerfile.rg35xx .
docker build -t ngn-rg35xx-builder -f Dockerfile.rg35xx .

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Failed to build Docker image.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo 🚀 [2/2] Compiling N'GINE Project (RELEASE MODE)...
echo Current directory: %cd%

rem Execution with forced optimizations for the USER PROJECT.
rem The project will automatically inherit OS_LINUX from the ngn library.
docker run --rm -v "%cd%:/workspace" ngn-rg35xx-builder bash -c "mkdir -p build_rg35xx && cd build_rg35xx && cmake .. -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu -DCMAKE_BUILD_TYPE=Release && make -j%NUMBER_OF_PROCESSORS%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ✅ Build Successful. Optimized binary ready in /build_rg35xx

rem --- OPTIONAL IMAGE EXPORT ---
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
    echo ⏭️ Skipping image export.
)

echo.
echo Process finished.
pause