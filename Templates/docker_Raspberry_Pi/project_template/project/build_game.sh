#!/bin/bash

# ==============================================================================
# N'GINE SDK - RPi4 Production Builder (Linux Launcher)
# ==============================================================================
# Description: Cross-compiles project for Raspberry Pi 4 (ARM64).
# Target:      Raspberry Pi 4 (Cortex-A72 / ARM64)
# ==============================================================================

PROJECT_NAME="Sample_Project"
PROJECT_VERSION="1.0.0"
DOCKER_IMAGE="ngn-rpi4-builder_production"

pause_exit() {
    local exit_code=$1
    echo ""
    echo "--------------------------------------------------------"
    read -n 1 -s -r -p "Process complete. Press any key to exit..." < /dev/tty
    echo ""
    exit "$exit_code"
}

clear
echo "========================================================"
echo " N'GINE SDK - RPi4 Production Build Launcher"
echo "========================================================"
echo ""

if [[ ! -f "scripts/build_internal.sh" ]]; then
    echo "CRITICAL ERROR: 'scripts/build_internal.sh' not found."
    pause_exit 1
fi

echo "[1/2] Initializing Cross-Compilation Environment..."
# Note: Removed --platform linux/amd64 as the container runs natively on PC
if ! docker run --rm -v "$(pwd):/workspace" "$DOCKER_IMAGE" bash scripts/build_internal.sh "$PROJECT_NAME" "$PROJECT_VERSION"; then
    echo ""
    echo "CRITICAL ERROR: Build process failed."
    pause_exit 1
fi

echo ""
echo "[2/2] Build Successful!"
echo "RPi4 binaries are in 'export/'."
pause_exit 0