@echo off
rem Clean Script
set BASE_DIR=%~dp0
echo Cleaning active build artifacts in: %BASE_DIR%
echo ---------------------------------------

rem Eliminar carpetas activas (build y temp)
for %%d in (build temp build_rg35xx_docker export) do (
    if exist "%BASE_DIR%%%d" (
        echo    [REMOVING] %%d...
        rd /s /q "%BASE_DIR%%%d"
    )
)

rem 3. Recrear el directorio temporal
if not exist "%BASE_DIR%temp" mkdir "%BASE_DIR%temp"

echo ---------------------------------------
echo Project is clean. Fresh build required.
pause