@echo off
setlocal
rem ============================================================================
rem Clean Script - Windows Version
rem ============================================================================

set "BASE_DIR=%~dp0"
echo Cleaning active build artifacts in: %BASE_DIR%
echo ---------------------------------------

rem 1. Remove all directories starting with "build"
for /d %%d in ("%BASE_DIR%build*") do (
    echo    [REMOVING] %%~nxd...
    rd /s /q "%%d"
)

rem 2. Remove specific artifact directories
for %%d in (temp exports) do (
    if exist "%BASE_DIR%%%d" (
        echo    [REMOVING] %%d...
        rd /s /q "%BASE_DIR%%%d"
    )
)

echo ---------------------------------------
echo Project is clean.
pause