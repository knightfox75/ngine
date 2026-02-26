@echo off
setlocal

title NGN Library - Windows MinGW Forge (System Path Edition)

echo ============================================================
echo NGN LIBRARY BUILDER - Windows MinGW (x86_64)
echo ============================================================
echo.

:: -----------------------------------------------------------------------------
:: 1. ENVIRONMENT CHECK
:: -----------------------------------------------------------------------------
:: We no longer hardcode paths. We rely on the System PATH (MSYS2/MinGW64).
:: This step verifies that the necessary tools are accessible before proceeding.
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 goto :error_tools

where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 goto :error_tools

echo [INFO] Tools detected successfully (g++, cmake). Using System PATH.

:: -----------------------------------------------------------------------------
:: 2. CPU CORE DETECTION (Auto-Threading)
:: -----------------------------------------------------------------------------
:: Maximize build performance by utilizing all available logical processors.
:: Fallback to 2 threads if the environment variable is somehow missing.
set "THREADS=%NUMBER_OF_PROCESSORS%"
if "%THREADS%"=="" (
    set "THREADS=2"
)
echo [INFO] Detected %THREADS% logical threads for compilation.

:: -----------------------------------------------------------------------------
:: 3. BUILD DIRECTORY INITIALIZATION
:: -----------------------------------------------------------------------------
:: Enforce out-of-source builds to keep the root source tree clean.
if not exist "build" (
    mkdir build
)
cd build || goto :error_cd

:: -----------------------------------------------------------------------------
:: 4. CMAKE CONFIGURATION
:: -----------------------------------------------------------------------------
:: Generate MinGW Makefiles pointing to the parent directory (root).
:: -DCMAKE_COLOR_MAKEFILE=OFF disables ANSI color codes to prevent raw escape 
:: codes (e.g., <-[32m) from appearing in the standard Windows CMD buffer.
echo.
echo [1/2] Running CMake Configuration...
cmake -G "MinGW Makefiles" -DCMAKE_COLOR_MAKEFILE=OFF ..
if %ERRORLEVEL% neq 0 goto :error_cmake

:: -----------------------------------------------------------------------------
:: 5. PARALLEL COMPILATION
:: -----------------------------------------------------------------------------
:: Execute the build process using the detected number of CPU threads.
:: FIX: Removed "-- --output-sync=target" because mingw32-make on Windows
:: often fails with mutex errors when attempting to synchronize output.
:: CMake's native --parallel flag handles the threading correctly.
echo.
echo [2/2] Compiling Binaries... (Using %THREADS% threads)
cmake --build . --parallel %THREADS%
if %ERRORLEVEL% neq 0 goto :error_build

:: -----------------------------------------------------------------------------
:: 6. SUCCESSFUL COMPLETION
:: -----------------------------------------------------------------------------
echo.
echo ============================================================
echo SUCCESS: Compilation finished flawlessly.
echo The binaries and headers have been exported to the "export" directory.
echo ============================================================
goto :end

:: -----------------------------------------------------------------------------
:: ERROR HANDLING LABELS
:: -----------------------------------------------------------------------------
:error_tools
echo.
echo ============================================================
echo CRITICAL ERROR: Build tools not found in PATH.
echo Please ensure MinGW64 (g++) and CMake are installed and added to your System PATH.
echo ============================================================
goto :end

:error_cd
echo.
echo ============================================================
echo CRITICAL ERROR: Failed to create or enter the "build" directory.
echo ============================================================
goto :end

:error_cmake
echo.
echo ============================================================
echo CRITICAL ERROR: CMake configuration failed. Check the logs above.
echo ============================================================
goto :end

:error_build
echo.
echo ============================================================
echo CRITICAL ERROR: Compilation failed. Check the build logs above.
echo ============================================================
goto :end

:end
echo.
pause
endlocal