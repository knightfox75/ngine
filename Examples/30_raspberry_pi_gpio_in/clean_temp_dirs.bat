@echo off
rem Clean Script
set BASE_DIR=%~dp0
echo Cleaning active build artifacts in: %BASE_DIR%
echo ---------------------------------------

rem Eliminar las carpetas activas
for %%d in (build temp) do (
    if exist "%BASE_DIR%%%d" (
        echo    [REMOVING] %%d...
        rd /s /q "%BASE_DIR%%%d"
    )
)

echo ---------------------------------------
echo Project is clean.
pause