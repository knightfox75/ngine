@echo off
rem ============================================================================
rem  N'GINE SDK - RG35XX Project Builder (Windows Template)
rem ============================================================================

rem [CONFIGURATION] Define your project name here (No spaces recommended)
set PROJECT_NAME=RG35XX_ngine_demo
set DOCKER_IMAGE=ngn-arm64-builder

title %PROJECT_NAME% - RG35XX Builder
cls
echo ====================================================
echo      %PROJECT_NAME% - RG35XX Cross-Compiler
echo ====================================================

echo.
echo 🐳 [1/2] Verifying Docker image...
docker image inspect %DOCKER_IMAGE% >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] The image '%DOCKER_IMAGE%' does not exist.
    echo Please run the SDK Forge/Builder script first.
    pause
    exit /b
)

echo.
echo 🚀 [2/2] Launching compilation container...
echo Project Directory: %cd%

rem Run the internal build script inside the container.
rem We pass %PROJECT_NAME% as the first argument to build_internal.sh

docker run --rm -v "%cd%:/workspace" %DOCKER_IMAGE% bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh %PROJECT_NAME%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ [ERROR] Compilation in Docker failed.
    pause
    exit /b
)

echo.
echo ✅ Compilation Finished.
echo The binary '%PROJECT_NAME%' is waiting in the 'export/RG35XX' folder.
echo Copy it to your console alongside the 'libs' folder.
pause