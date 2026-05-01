@echo off
rem ============================================================================
rem  N'GINE - RG35XX Project Builder (Windows Host)
rem  Target Architecture: ARM64 (Anbernic RG35XX / Cortex-A53)
rem ============================================================================

rem --- UTF-8 SAFETY JUMP ---
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
rem 1. Define ANSI Colors
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "ESC=%%b"
)
set "GREEN=%ESC%[1;32m"
set "BLUE=%ESC%[1;34m"
set "YELLOW=%ESC%[1;33m"
set "RED=%ESC%[0;31m"
set "NC=%ESC%[0m"

set PROJECT_NAME=Sample_Project
set PROJECT_VERSION=0.1.0.0
set DOCKER_IMAGE=ngn-rg35xx-builder

cls
echo %BLUE%----------------------------------------------------%NC%
echo %GREEN%  %PROJECT_NAME% - RG35XX Release Builder           %NC%
echo   Version: %PROJECT_VERSION%
echo %BLUE%----------------------------------------------------%NC%
echo.

echo 🐳 %BLUE%[1/2] Checking Docker Image...%NC%
rem Verify if the forge image exists before attempting execution
docker image inspect %DOCKER_IMAGE% >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo %RED%[ERROR] Image '%DOCKER_IMAGE%' not found. %NC%
    echo Please ensure the Docker image is built before running this script.
    pause
    exit /b
)

echo.
echo 🚀 %BLUE%[2/2] Compiling...%NC%
rem Execute the internal build script inside the ARM64 cross-compiler container
rem We pass PROJECT_NAME and PROJECT_VERSION as explicit arguments
docker run --rm ^
    -v "%cd%:/workspace" ^
    %DOCKER_IMAGE% ^
    bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh %PROJECT_NAME% %PROJECT_VERSION%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo %RED%❌ Compilation Failed.%NC%
    pause
    exit /b
)

echo.
echo %GREEN%✅ Done! %NC%
echo    Binary location: export/%PROJECT_NAME%
echo.
echo    %YELLOW%[POST-BUILD REQUIREMENT]%NC%
echo    Ensure you have your patched 'libSDL2.so' in the 'libs' subfolder 
echo    on the target device to satisfy the RPATH configuration.
pause