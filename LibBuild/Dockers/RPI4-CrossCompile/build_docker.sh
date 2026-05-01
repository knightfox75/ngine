#!/bin/bash

# ==============================================================================
# N'GINE - RPi4 RELEASE CROSS-COMPILER (Linux Host)
# Target: Raspberry Pi 4 (ARM64)
# Mode: RELEASE (Optimized -O3, Stripped)
# ==============================================================================

# 1. Color Definitions
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Configuration Constants
IMAGE_NAME="ngn-rpi4-builder_production"
DOCKER_FILE="Dockerfile.ngn-rpi4-release"
TAR_NAME="ngn-rpi4-builder_production.tar"

# 2. Robust Pause Function
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    read -n 1 -s -r < /dev/tty
}

clear
echo -e "${BLUE}============================================================================${NC}"
echo -e "${GREEN} N'GINE RPi4 MASTER FORGE (CROSS-COMPILER)${NC}"
echo -e "${BLUE} Target: Raspberry Pi 4 (ARM64)${NC}"
echo -e "${BLUE} Mode: RELEASE (Optimized -O3, Stripped)${NC}"
echo -e "${BLUE}============================================================================${NC}"
echo

# --- STEP 1: BUILD DOCKER IMAGE ---
echo -e "🐳 [1/2] ${BLUE}Building Cross-Compiler Image...${NC}"
echo -e "   Using host CPU for maximum speed."

# IMPORTANT: No --platform flag. Running native x86.
if ! docker build -t "$IMAGE_NAME" -f "$DOCKER_FILE" . ; then
    echo -e "\n${RED}❌ [ERROR] Failed to build the Docker image.${NC}"
    pause_script
    exit 1
fi

# --- STEP 2: VERIFICATION ---
echo -e "\n🔍 [2/2] ${BLUE}Verifying NGN Lib Installation...${NC}"

# Check if the static library exists within the cross-compilation toolchain path
if docker run --rm "$IMAGE_NAME" bash -c "ls -lh /usr/aarch64-linux-gnu/lib/libngn.a"; then
    echo -e "${GREEN}✅ NGN Static Lib found and ready.${NC}"
else
    echo -e "${YELLOW}⚠️  [WARNING] Library verification failed.${NC}"
fi

# --- STEP 3: OPTIONAL IMAGE EXPORT ---
echo -e "\n📦 ${BLUE}[EXTRA] Docker Image Export${NC}"
read -p ">> Do you want to export the Docker Image to a .tar file? (y/N): " EXPORT_CHOICE

if [[ "$EXPORT_CHOICE" =~ ^[Yy]$ ]]; then
    echo -e "\n💾 ${YELLOW}Exporting Docker Image: $TAR_NAME ...${NC}"
    echo "   Please wait..."
    
    if docker save -o "$TAR_NAME" "$IMAGE_NAME"; then
        echo -e "${GREEN}✅ Image exported successfully.${NC}"
        chmod 644 "$TAR_NAME"
    else
        echo -e "\n${RED}❌ Failed to export Docker image.${NC}"
    fi
else
    echo -e "\n⏭️  Skipping export."
fi

echo -e "\n${BLUE}============================================================================${NC}"
echo -e "${GREEN} READY TO CROSS-COMPILE${NC}"
echo -e "${BLUE}============================================================================${NC}"

pause_script