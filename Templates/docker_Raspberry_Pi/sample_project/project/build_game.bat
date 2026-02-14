@echo off
set "PROJECT_NAME=Sample_Project"
set "DOCKER_IMAGE=ngn-rpi4-builder"

title %PROJECT_NAME% - RPi4 Full Bundler
cls

echo 🚀 Launching Build ^& Recursive Bundling...

docker run --rm ^
  --platform linux/arm64 ^
  -v "%cd%:/workspace" ^
  %DOCKER_IMAGE% ^
  bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh %PROJECT_NAME%"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ [ERROR] Process failed.
    pause
    exit /b
)

echo.
echo ✅ Done! Check 'game_export/' folder.
pause