#!/bin/bash

# ==============================================================================
# SAMPLE_PROJECT - AUTO BUILD SYSTEM
# Target: Auto-Detect (x86_64 / ARM64)
# Feature: Architecture Detection + CMake Build
# ==============================================================================

# 1. Color Definitions for Visual Feedback
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 2. Robust Pause Function (TTY Fix)
# Forces reading from the physical terminal, preventing closures in GUI environments.
pause_script() {
    echo -e "\n${BLUE}Press any key to close...${NC}"
    # Explicitly redirect input from /dev/tty
    read -n 1 -s -r < /dev/tty
}

# --- STEP 1: ARCHITECTURE AUTO-DETECTION ---
ARCH_SYSTEM=$(uname -m)

if [ "$ARCH_SYSTEM" == "x86_64" ]; then
    # PC Desktop Environment
    ARCH_VAL="Linux_x86_64"
    ARCH_TAG="Linux"
    MSG_ARCH="PC Desktop (x86_64)"
elif [ "$ARCH_SYSTEM" == "aarch64" ]; then
    # Raspberry Pi 4/5 (64-bit OS)
    ARCH_VAL="Raspberry"
    ARCH_TAG="Raspberry"
    MSG_ARCH="Raspberry Pi (ARM64)"
else
    # Unsupported Architecture
    echo -e "${RED}❌ Error: Unsupported Architecture ($ARCH_SYSTEM)${NC}"
    echo -e "${YELLOW}Only x86_64 and aarch64 are supported.${NC}"
    pause_script
    exit 1
fi

clear
echo -e "${BLUE}====================================================${NC}"
echo -e "${GREEN}      SAMPLE_PROJECT - AUTO BUILD SYSTEM              ${NC}"
echo -e "${BLUE}      Detected: ${YELLOW}$MSG_ARCH${BLUE}                    ${NC}"
echo -e "${BLUE}====================================================${NC}"

# --- STEP 2: BUILD MODE SELECTION ---
echo -e "\n⚙️  ${BLUE}[Build Mode Selection]${NC}"
echo -e "1) Debug   (Default - Symbols enabled)"
echo -e "2) Release (Optimized - O3/LTO)"
read -p "Select Build Mode [1-2] (Default: Debug): " m_choice

# Logic preservation: Default to Debug if input is not '2'
case $m_choice in
    2) MODE_VAL="Release" ;;
    *) MODE_VAL="Debug" ;;
esac

# --- STEP 3: PATH CONFIGURATION ---
BUILD_DIR="build/${ARCH_TAG}-${MODE_VAL}"

echo -e "\n📂 ${BLUE}Configuring build directory: ${GREEN}${BUILD_DIR}${NC}"

# Clean previous build artifacts for this specific configuration
if [ -d "$BUILD_DIR" ]; then 
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# --- STEP 4: CMAKE CONFIGURATION ---
echo -e "\n🔧 ${BLUE}Configuring project with CMake...${NC}"

# Passing detected architecture and mode to CMake
cmake ../.. -DCMAKE_ARCH_TYPE="$ARCH_VAL" -DCMAKE_BUILD_TYPE="$MODE_VAL"

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ Error: CMake Configuration Failed.${NC}"
    cd ../..
    pause_script
    exit 1
fi

# --- STEP 5: COMPILATION ---
echo -e "\n🚀 ${BLUE}Compiling binary...${NC}"

# Auto-detect available CPU cores for parallel compilation
CPU_CORES=$(nproc 2>/dev/null || echo 1)
cmake --build . -j"$CPU_CORES"

# --- STEP 6: FINAL STATUS ---
if [ $? -eq 0 ]; then
    echo -e "\n${GREEN}====================================================${NC}"
    echo -e "${GREEN}✅ BUILD SUCCESSFUL!                                ${NC}"
    echo -e "${GREEN}   Binary location: ${BUILD_DIR}/Sample_Project       ${NC}"
    echo -e "${GREEN}====================================================${NC}"
else
    echo -e "\n${RED}❌ Error: Build Failed during compilation.${NC}"
fi

# Return to root directory before exit
cd ../..
pause_script