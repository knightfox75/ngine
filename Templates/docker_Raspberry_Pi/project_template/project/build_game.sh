#!/bin/bash

# ==============================================================================
# N'GINE - RPi4 Full Bundler (Linux Version)
# Target: Raspberry Pi 4/400 (ARM64)
# ==============================================================================

# Colores para la terminal
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Configuración
PROJECT_NAME="Ngine_Project"
DOCKER_IMAGE="ngn-rpi4-builder"
INTERNAL_SCRIPT="scripts/build_internal.sh"

# Limpiar pantalla
clear

echo -e "${BLUE}====================================================${NC}"
echo -e "${BLUE}      ${PROJECT_NAME} - RPi4 Full Bundler${NC}"
echo -e "${BLUE}====================================================${NC}"
echo

echo -e "🚀 ${BLUE}Launching Build & Recursive Bundling...${NC}"
echo -e "   Project directory: $(pwd)"
echo

# Ejecutar el contenedor Docker
# -v $(pwd):/workspace mapea el directorio actual
docker run --rm \
  --platform linux/arm64 \
  -v "$(pwd):/workspace" \
  "$DOCKER_IMAGE" \
  bash -c "chmod +x $INTERNAL_SCRIPT && ./$INTERNAL_SCRIPT $PROJECT_NAME"

# Capturar el estado de salida de Docker
if [ $? -ne 0 ]; then
    echo -e "\n${RED}❌ [ERROR] Process failed.${NC}"
    echo -e "Check the logs above for more information."
    echo
    read -p "Press Enter to exit..."
    exit 1
fi

echo -e "\n${GREEN}✅ Done! Check 'game_export/' folder.${NC}"
echo
read -p "Press Enter to exit..."
