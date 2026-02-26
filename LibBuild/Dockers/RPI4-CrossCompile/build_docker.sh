#!/bin/bash

# ==============================================================================
# N'GINE - RPi4 RELEASE CROSS-COMPILER (Linux Host)
# Target: Raspberry Pi 4 (ARM64)
# ==============================================================================

# Set terminal title
echo -e "\033]0;N'GINE - RPi4 Release Forge\007"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

IMAGE_NAME="ngn-rpi4-builder_production"
DOCKER_FILE="Dockerfile.ngn-rpi4-release"
TAR_NAME="ngn-rpi4-builder_production.tar"

clear
echo -e "${BLUE}============================================================================${NC}"
echo -e "${BLUE} N'GINE RPi4 MASTER FORGE (CROSS-COMPILER)${NC}"
echo -e "${BLUE} Mode: RELEASE (Optimized -O3, Stripped)${NC}"
echo -e "${BLUE}============================================================================${NC}"
echo

# ------------------------------------------------------------------------------
# STEP 1: BUILD IMAGE
# ------------------------------------------------------------------------------
echo -e "🐳 [1/2] ${YELLOW}Building Cross-Compiler Image...${NC}"
echo -e "   Using host CPU for maximum speed."

# IMPORTANT: No --platform flag. Running native x86.
if ! docker build -t "$IMAGE_NAME" -f "$DOCKER_FILE" . ; then
    echo -e "\n${RED}❌ [ERROR] Failed to build the Docker image.${NC}"
    read -p "Press [Enter] to exit..." < /dev/tty
    exit 1
fi

# ------------------------------------------------------------------------------
# STEP 2: VERIFICATION
# ------------------------------------------------------------------------------
echo -e "\n🔍 [2/2] ${YELLOW}Verifying NGN Lib Installation...${NC}"

if docker run --rm "$IMAGE_NAME" bash -c "ls -lh /usr/aarch64-linux-gnu/lib/libngn.a"; then
    echo -e "${GREEN}✅ NGN Static Lib found and ready.${NC}"
else
    echo -e "${YELLOW}⚠️  Warning: Library verification failed.${NC}"
fi

# ------------------------------------------------------------------------------
# STEP 3: OPTIONAL EXPORT
# ------------------------------------------------------------------------------
echo
read -p ">> Do you want to export the Docker Image to a .tar file? (y/N): " export_choice < /dev/tty

if [[ "$export_choice" =~ ^[Yy]$ ]]; then
    echo -e "\n💾 ${YELLOW}Exporting Docker Image...${NC}"
    
    if docker save -o "$TAR_NAME" "$IMAGE_NAME"; then
        echo -e "${GREEN}✅ Image exported successfully.${NC}"
        chmod 644 "$TAR_NAME"
    else
        echo -e "\n${RED}❌ Failed to export Docker image.${NC}"
    fi
else
    echo -e "\n${NC}⏭️  Skipping export.${NC}"
fi

echo -e "\n${BLUE}============================================================================${NC}"
echo -e "${BLUE} PROCESS FINISHED${NC}"
echo -e "${BLUE}============================================================================${NC}"
read -p "Press [Enter] to exit..." < /dev/tty