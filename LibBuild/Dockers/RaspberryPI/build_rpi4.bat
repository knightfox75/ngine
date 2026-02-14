@echo off
title N'GINE - UPDATE SDK (Raspberry Pi 400) + EXPORT
cls

:: ============================================================================
:: CONFIGURACION DE ENTORNO
:: ============================================================================
set "BUILDER_IMAGE=ngn-rpi4-builder"
set "DOCKER_FILE=Dockerfile.ngine-rpi4"
set "EXPORT_DIR=lib_export"
set "TAR_NAME=ngn_sdk_rpi4.tar"

echo ============================================================================
echo  N'GINE SDK BUILDER (THE FORGE)
echo  Target: Raspberry Pi 400 (ARM64 / Debian Trixie)
echo ============================================================================
echo.

:: ----------------------------------------------------------------------------
:: PASO 1: CONSTRUCCION DE LA IMAGEN
:: ----------------------------------------------------------------------------
echo 🐳 [1/3] Building/Updating Docker Image...
echo    This may take a while if sources have changed.
echo.

docker build --platform linux/arm64 -t %BUILDER_IMAGE% -f %DOCKER_FILE% .

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ [ERROR] Failed to build the Docker image.
    pause
    exit /b 1
)

:: ----------------------------------------------------------------------------
:: PASO 2: VERIFICACION DE LA VERSION
:: ----------------------------------------------------------------------------
echo.
echo 🔍 [2/3] Verifying Installed Version...
echo    Installed in Container: 
docker run --rm --platform linux/arm64 %BUILDER_IMAGE% pkg-config --modversion ngn

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ⚠️ [WARNING] Version check failed. Check if SDK installed correctly.
)

:: ----------------------------------------------------------------------------
:: PASO 3: EXPORTACION OPCIONAL
:: ----------------------------------------------------------------------------
echo.
set "export_choice="
set /p export_choice=">> Do you want to export the SDK to a .tar file? (y/N): "

if /i "%export_choice%"=="y" (
    echo.
    echo 📦 [3/3] Exporting SDK artifacts...
    
    :: Crear carpeta si no existe
    if not exist "%EXPORT_DIR%" (
        echo    Creating directory %EXPORT_DIR%...
        mkdir "%EXPORT_DIR%"
    )

    :: Ejecutar contenedor para empaquetado
    :: Empaquetamos: Headers, Binarios, CMake Config y Pkg-config
    docker run --rm ^
      --platform linux/arm64 ^
      -v "%cd%/%EXPORT_DIR%:/export" ^
      %BUILDER_IMAGE% ^
      bash -c "tar -cvf /export/%TAR_NAME% /usr/local/include/NGN /usr/local/lib/libngn* /usr/local/lib/cmake/NGN /usr/local/lib/pkgconfig/ngn.pc"

    :: Comprobar éxito de la exportación
    if NOT ERRORLEVEL 1 (
        echo.
        echo ✅ Export complete: %EXPORT_DIR%\%TAR_NAME%
    ) else (
        echo.
        echo ❌ [ERROR] Export failed during tar process.
    )
) else (
    echo.
    echo ⏭️  Skipping export.
)

echo.
echo ============================================================================
echo  PROCESS FINISHED
echo ============================================================================
pause