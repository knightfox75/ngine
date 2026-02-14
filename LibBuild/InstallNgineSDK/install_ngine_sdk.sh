#!/bin/bash

# ==============================================================================
# NGN ENGINE - BARE METAL UNIVERSAL INSTALLER (PC x86_64 & RPI4)
# ==============================================================================

# --- CONFIGURACIÓN DE VERSIONES ---
VER_SDL2="2.30.11"
VER_SDL_TTF="2.24.0"
VER_SFML="2.6.2"

ROOT_DIR=$(pwd)
WORK_DIR="/tmp/ngn_bare_metal_setup"
LOG_FILE="/tmp/ngn_build_process.log"

# Colores
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# --- FUNCIÓN DE EJECUCIÓN CON SPINNER ---
# Uso: execute_step "Mensaje para el usuario" "comando a ejecutar"
function execute_step() {
    local msg="$1"
    local cmd="$2"

    echo -n "  - $msg ... "

    # Ejecutar comando en background y redirigir todo al log
    eval "$cmd" > "$LOG_FILE" 2>&1 &
    local pid=$!
    local delay=0.1
    local spinstr='|/-\'
    
    # Bucle mientras el proceso (PID) siga vivo
    while kill -0 $pid 2> /dev/null; do
        local temp=${spinstr#?}
        printf "[%c]" "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b" # Borrar los 3 caracteres "[x]" para animar
    done

    # Esperar el código de salida real
    wait $pid
    local exit_code=$?

    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}[OK]${NC}"
    else
        echo -e "${RED}[ERROR]${NC}"
        echo -e "\n${RED}>>> PROCESS FAILED! Details from log ($LOG_FILE):${NC}"
        echo "--------------------------------------------------------"
        tail -n 20 "$LOG_FILE"
        echo "--------------------------------------------------------"
        echo -e "${YELLOW}Installation aborted due to errors.${NC}"
        echo -e "${YELLOW}Press Enter to exit...${NC}"
        read
        exit 1
    fi
}

# Detección de Arquitectura
ARCH_SYSTEM=$(uname -m)
if [ "$ARCH_SYSTEM" == "x86_64" ]; then
    TARGET_ARCH="x86"
    MSG_ARCH="PC Desktop (x86_64)"
elif [ "$ARCH_SYSTEM" == "aarch64" ]; then
    TARGET_ARCH="raspberry"
    MSG_ARCH="Raspberry Pi 4/5 (ARM64)"
else
    echo -e "${RED}Unsupported Architecture: $ARCH_SYSTEM${NC}"
    exit 1
fi

clear
echo -e "${BLUE}====================================================${NC}"
echo -e "${BLUE}   NGN ENGINE - FULL SYSTEM PROVISIONING            ${NC}"
echo -e "${BLUE}====================================================${NC}"
echo -e "Target System: ${YELLOW}$MSG_ARCH${NC}"
echo -e "Libraries to install:"
echo -e " - SDL2 v$VER_SDL2"
echo -e " - SDL2_ttf v$VER_SDL_TTF"
echo -e " - SFML v$VER_SFML"
echo -e " - NGN Engine (Latest Local Source)"
echo -e "${BLUE}====================================================${NC}"

echo -e "${RED}WARNING: This will install libraries to /usr/local and may require sudo password.${NC}"
read -p "Start Installation? [y/N] " choice
if [[ ! "$choice" =~ ^[Yy]$ ]]; then exit 0; fi

# Refrescamos sudo al principio para evitar cortes en el spinner
sudo -v

# ==============================================================================
# FASE 1: DEPENDENCIAS DEL SISTEMA (APT)
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 1] Installing System Dependencies...${NC}"

# APT es mejor dejarlo visible porque suele pedir interacciones o mostrar progreso propio
sudo apt-get update
SYS_DEPS=(
    build-essential cmake git wget curl pkg-config
    libopenal-dev libvorbis-dev libogg-dev libflac-dev
    libfreetype6-dev libharfbuzz-dev
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    libgl1-mesa-dev libglu1-mesa-dev libudev-dev
    libsamplerate0-dev libdbus-1-dev libibus-1.0-dev
)
sudo apt-get install -y --no-install-recommends "${SYS_DEPS[@]}"

# ==============================================================================
# FASE 2: PREPARACIÓN DEL ENTORNO
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 2] Preparing Build Environment...${NC}"
rm -rf "$WORK_DIR"
mkdir -p "$WORK_DIR"
cd "$WORK_DIR"

# ==============================================================================
# FASE 3: SDL2
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 3] Building SDL2 v$VER_SDL2...${NC}"

# Sanitize
echo "  - Cleaning old SDL2 system files..."
sudo rm -rf /usr/local/include/SDL2
sudo rm -f /usr/local/lib/libSDL2*
sudo rm -f /usr/local/lib/pkgconfig/sdl2.pc
sudo rm -rf /usr/local/lib/cmake/SDL2 

# Download
execute_step "Downloading Source" "wget -q 'https://github.com/libsdl-org/SDL/releases/download/release-${VER_SDL2}/SDL2-${VER_SDL2}.tar.gz'"
tar -xzf "SDL2-${VER_SDL2}.tar.gz"
cd "SDL2-${VER_SDL2}"

# Build & Install
execute_step "Configuring" "./configure"
execute_step "Compiling (This may take a while)" "make -j$(nproc)"
execute_step "Installing" "sudo make install"

cd ..

# ==============================================================================
# FASE 4: SDL2_ttf
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 4] Building SDL2_ttf v$VER_SDL_TTF...${NC}"

# Sanitize
echo "  - Cleaning old SDL2_ttf system files..."
sudo rm -f /usr/local/include/SDL2/SDL_ttf.h
sudo rm -f /usr/local/lib/libSDL2_ttf*
sudo rm -f /usr/local/lib/pkgconfig/SDL2_ttf.pc

# Download
execute_step "Downloading Source" "wget -q 'https://github.com/libsdl-org/SDL_ttf/releases/download/release-${VER_SDL_TTF}/SDL2_ttf-${VER_SDL_TTF}.tar.gz'"
tar -xzf "SDL2_ttf-${VER_SDL_TTF}.tar.gz"
cd "SDL2_ttf-${VER_SDL_TTF}"

export CPPFLAGS="-I/usr/local/include/SDL2 -I/usr/include/freetype2 -I/usr/include/harfbuzz"

# Build & Install
execute_step "Configuring" "./configure --disable-freetype-builtin --disable-harfbuzz-builtin"
execute_step "Compiling (This may take a while)" "make -j$(nproc)"
execute_step "Installing" "sudo make install"

unset CPPFLAGS
cd ..

# ==============================================================================
# FASE 5: SFML
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 5] Building SFML v$VER_SFML...${NC}"

# Sanitize
echo "  - Cleaning old SFML system files..."
sudo rm -rf /usr/local/include/SFML
sudo rm -f /usr/local/lib/libsfml*
sudo rm -rf /usr/local/lib/cmake/SFML

# Download
execute_step "Downloading Source" "wget -q 'https://github.com/SFML/SFML/archive/refs/tags/${VER_SFML}.tar.gz' -O 'SFML-${VER_SFML}.tar.gz'"
tar -xzf "SFML-${VER_SFML}.tar.gz"
cd "SFML-${VER_SFML}"

mkdir build && cd build

# Build & Install
execute_step "Configuring CMake" "cmake .."
execute_step "Compiling (This may take a while)" "make -j$(nproc)"
execute_step "Installing" "sudo make install"

cd ../..

# ==============================================================================
# FASE 6: N'gine
# ==============================================================================
echo -e "\n${YELLOW}[PHASE 6] Building N'gine ($TARGET_ARCH)...${NC}"

cd "$ROOT_DIR" 
BUILD_DIR="build_$TARGET_ARCH"

echo "  - Cleaning old NGN system files..."
sudo rm -rf /usr/local/include/NGN
sudo rm -f /usr/local/lib/libngn*
sudo rm -rf /usr/local/lib/cmake/NGN
sudo rm -f /usr/local/lib/pkgconfig/ngn.pc

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
rm -rf * 

# Build & Install NGN
execute_step "Configuring CMake" "cmake .. -DARCH=$TARGET_ARCH"
execute_step "Compiling (This may take a while)" "cmake --build . -j$(nproc)"
execute_step "Installing" "sudo cmake --install ."

# Update Cache
execute_step "Updating System Library Cache" "sudo ldconfig"

# ==============================================================================
# FINALIZACIÓN
# ==============================================================================
rm -rf "$WORK_DIR"

echo -e "\n${GREEN}====================================================${NC}"
echo -e "${GREEN}   FULL INSTALLATION COMPLETED SUCCESSFULLY!        ${NC}"
echo -e "${GREEN}====================================================${NC}"
echo -e "System Status:"
echo -n " - SDL2 Version:     "; pkg-config --modversion sdl2
echo -n " - SDL2_ttf Version: "; pkg-config --modversion SDL2_ttf
echo -n " - SFML Version:     "; pkg-config --modversion sfml-all
echo -n " - NGN Version:      "; pkg-config --modversion ngn
echo ""
echo -e "${YELLOW}Press Enter to exit...${NC}"
read