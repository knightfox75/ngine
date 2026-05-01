#!/bin/bash

# ==============================================================================
# N'GINE - SOURCE INSTALLER (UPDATE ONLY)
# ==============================================================================
# Description: Quick update for N'GINE from local source (No dependencies check)
# Target: PC (x86_64) & Raspberry Pi (ARM64)
# ==============================================================================

# Color Definitions
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- AUTOMATIC ARCHITECTURE DETECTION ---
ARCH_SYSTEM=$(uname -m)
if [ "$ARCH_SYSTEM" == "x86_64" ]; then
    TARGET_ARCH="x86"
    MSG="PC Desktop (x86_64)"
elif [ "$ARCH_SYSTEM" == "aarch64" ]; then
    TARGET_ARCH="raspberry"
    MSG="Raspberry Pi (ARM64)"
else
    echo -e "${RED}❌ Unknown architecture: $ARCH_SYSTEM${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read -r
    exit 1
fi

clear
echo -e "${BLUE}====================================================${NC}"
echo -e "${GREEN}   N'GINE - SOURCE UPDATE UTILITY                   ${NC}"
echo -e "${BLUE}====================================================${NC}"
echo -e "Target System: ${YELLOW}$MSG${NC}"
echo -e "${BLUE}====================================================${NC}"

# --- SAFE CLEANUP FUNCTION ---
function clean_previous_install() {
    echo -e "\n${YELLOW}🧹 [MAINTENANCE] Cleaning old NGN versions from /usr/local...${NC}"
    sudo rm -rf /usr/local/include/NGN
    sudo rm -f /usr/local/lib/libngn*
    sudo rm -rf /usr/local/lib/cmake/NGN
    sudo rm -f /usr/local/lib/pkgconfig/ngn.pc
}

# --- BUILD ENVIRONMENT PREPARATION ---
echo -e "\n${BLUE}[1/3] Preparing Build Environment...${NC}"
BUILD_DIR="build_$TARGET_ARCH"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Quick cache clean inside build folder
rm -rf * 

# --- CMAKE CONFIGURATION ---
echo -e "${YELLOW}⚙️  Configuring CMake...${NC}"
cmake .. -DARCH=$TARGET_ARCH

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [ERROR] Configuration failed.${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read -r
    exit 1
fi

# --- COMPILATION ---
echo -e "\n${YELLOW}🚀 [2/3] Compiling N'GINE...${NC}"
cmake --build . -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [ERROR] Build failed.${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read -r
    exit 1
fi

# --- INSTALLATION ---
echo -e "\n${BLUE}[3/3] Ready to install. Sudo password required.${NC}"
clean_previous_install

echo -e "${YELLOW}💾 Installing binaries...${NC}"
sudo cmake --install .

# Update system library cache immediately
echo "🔄 Updating system library cache..."
sudo ldconfig

# --- FINAL STATUS ---
if [ $? -eq 0 ]; then
    echo -e "\n${GREEN}====================================================${NC}"
    echo -e "${GREEN}   ✅ SUCCESS! N'GINE Updated.                      ${NC}"
    echo -e "${GREEN}====================================================${NC}"
    echo -n "Current Version: "
    pkg-config --modversion ngn
else
    echo -e "\n${RED}❌ [ERROR] Installation failed.${NC}"
fi

# Return to original directory
cd ..

echo -e "\n${YELLOW}Press Enter to exit...${NC}"
read -r