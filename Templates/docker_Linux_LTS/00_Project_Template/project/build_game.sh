#!/bin/bash
# ==============================================================================
# N'GINE SDK - GAME PRODUCTION BUILDER (Linux Host Launcher)
# Target:      x86_64 Linux / SteamDeck (Release)
# Description: Cross-compilation orchestrator via Docker container.
# ==============================================================================

# 1. Define ANSI Colors
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 2. Configuration Constants
PROJECT_NAME="Project_Sample"
PROJECT_VERSION="0.1.0.0"
DOCKER_IMAGE="ngn-linux-pc-builder_production"

clear
echo -e "${BLUE}========================================================${NC}"
echo -e "${GREEN}  N'GINE SDK - LINUX PRODUCTION BUILDER (NATIVE HOST)   ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo ""

# --- STEP 1: PRE-FLIGHT CHECK ---
echo -e "🔍 ${BLUE}[1/2] Verifying Workspace...${NC}"
if [ ! -f "scripts/build_internal.sh" ]; then
    echo -e "${RED}❌ [CRITICAL] Build script 'scripts/build_internal.sh' not found.${NC}"
    echo -e "${RED}   Ensure you are running this from the project root.${NC}"
    exit 1
fi

# --- STEP 2: LAUNCH BUILD CONTAINER ---
echo ""
echo -e "🐳 ${BLUE}[2/2] Initializing Production Environment...${NC}"
echo "   Target Image: $DOCKER_IMAGE"
echo ""

# Mounting $(pwd) to /workspace. Passing Args: Name, Version
# Using --platform linux/amd64 to ensure correct architecture emulation if running on ARM (Mac M1/M2)
docker run --rm --platform linux/amd64 -v "$(pwd):/workspace" "$DOCKER_IMAGE" \
    bash scripts/build_internal.sh "$PROJECT_NAME" "$PROJECT_VERSION"

# Check exit code of the last command (docker)
if [ $? -ne 0 ]; then
    echo ""
    echo -e "${RED}========================================================${NC}"
    echo -e "${RED}⚠️ [CRITICAL] PIPELINE FAILED                            ${NC}"
    echo -e "${RED}   Review the Docker logs above for compilation errors. ${NC}"
    echo -e "${RED}========================================================${NC}"
    exit 1
fi

# --- STEP 3: SUCCESS REPORTING ---
echo ""
echo -e "${GREEN}========================================================${NC}"
echo -e "${GREEN}✅ BUILD SUCCESSFUL!                                    ${NC}"
echo -e "${GREEN}   Standalone binaries located in: 'export/'            ${NC}"
echo -e "${GREEN}========================================================${NC}"
exit 0