@echo off
rem ============================================================================
rem  N'GINE - RG35XX Project Builder (Windows Host)
rem  Target Architecture: ARM64 (Anbernic RG35XX / Cortex-A53)
rem ============================================================================
set PROJECT_NAME=Sample_Project
set PROJECT_VERSION=0.1.0.0
set DOCKER_IMAGE=ngn-rg35xx-builder

title %PROJECT_NAME% v%PROJECT_VERSION% - Hybrid Builder
cls
echo ----------------------------------------------------
echo  %PROJECT_NAME% - RG35XX Release Builder
echo  Version: %PROJECT_VERSION%
echo ----------------------------------------------------

echo.
echo 🐳 [1/2] Checking Docker Image...
rem Verify if the forge image exists before attempting execution
docker image inspect %DOCKER_IMAGE% >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Image '%DOCKER_IMAGE%' not found. 
    echo Please ensure the Docker image is built before running this script.
    pause
    exit /b
)

echo.
echo 🚀 [2/2] Compiling...
rem Execute the internal build script inside the ARM64 cross-compiler container
rem We pass PROJECT_NAME and PROJECT_VERSION as explicit arguments to the internal script
docker run --rm ^
    -v "%cd%:/workspace" ^
    %DOCKER_IMAGE% ^
    bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh %PROJECT_NAME% %PROJECT_VERSION%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ Compilation Failed.
    pause
    exit /b
)

echo.
echo ✅ Done! 
echo    Binary location: export/%PROJECT_NAME%
echo.
echo    [POST-BUILD REQUIREMENT]
echo    Ensure you have your patched 'libSDL2.so' in the 'libs' subfolder 
echo    on the target device to satisfy the RPATH configuration.
pause