#!/bin/bash

# ==============================================================================
# N'GINE - RG35XX Project Builder (Linux Host)
# Target: ARM64 (Cortex-A53)
# ==============================================================================
PROJECT_NAME="Sample_Project"
PROJECT_VERSION="0.1.0.0"
DOCKER_IMAGE="ngn-rg35xx-builder"

# Color Definitions
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Robust Pause Function
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    read -n 1 -s -r < /dev/tty
}

clear
echo -e "${BLUE}----------------------------------------------------${NC}"
echo -e "${GREEN}  $PROJECT_NAME - RG35XX Hybrid Builder  ${NC}"
echo -e "  Version: $PROJECT_VERSION"
echo -e "${BLUE}----------------------------------------------------${NC}"
echo ""

# Check for the existence of the Forge Image
docker image inspect $DOCKER_IMAGE >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Docker Image '$DOCKER_IMAGE' not found.${NC}"
    pause_script
    exit 1
fi

echo -e "🚀 ${BLUE}Compiling project...${NC}"
# --- DOCKER EXECUTION ---
# We pass PROJECT_NAME and PROJECT_VERSION
docker run --rm \
    --user $(id -u):$(id -g) \
    -v "$(pwd):/workspace" \
    $DOCKER_IMAGE \
    bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh $PROJECT_NAME $PROJECT_VERSION"

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ Compilation Error.${NC}"
    pause_script
    exit 1
fi

echo -e "\n${GREEN}✅ Completed.${NC}"
echo -e "   Binary located at: export/$PROJECT_NAME"
echo -e "   NOTE: Ensure the 'libs' folder with libSDL2.so is adjacent to the binary."
pause_script