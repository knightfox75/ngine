#!/bin/bash

# ==============================================================================
# N'GINE SDK - LINUX LAUNCHER (Release Mode)
# Target: RG35XX (Cortex-A53)
# Feature: Build + Optional Image Export
# ==============================================================================

# 1. Color Definitions for Visual Feedback
GREEN='\033[1;32m'
BLUE='\033[1;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 2. Robust Pause Function (TTY Fix)
# Forces reading from the physical terminal, preventing closures in GUI environments.
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    # Explicitly redirect input from /dev/tty
    read -n 1 -s -r < /dev/tty
}

clear
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}   N'GINE - RG35XX ARM64 BUILDER    ${NC}"
echo -e "${BLUE}========================================${NC}"

# --- STEP 1: BUILD THE IMAGE ---
echo -e "\n🐳 ${BLUE}[1/2] Building/Verifying Forge image...${NC}"

# Use Dockerfile.rg35xx. If it fails, capture the error.
docker build -t ngn-rg35xx-builder -f Dockerfile.rg35xx .

if [ $? -ne 0 ]; then
    echo -e "\n${RED}[ERROR] Failed to build the Docker image.${NC}"
    pause_script
    exit 1
fi

# --- STEP 2: COMPILE THE PROJECT ---
echo -e "\n🚀 ${BLUE}[2/2] Compiling N'GINE Project (RELEASE MODE)...${NC}"
echo "Workdir: $(pwd)"

# Runtime Flags Explanation:
# --rm: Deletes the container upon exit (cleanup).
# -v: Mounts the current directory.
# --user: VITAL on Linux. Maps the Docker user to your local user 
#         so that compiled files are not owned by 'root' and remain editable.
docker run --rm \
    --user $(id -u):$(id -g) \
    -v "$(pwd):/workspace" \
    ngn-rg35xx-builder \
    bash -c "mkdir -p build_rg35xx && cd build_rg35xx && \
    cmake .. \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
    -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
    -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu \
    -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)"

# Compilation error verification
if [ $? -ne 0 ]; then
    echo -e "\n${RED}[ERROR] Compilation has failed.${NC}"
    pause_script
    exit 1
fi

echo -e "\n${GREEN}✅ Process completed. Optimized binary ready in /build_rg35xx${NC}"

# --- STEP 3: OPTIONAL IMAGE EXPORT ---
echo -e "\n📦 ${BLUE}[EXTRA] Image Export Utility${NC}"
read -p "Do you want to export the builder image to a .tar file? (y/N): " EXPORT_CHOICE

# Check if input is 'y' or 'Y'
if [[ "$EXPORT_CHOICE" =~ ^[Yy]$ ]]; then
    echo -e "\n💾 Exporting 'ngn-rg35xx-builder' to 'ngn-rg35xx-builder.tar'..."
    echo "This may take a moment..."
    
    docker save -o ngn-rg35xx-builder.tar ngn-rg35xx-builder
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Image exported successfully as ngn-rg35xx-builder.tar${NC}"
        # Adjusting permissions so the exported tar belongs to the user, not root
        chmod 644 ngn-rg35xx-builder.tar
    else
        echo -e "${RED}⚠️ Error: Failed to export image.${NC}"
    fi
else
    echo -e "\n⏭️  Skipping image export."
fi

pause_script