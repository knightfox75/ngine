#!/bin/bash

# ==============================================================================
# Sample_Project - Linux/Pi Build Specialist
# Auto-Detect Architecture & Build
# ==============================================================================

# Colores
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# ------------------------------------------------------------------------------
# 1. AUTO-DETECCIÓN DE ARQUITECTURA
# ------------------------------------------------------------------------------
ARCH_SYSTEM=$(uname -m)

if [ "$ARCH_SYSTEM" == "x86_64" ]; then
    # PC Desktop
    ARCH_VAL="Linux_x86_64"
    ARCH_TAG="Linux"
    MSG_ARCH="PC Desktop (x86_64)"
elif [ "$ARCH_SYSTEM" == "aarch64" ]; then
    # Raspberry Pi 4/5 (64-bit OS)
    ARCH_VAL="Raspberry"
    ARCH_TAG="Raspberry"
    MSG_ARCH="Raspberry Pi (ARM64)"
else
    # Arquitectura no soportada tras la limpieza
    echo -e "${RED}Error: Unsupported Architecture ($ARCH_SYSTEM)${NC}"
    echo -e "${YELLOW}Only x86_64 and aarch64 are supported.${NC}"
    read -p "Press Enter to exit."
    exit 1
fi

clear
echo -e "${BLUE}====================================================${NC}"
echo -e "${BLUE}      Sample_Project - Auto Build System              ${NC}"
echo -e "${BLUE}      Detected: ${GREEN}$MSG_ARCH${BLUE}                    ${NC}"
echo -e "${BLUE}====================================================${NC}"

# ------------------------------------------------------------------------------
# 2. SELECCIÓN DE MODO (Default: DEBUG)
# ------------------------------------------------------------------------------
echo -e "\n${YELLOW}[Build Mode]${NC}"
echo -e "1) Debug   (Default - Symbols enabled)"
echo -e "2) Release (Optimized - O3/LTO)"
read -p "Selection [1-2] (Enter for Debug): " m_choice

case $m_choice in
    2) MODE_VAL="Release" ;;
    *) MODE_VAL="Debug" ;; # Default catch-all
esac

# ------------------------------------------------------------------------------
# 3. CONFIGURACIÓN DE RUTAS
# ------------------------------------------------------------------------------
BUILD_DIR="build/${ARCH_TAG}-${MODE_VAL}"

echo -e "\n${BLUE}Configuring build in: ${GREEN}${BUILD_DIR}${NC}"

# Limpieza del directorio específico
if [ -d "$BUILD_DIR" ]; then rm -rf "$BUILD_DIR"; fi
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# ------------------------------------------------------------------------------
# 4. CMAKE Y COMPILACIÓN
# ------------------------------------------------------------------------------
echo -e "${YELLOW}Running CMake...${NC}"
cmake ../.. -DCMAKE_ARCH_TYPE="$ARCH_VAL" -DCMAKE_BUILD_TYPE="$MODE_VAL"

if [ $? -ne 0 ]; then
    echo -e "\n${RED}CMake Configuration Failed.${NC}"
    cd ../..
    read -p "Press Enter to exit."
    exit 1
fi

echo -e "\n${YELLOW}Compiling binary...${NC}"
CPU_CORES=$(nproc 2>/dev/null || echo 1)
cmake --build . -j"$CPU_CORES"

# ------------------------------------------------------------------------------
# 5. RESULTADO FINAL
# ------------------------------------------------------------------------------
if [ $? -eq 0 ]; then
    echo -e "\n${GREEN}====================================================${NC}"
    echo -e "${GREEN}   BUILD SUCCESSFUL!                                ${NC}"
    echo -e "${GREEN}   Binary location: ${BUILD_DIR}/Sample_Project       ${NC}"
    echo -e "${GREEN}====================================================${NC}"
else
    echo -e "\n${RED}Build Failed.${NC}"
fi

cd ../..
read -p "Press Enter to exit."