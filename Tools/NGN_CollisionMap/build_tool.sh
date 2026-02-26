#!/usr/bin/env bash

# ==============================================================================
# N'GINE SDK - Universal Utility Builder (Linux/UNIX Version)
# ==============================================================================

# --- [1] CONFIGURATION ---
TOOL_NAME="NGN_CollisionMap"
LINUX_IMAGE="ngn-linux-pc-builder"
RPI_IMAGE="ngn-rpi4-builder_production"

# Determine the number of CPU cores for parallel building
CORES=$(nproc 2>/dev/null || echo 1)

clear

echo "========================================================"
echo " N'GINE SDK - Multi-Platform Utility Builder"
echo " Tool: $TOOL_NAME"
echo "========================================================"
echo ""
echo "Select Target Platform:"
echo "[1] Linux x86_64 (Docker)"
echo "[2] Linux x86_64 (Local Native)"
echo "[3] Raspberry Pi (Docker)"
echo "[4] Raspberry Pi (Local Native)"
echo ""
read -p "Selection (1-4): " CHOICE

# --- [2] TARGETS ---
case "$CHOICE" in
    1)
        echo "[INFO] Launching Linux x86_64 Docker Build..."
        docker run --rm --platform linux/amd64 -v "$(pwd):/workspace" "$LINUX_IMAGE" bash scripts/internal_build.sh "$TOOL_NAME" "Linux_x86_64"
        ;;
    2)
        echo "[INFO] Building for Linux x86_64 (Local Native)..."
        mkdir -p build_linux
        cd build_linux || exit
        
        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_ARCH_TYPE=Linux_x86_64 -DTOOL_NAME="$TOOL_NAME"
        cmake --build . --parallel "$CORES"
        ;;
    3)
        echo "[INFO] Launching Raspberry Pi Docker Build..."
        docker run --rm -v "$(pwd):/workspace" "$RPI_IMAGE" bash scripts/internal_build.sh "$TOOL_NAME" "Raspberry"
        ;;
    4)
        echo "[INFO] Building for Raspberry Pi (Local Native)..."
        mkdir -p build_rpi
        cd build_rpi || exit
        
        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_ARCH_TYPE=Raspberry -DTOOL_NAME="$TOOL_NAME"
        cmake --build . --parallel "$CORES"
        ;;
    *)
        echo "[ERROR] Invalid selection."
        exit 1
        ;;
esac

# --- [3] FINISH ---
echo ""
echo "========================================================"
echo "Process Complete. Check /exports/ folder."

# Equivalent to 'pause' in Windows
read -n 1 -s -r -p "Press any key to continue..."
echo ""
exit 0