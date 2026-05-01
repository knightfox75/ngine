@echo off

set BASE_DIR=%~dp0
echo Cleaning active build artifacts in: %BASE_DIR%
echo ---------------------------------------

for %%d in (build export) do (
    if exist "%BASE_DIR%%%d" (
        echo    [REMOVING] %%d...
        rd /s /q "%BASE_DIR%%%d"
    )
)

echo ---------------------------------------
echo Project is clean. Fresh build required.
pause