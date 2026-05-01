@echo off
setlocal
title Sample_Project - Windows Build Specialist

rem ==============================================================================
rem SAMPLE_PROJECT - WINDOWS BUILD SYSTEM
rem Target: Windows x86_64 (MinGW)
rem Feature: Build Mode Selection + CMake Integration
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
rem 1. Define ANSI Colors (Windows 10/11 Compatible)
rem This trick captures the Escape character into the variable ESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "ESC=%%b"
)
set "GREEN=%ESC%[1;32m"
set "BLUE=%ESC%[1;34m"
set "YELLOW=%ESC%[1;33m"
set "RED=%ESC%[0;31m"
set "NC=%ESC%[0m"

cls
echo %BLUE%====================================================%NC%
echo %GREEN%      SAMPLE_PROJECT - WINDOWS BUILD SYSTEM           %NC%
echo %BLUE%      Target: %YELLOW%Windows (x86_64) - MinGW%BLUE%                 %NC%
echo %BLUE%====================================================%NC%

rem --- STEP 2: BUILD MODE SELECTION ---
echo.
echo %YELLOW%[Build Mode Selection]%NC%
echo 1) Debug   (Default - Symbols enabled)
echo 2) Release (Optimized - O3/LTO)
echo.
set /p m_choice="Select Build Mode [1-2] (Default: Debug): "

rem Logic preservation: Default to Debug if input is not '2'
if "%m_choice%"=="2" (
    set "MODE_VAL=Release"
) else (
    set "MODE_VAL=Debug"
)

rem --- STEP 3: PATH CONFIGURATION ---
set "ARCH_VAL=Windows_x86_64"
set "ARCH_TAG=Windows"
set "BUILD_DIR=build\%ARCH_TAG%-%MODE_VAL%"

echo.
echo 📂 %BLUE%Configuring build directory: %GREEN%%BUILD_DIR%%NC%

rem Clean previous build artifacts for this specific configuration
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

rem --- STEP 4: CMAKE CONFIGURATION ---
echo.
echo 🔧 %BLUE%Configuring project with CMake (MinGW)...%NC%

cmake ../.. -G "MinGW Makefiles" -DCMAKE_ARCH_TYPE="%ARCH_VAL%" -DCMAKE_BUILD_TYPE="%MODE_VAL%"

if %ERRORLEVEL% neq 0 (
    echo.
    echo %RED%❌ Error: CMake Configuration Failed.%NC%
    echo %RED%   Check if MinGW is in your PATH.%NC%
    pause
    exit /b %ERRORLEVEL%
)

rem --- STEP 5: COMPILATION ---
echo.
echo 🚀 %BLUE%Compiling binary...%NC%

rem Uses %NUMBER_OF_PROCESSORS% to match 'nproc' behavior
cmake --build . -j %NUMBER_OF_PROCESSORS%

if %ERRORLEVEL% neq 0 (
    echo.
    echo %RED%❌ Error: Build Failed during compilation.%NC%
    pause
    exit /b %ERRORLEVEL%
)

rem --- STEP 6: FINAL STATUS ---
cd ../..
echo.
echo %GREEN%====================================================%NC%
echo %GREEN%✅ BUILD SUCCESSFUL!                                %NC%
echo %GREEN%   Executable: %BUILD_DIR%\Sample_Project.exe         %NC%
echo %GREEN%====================================================%NC%

echo.
echo %BLUE%Press any key to close...%NC%
pause >nul