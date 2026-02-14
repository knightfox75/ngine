#!/bin/bash

# ==============================================================================
# NGN ENGINE - SOURCE INSTALLER (UPDATE ONLY)
# ==============================================================================

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Detección Auto de Arquitectura
ARCH_SYSTEM=$(uname -m)
if [ "$ARCH_SYSTEM" == "x86_64" ]; then
    TARGET_ARCH="x86"
    MSG="PC (x86_64)"
elif [ "$ARCH_SYSTEM" == "aarch64" ]; then
    TARGET_ARCH="raspberry"
    MSG="RPI (ARM64)"
else
    echo -e "${RED}Unknown architecture: $ARCH_SYSTEM${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read
    exit 1
fi

clear
echo -e "${BLUE}====================================================${NC}"
echo -e "${BLUE}   NGN ENGINE - SOURCE UPDATE ($MSG)                ${NC}"
echo -e "${BLUE}====================================================${NC}"

# --- FUNCIÓN DE LIMPIEZA SEGURA ---
function clean_previous_install() {
    echo -e "\n${YELLOW}[MAINTENANCE] Cleaning old NGN versions from /usr/local...${NC}"
    sudo rm -rf /usr/local/include/NGN
    sudo rm -f /usr/local/lib/libngn*
    sudo rm -rf /usr/local/lib/cmake/NGN
    sudo rm -f /usr/local/lib/pkgconfig/ngn.pc
}

# Preparación del entorno de compilación
BUILD_DIR="build_$TARGET_ARCH"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1
# Limpieza rápida de cache
rm -rf * 

# Configuración CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake .. -DARCH=$TARGET_ARCH

if [ $? -ne 0 ]; then
    echo -e "${RED}Configuration failed.${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read
    exit 1
fi

# Compilación
echo -e "\n${YELLOW}Compiling...${NC}"
cmake --build . -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed.${NC}"
    echo -e "${YELLOW}Press Enter to exit...${NC}"
    read
    exit 1
fi

# Instalación
echo -e "\n${GREEN}Ready to install. Password required.${NC}"
clean_previous_install
sudo cmake --install .

# Actualizamos la cache de librerías del sistema inmediatamente
echo "Updating system library cache..."
sudo ldconfig
# ==========================

if [ $? -eq 0 ]; then
    echo -e "\n${GREEN}====================================================${NC}"
    echo -e "${GREEN}   SUCCESS! NGN Engine Updated.                     ${NC}"
    echo -e "${GREEN}====================================================${NC}"
    pkg-config --modversion ngn
else
    echo -e "${RED}Installation failed.${NC}"
fi

# Volver al directorio original
cd ..

echo -e "\n${YELLOW}Press Enter to exit...${NC}"
read