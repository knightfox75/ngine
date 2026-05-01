#!/bin/bash
# ==============================================================================
# N'GINE SDK - RPI4 PRODUCTION BUILDER (Linux Host)
# Target:      Raspberry Pi 4 (Cortex-A72 / ARM64)
# Description: Cross-compilation via Docker container.
# ==============================================================================

# 1. Define ANSI Colors
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Robust Pause Function
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    read -n 1 -s -r < /dev/tty
}

# 2. Configuration Constants
PROJECT_NAME="Sample_Project"
PROJECT_VERSION="0.1.0.0"
DOCKER_IMAGE="ngn-rpi4-builder_production"

clear
echo -e "${BLUE}========================================================${NC}"
echo -e "${GREEN}   N'GINE SDK - RPI4 PRODUCTION BUILDER (LINUX HOST)    ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo ""
echo -e "   Target:  ${YELLOW}Raspberry Pi 4 (ARM64)${NC}"
echo "   Project: $PROJECT_NAME v$PROJECT_VERSION"
echo ""

# --- STEP 1: PRE-FLIGHT CHECK ---
echo -e "🔍 ${BLUE}[1/2] Verifying Workspace...${NC}"

if [ ! -f "scripts/build_internal.sh" ]; then
    echo -e "   ${RED}❌ Error: Build script 'scripts/build_internal.sh' not found.${NC}"
    echo -e "   ${YELLOW}Ensure you are running this from the project root.${NC}"
    pause_script
    exit 1
fi
echo "   - Internal script found."

# --- STEP 2: LAUNCH BUILD CONTAINER ---
echo ""
echo -e "🚀 ${BLUE}[2/2] Initializing Cross-Compilation Environment...${NC}"
echo "   - Target Image: $DOCKER_IMAGE"
echo "   - Workspace:    $(pwd)"

# We pass exactly 2 arguments: Name, Version
docker run --rm -v "$(pwd):/workspace" "$DOCKER_IMAGE" \
    bash scripts/build_internal.sh "$PROJECT_NAME" "$PROJECT_VERSION"

# Check exit code
if [ $? -ne 0 ]; then
    echo ""
    echo -e "${RED}========================================================${NC}"
    echo -e "${RED}❌ [CRITICAL ERROR] Build Pipeline Failed                 ${NC}"
    echo -e "${RED}   Review the Docker logs above for details.            ${NC}"
    echo -e "${RED}========================================================${NC}"
    pause_script
    exit 1
fi

# --- STEP 3: SUCCESS REPORTING ---
echo ""
echo -e "${GREEN}========================================================${NC}"
echo -e "${GREEN}✅ BUILD COMPLETE!                                      ${NC}"
echo -e "${GREEN}   RPi4 binaries located in: export/                    ${NC}"
echo -e "${GREEN}========================================================${NC}"
pause_script
exit 0