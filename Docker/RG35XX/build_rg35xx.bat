@echo off
title N'GINE - RG35XX ARM64 Builder
cls

echo 🐳 [1/2] Building/Verifying Forge image...
docker build -t ngn-arm64-builder -f Dockerfile.aarch64 .

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Failed to build Docker image.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo 🚀 [2/2] Compiling N'GINE for RG35XX (ARM64)...
echo Current directory: %cd%

rem Execute container:
rem -v maps host directory to /workspace inside the container
docker run --rm -v "%cd%:/workspace" ngn-arm64-builder bash -c "mkdir -p build_rg35xx && cd build_rg35xx && cmake .. -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu && make -j%NUMBER_OF_PROCESSORS%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ✅ Process completed. N'GINE SDK compiled and installed correctly.
pause