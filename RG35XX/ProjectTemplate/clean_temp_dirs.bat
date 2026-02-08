@echo off
rem NightFox - Clean Script v2 (Lean & Modern)
set BASE_DIR=%~dp0
echo Cleaning active build artifacts in: %BASE_DIR%
echo ---------------------------------------

rem 1. Eliminar carpetas activas (build y temp)
for %%d in (build temp build_rg35xx_docker export) do (
    if exist "%BASE_DIR%%%d" (
        echo    [REMOVING] %%d...
        rd /s /q "%BASE_DIR%%%d"
    )
)

rem 2. Eliminar binarios de la raíz
echo    [REMOVING] Root binaries...
if exist "%BASE_DIR%Idunns_Grace.exe" del /f /q "%BASE_DIR%Idunns_Grace.exe"
if exist "%BASE_DIR%Idunns_Grace" del /f /q "%BASE_DIR%Idunns_Grace"

rem 3. Recrear zona de staging
if not exist "%BASE_DIR%temp" mkdir "%BASE_DIR%temp"

echo ---------------------------------------
echo Project is clean. Fresh build required.
pause