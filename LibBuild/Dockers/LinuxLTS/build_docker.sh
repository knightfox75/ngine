#!/bin/bash

# ==============================================================================
# N'GINE SDK - LINUX PC BUILDER (Production/Release)
# Target: x86_64 Linux / SteamDeck
# Feature: Build + Verify + Optional Image Export
# ==============================================================================

# 1. Color Definitions for Visual Feedback
GREEN='\033[1;32m'
BLUE='\033[1;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Configuration Constants
BUILDER_IMAGE="ngn-linux-pc-builder_production"
DOCKER_FILE="Dockerfile.ngine-linux-pc"
IMAGE_TAR_NAME="ngn-linux-pc-builder_production.tar"

# 2. Robust Pause Function
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    read -n 1 -s -r < /dev/tty
}

clear
echo -e "${BLUE}========================================================${NC}"
echo -e "${GREEN} N'GINE SDK - LINUX PC BUILDER (Production)${NC}"
echo -e "${BLUE}========================================================${NC}"

# --- STEP 1: BUILD IMAGE ---
echo -e "\n🐳 ${BLUE}[1/3] Initializing Docker Build Process (Production)...${NC}"

# Building for linux/amd64 explicitly to ensure compatibility
docker build --platform linux/amd64 -t "$BUILDER_IMAGE" -f "$DOCKER_FILE" .

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [CRITICAL ERROR] Docker build failed. Inspect the logs above.${NC}"
    pause_script
    exit 1
fi

# --- STEP 2: VERIFY ENVIRONMENT ---
echo -e "\n🔍 ${BLUE}[2/3] Verifying Production Environment Integrity...${NC}"

# Check if the 'ngn' library is correctly registered within the container
docker run --rm --platform linux/amd64 "$BUILDER_IMAGE" pkg-config --modversion ngn

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [VERIFICATION ERROR] 'ngn' package not found within the built image.${NC}"
    pause_script
    exit 1
fi

# --- STEP 3: OPTIONAL EXPORT ---
echo -e "\n📦 ${BLUE}[3/3] Export Docker Image to .tar for distribution?${NC}"
read -p ">> Do you want to export the builder image? (y/N): " EXPORT_CHOICE

if [[ "$EXPORT_CHOICE" =~ ^[Yy]$ ]]; then
    echo -e "\n💾 Exporting Docker Image to filesystem..."
    echo "This may take a moment..."
    
    docker save -o "$IMAGE_TAR_NAME" "$BUILDER_IMAGE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ SUCCESS: Image exported to: $IMAGE_TAR_NAME${NC}"
        # Ensure file permissions are correct for the user
        chmod 644 "$IMAGE_TAR_NAME"
    else
        echo -e "${RED}⚠️ [EXPORT ERROR] Failed to save .tar file. Check disk space.${NC}"
    fi
else
    echo -e "\n⏭️  Export phase skipped by user choice."
fi

echo -e "\n${GREEN}✅ Pipeline completed successfully.${NC}"
pause_script