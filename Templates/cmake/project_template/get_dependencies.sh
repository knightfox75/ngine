#!/bin/bash

# ==============================================================================
# NightFox SDK - UNIVERSAL Dependency Bundler (PC x86_64 & Pi AArch64)
# ==============================================================================

clear

# --- CONFIGURACIÓN ---
BIN_NAME="Sample_Project"
BIN_DIR="$(dirname "$0")"
BIN="$BIN_DIR/$BIN_NAME"

# DETECCIÓN AUTOMÁTICA DE ARQUITECTURA (La clave de la portabilidad)
ARCH_NAME=$(uname -m)
ARCH="${ARCH_NAME}-linux-gnu"
LIBS_DIR="$BIN_DIR/libs/$ARCH/"

# RUTAS DE BÚSQUEDA UNIVERSALES
LIB_PATHS=(
    "/usr/local/lib"
    "/usr/lib/$ARCH"
    "/lib/$ARCH"
    "/usr/lib"
    "/lib"
)

# FILTRO DE SEGURIDAD (No tocar: previene conflictos de Kernel/GLIBC)
EXCLUDE_REGEX="^(libc\.so|libstdc\+\+|libgcc_s|libm\.so|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX11|libxcb|libXau|libXdmcp)"

# Colores para Total Commander / Terminal
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
LIGHT_BLUE=$(tput setaf 6)
RED=$(tput setaf 1)
GREY=$(tput setaf 8)
RESET=$(tput sgr0)

mkdir -p "$LIBS_DIR"
declare -A PROCESSED_LIBS
declare -A RESOLVED_DEPS

# --- FUNCIONES ---

clear_libs_dir() {
  echo "?? Cleaning $LIBS_DIR..."
  rm -rf "$LIBS_DIR"/*
}

find_dependencies() {
  objdump -p "$1" 2>/dev/null | awk '/NEEDED/ {print $2}'
}

copy_library() {
  local lib=$1
  
  # Filtro: ¿Es una librería del sistema base?
  if [[ "$lib" =~ $EXCLUDE_REGEX ]]; then
      echo "   ${GREY}[System Lib - Skipped] $lib${RESET}"
      PROCESSED_LIBS["$lib"]=1
      return 2
  fi

  # Búsqueda en rutas conocidas
  for path in "${LIB_PATHS[@]}"; do
    if [ -e "$path/$lib" ]; then
      # cp -L es vital: convierte enlaces simbólicos en archivos reales
      cp -L "$path/$lib" "$LIBS_DIR/$lib"
      return 0
    fi
  done
  
  return 1
}

resolve_dependencies_recursive() {
  local file=$1
  local deps=$(find_dependencies "$file")
  
  for dep in $deps; do
    if [[ -z "${PROCESSED_LIBS[$dep]}" ]]; then
      PROCESSED_LIBS["$dep"]=1
      
      copy_library "$dep"
      local result=$?
      
      if [ $result -eq 0 ]; then
        echo "?? Bundled ($ARCH_NAME): $GREEN$dep$RESET"
        RESOLVED_DEPS["$dep"]=1
        resolve_dependencies_recursive "$LIBS_DIR/$dep"
      elif [ $result -eq 1 ]; then
        echo "   ${RED}[NOT FOUND]${RESET} $dep"
      fi
    fi
  done
}

# --- EJECUCIÓN ---

clear_libs_dir

if [ ! -f "$BIN" ]; then
  echo "${RED}Error: Binario $BIN no encontrado.${RESET}"
  exit 1
fi

echo "?? Analyzing $LIGHT_BLUE$BIN_NAME$RESET on $YELLOW$ARCH_NAME$RESET..."
resolve_dependencies_recursive "$BIN"

echo -e "\n------------------------------------------------"
echo -e "Total bundled for $ARCH_NAME: $LIGHT_BLUE${#RESOLVED_DEPS[@]}$RESET"
echo -e "Location: $YELLOW$LIBS_DIR$RESET"
echo -e "------------------------------------------------"

echo "Press any key to exit..."
read -n 1 -s
echo