@echo off
setlocal
title Sample_Project - Windows Build Specialist

rem ==============================================================================
rem Sample_Project - Windows Build System
rem Architecture: Windows x86_64 (Fixed)
rem Default Mode: DEBUG
rem ==============================================================================

:START
cls
echo ====================================================
echo        Sample_Project - Windows Build System          
echo        Target: Windows (x86_64) - MinGW
echo ====================================================

rem 1. SELECCIÓN DE MODO (Default = Debug)
echo.
echo [Build Mode]
echo 1) Debug   (Default - Symbols enabled)
echo 2) Release (Optimized - O3/LTO)
echo.
set /p m_choice="Selection [Enter for Debug]: "

rem Lógica de "Default": Si es 2, Release. Cualquier otra cosa (incluido vacío), Debug.
if "%m_choice%"=="2" (
    set "MODE_VAL=Release"
) else (
    set "MODE_VAL=Debug"
)

rem 2. CONFIGURACIÓN
set "ARCH_VAL=Windows_x86_64"
set "ARCH_TAG=Windows"
set "BUILD_DIR=build\%ARCH_TAG%-%MODE_VAL%"

echo.
echo ----------------------------------------------------
echo Configuring for: %ARCH_VAL%
echo Mode:            %MODE_VAL%
echo Path:            %BUILD_DIR%
echo ----------------------------------------------------

rem Limpieza y creación de directorio
if exist "%BUILD_DIR%" rd /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

rem 3. CMAKE Y COMPILACIÓN
echo.
echo [1/2] Running CMake configuration...
cmake ../.. -G "MinGW Makefiles" -DCMAKE_ARCH_TYPE="%ARCH_VAL%" -DCMAKE_BUILD_TYPE="%MODE_VAL%"

if %ERRORLEVEL% neq 0 (
    echo.
    color 4F
    echo [ERROR] CMake Configuration failed.
    pause
    color 07
    exit /b %ERRORLEVEL%
)

echo.
echo [2/2] Compiling binary...
cmake --build . -j %NUMBER_OF_PROCESSORS%

if %ERRORLEVEL% neq 0 (
    echo.
    color 4F
    echo [ERROR] Build failed.
    pause
    color 07
    exit /b %ERRORLEVEL%
)

rem 4. FINALIZACIÓN
cd ../..
echo.
echo ====================================================
echo    BUILD SUCCESSFUL!
echo    Executable: %BUILD_DIR%\Sample_Project.exe
echo ====================================================
pause