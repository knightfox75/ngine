@echo off
setlocal enableextensions enabledelayedexpansion

rem ==============================================================================
rem N'GINE LIBRARY BUILDER - Windows MinGW (x86_64)
rem Target: Windows Native (System Path Edition)
rem Feature: Native CMake/MinGW Build Automation
rem ==============================================================================

rem --- UTF-8 SAFETY JUMP ---
rem Changes codepage to UTF-8 and immediately jumps to label to prevent 
rem buffer read errors common in batch files with emojis.
chcp 65001 >nul
goto :START_LOGIC

:START_LOGIC
title N'GINE - Windows MinGW Forge
cls

echo ============================================================
echo  N'GINE LIBRARY BUILDER - Windows MinGW (x86_64)
echo ============================================================
echo.

rem --- 1. ENVIRONMENT CHECK ---
rem We no longer hardcode paths. We rely on the System PATH (MSYS2/MinGW64).
rem This step verifies that the necessary tools are accessible before proceeding.
echo 🔍 [1/4] Checking Environment (System PATH)...
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 goto :error_tools

where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 goto :error_tools

echo    ✅ Tools detected successfully (g++, cmake).

rem --- 2. CPU CORE DETECTION ---
rem Maximize build performance by utilizing all available logical processors.
rem Fallback to 2 threads if the environment variable is somehow missing.
echo.
echo ⚙️  [2/4] Detecting CPU Cores...
set "THREADS=%NUMBER_OF_PROCESSORS%"
if "%THREADS%"=="" (
    set "THREADS=2"
)
echo    ✅ Detected %THREADS% logical threads for compilation.

rem --- 3. BUILD DIRECTORY INITIALIZATION ---
rem Enforce out-of-source builds to keep the root source tree clean.
echo.
echo 📂 [3/4] Initializing Build Directory...
if not exist "build" (
    mkdir build
)
cd build || goto :error_cd
echo    ✅ Build directory ready.

rem --- 4. CMAKE CONFIGURATION ---
rem Generate MinGW Makefiles pointing to the parent directory (root).
rem -DCMAKE_COLOR_MAKEFILE=OFF disables ANSI color codes to prevent raw escape 
rem codes from appearing in the standard Windows CMD buffer.
echo.
echo ⚙️  [4/4] Running CMake Configuration...
cmake -G "MinGW Makefiles" -DCMAKE_COLOR_MAKEFILE=OFF ..
if %ERRORLEVEL% neq 0 goto :error_cmake

rem --- 5. PARALLEL COMPILATION ---
rem Execute the build process using the detected number of CPU threads.
rem Note: CMake's native --parallel flag handles the threading correctly 
rem without Mingw32 mutex sync issues.
echo.
echo 🚀 Compiling Binaries... (Using %THREADS% threads)
cmake --build . --parallel %THREADS%
if %ERRORLEVEL% neq 0 goto :error_build

rem --- 6. SUCCESSFUL COMPLETION ---
echo.
echo ============================================================
echo  ✅ SUCCESS: Compilation finished flawlessly.
echo  The binaries and headers have been exported to the "export" directory.
echo ============================================================
goto :end


rem ==============================================================================
rem ERROR HANDLING LABELS
rem ==============================================================================

:error_tools
echo.
echo ============================================================
echo  ❌ CRITICAL ERROR: Build tools not found in PATH.
echo  Please ensure MinGW64 (g++) and CMake are installed 
echo  and added to your System PATH.
echo ============================================================
goto :end

:error_cd
echo.
echo ============================================================
echo  ❌ CRITICAL ERROR: Failed to create or enter "build" folder.
echo ============================================================
goto :end

:error_cmake
echo.
echo ============================================================
echo  ❌ CRITICAL ERROR: CMake configuration failed. 
echo  Check the logs above.
echo ============================================================
goto :end

:error_build
echo.
echo ============================================================
echo  ❌ CRITICAL ERROR: Compilation failed. 
echo  Check the build logs above.
echo ============================================================
goto :end

:end
echo.
echo Press any key to close...
pause >nul
endlocal