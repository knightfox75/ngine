#!/bin/bash

# ==============================================================================
# NightFox SDK - UNIVERSAL Dependency Bundler (PC x86_64 & Pi AArch64)
# ==============================================================================

clear

# --- CONFIGURATION ---
BIN_NAME="Sample_Project"
BIN_DIR="$(dirname "$0")"
BIN="$BIN_DIR/$BIN_NAME"

# AUTOMATIC ARCHITECTURE DETECTION (The key to portability)
ARCH_NAME=$(uname -m)
ARCH="${ARCH_NAME}-linux-gnu"
LIBS_DIR="$BIN_DIR/libs/$ARCH/"

# UNIVERSAL SEARCH PATHS
LIB_PATHS=(
    "/usr/local/lib"
    "/usr/lib/$ARCH"
    "/lib/$ARCH"
    "/usr/lib"
    "/lib"
)

# Colors for Total Commander / Terminal
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
LIGHT_BLUE=$(tput setaf 6)
RED=$(tput setaf 1)
GREY=$(tput setaf 8)
RESET=$(tput sgr0)

# SECURITY FILTER (Platform specific EXCLUDE_REGEX assignment)
case "$ARCH_NAME" in
    x86_64|i?86)
        # Linux (x86/x64)
        EXCLUDE_REGEX="^(libc\.|libstdc\+\+|libgcc_s|libm\.|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX|libxcb|libGL|libglapi|libdbus|libasound|libgbm|libdrm|libwayland|libpulse|libexpat|libz\.|libuuid|libsndio|libfontconfig|libbsd|libmd)"
        ;;
    aarch64|arm*)
        # Raspberry PI (ARM 64-bit / 32-bit)
        EXCLUDE_REGEX="^(libc\.|libstdc\+\+|libgcc_s|libm\.|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX|libxcb|libGL|libglapi|libdbus|libasound|libgbm|libdrm|libwayland|libpulse|libexpat|libz\.|libuuid|libfontconfig|libbsd|libmd|libglib-2\.0|libpcre2-8|libharfbuzz|libgraphite2)"
        ;;
    *)
        echo "${RED}Error: Unsupported architecture $ARCH_NAME${RESET}"
        exit 1
        ;;
esac

mkdir -p "$LIBS_DIR"
declare -A PROCESSED_LIBS
declare -A RESOLVED_DEPS

# --- FUNCTIONS ---

clear_libs_dir() {
  echo "?? Cleaning $LIBS_DIR..."
  rm -rf "$LIBS_DIR"/*
}

find_dependencies() {
  objdump -p "$1" 2>/dev/null | awk '/NEEDED/ {print $2}'
}

copy_library() {
  local lib=$1
  
  # Filter: Is it a base system library?
  if [[ "$lib" =~ $EXCLUDE_REGEX ]]; then
      echo "   ${GREY}[System Lib - Skipped] $lib${RESET}"
      PROCESSED_LIBS["$lib"]=1
      return 2
  fi

  # Search in known paths
  for path in "${LIB_PATHS[@]}"; do
    if [ -e "$path/$lib" ]; then
      # cp -L is vital: converts symlinks to real files
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

# --- EXECUTION ---

clear_libs_dir

if [ ! -f "$BIN" ]; then
  echo "${RED}Error: Binary $BIN not found.${RESET}"
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