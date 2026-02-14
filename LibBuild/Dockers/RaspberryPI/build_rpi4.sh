#!/bin/bash

# ==============================================================================
# N'GINE - UPDATE SDK (Raspberry Pi 400) + EXPORT
# Platform: Linux (Bash)
# ==============================================================================

# Colores para la terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuración de entorno
BUILDER_IMAGE="ngn-rpi4-builder"
DOCKER_FILE="Dockerfile.ngine-rpi4"
EXPORT_DIR="lib_export"
TAR_NAME="ngn_sdk_rpi4.tar"

clear
echo -e "${BLUE}============================================================================${NC}"
echo -e "${BLUE} N'GINE SDK BUILDER (THE FORGE)${NC}"
echo -e "${BLUE} Target: Raspberry Pi 400 (ARM64 / Debian Trixie)${NC}"
echo -e "${BLUE}============================================================================${NC}"
echo

# ------------------------------------------------------------------------------
# PASO 1: CONSTRUCCIÓN DE LA IMAGEN
# ------------------------------------------------------------------------------
echo -e "🐳 [1/3] ${YELLOW}Building/Updating Docker Image...${NC}"
echo -e "   This may take a while if sources have changed."
echo

docker build --platform linux/arm64 -t "$BUILDER_IMAGE" -f "$DOCKER_FILE" .

if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [ERROR] Failed to build the Docker image.${NC}"
    exit 1
fi

# ------------------------------------------------------------------------------
# PASO 2: VERIFICACIÓN DE LA VERSIÓN
# ------------------------------------------------------------------------------
echo -e "\n🔍 [2/3] ${YELLOW}Verifying Installed Version...${NC}"
echo -e "   Installed in Container: "
docker run --rm --platform linux/arm64 "$BUILDER_IMAGE" pkg-config --modversion ngn

if [ $? -ne 0 ]; then
    echo -e "\n${YELLOW}⚠️ [WARNING] Version check failed. Check if SDK installed correctly.${NC}"
fi

# ------------------------------------------------------------------------------
# PASO 3: EXPORTACIÓN OPCIONAL
# ------------------------------------------------------------------------------
echo
read -p ">> Do you want to export the SDK to a .tar file? (y/N): " export_choice

if [[ "$export_choice" =~ ^[Yy]$ ]]; then
    echo -e "\n📦 [3/3] ${YELLOW}Exporting SDK artifacts...${NC}"
    
    # Crear carpeta si no existe
    if [ ! -d "$EXPORT_DIR" ]; then
        echo "   Creating directory $EXPORT_DIR..."
        mkdir -p "$EXPORT_DIR"
    fi

    # Ejecutar contenedor para empaquetado
    # Usamos $(pwd) para obtener la ruta absoluta actual
    docker run --rm \
      --platform linux/arm64 \
      -v "$(pwd)/$EXPORT_DIR:/export" \
      "$BUILDER_IMAGE" \
      bash -c "tar -cvf /export/$TAR_NAME /usr/local/include/NGN /usr/local/lib/libngn* /usr/local/lib/cmake/NGN /usr/local/lib/pkgconfig/ngn.pc"

    if [ $? -eq 0 ]; then
        echo -e "\n${GREEN}✅ Export complete: $EXPORT_DIR/$TAR_NAME${NC}"
    else
        echo -e "\n${RED}❌ [ERROR] Export failed during tar process.${NC}"
    fi
else
    echo -e "\n⏭️  Skipping export."
fi

echo -e "\n${BLUE}============================================================================${NC}"
echo -e "${BLUE} PROCESS FINISHED${NC}"
echo -e "${BLUE}============================================================================${NC}"

# Pausa final similar a la de Windows
read -n 1 -s -r -p "Press any key to exit..."
echo