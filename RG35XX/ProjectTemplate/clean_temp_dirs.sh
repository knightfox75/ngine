#!/bin/bash

# NightFox - Clean Script v2 (Lean & Modern)
# Solo actúa sobre carpetas del nuevo pipeline de CMake

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
echo -e "\033[1;34mCleaning active build artifacts in: $BASE_DIR\033[0m"

# 1. Eliminar carpetas que CMake recreará
for dir in build temp build_rg35xx_docker export; do
    if [ -d "$BASE_DIR/$dir" ]; then
        echo "   [REMOVING] $dir..."
        rm -rf "$BASE_DIR/$dir"
    fi
done

# 2. Eliminar binarios finales de la raíz
echo "   [REMOVING] Root binaries..."
rm -f "$BASE_DIR/Idunns_Grace"
rm -f "$BASE_DIR/Idunns_Grace.exe"

# 3. Preparar directorio staging
mkdir -p "$BASE_DIR/temp"

echo -e "\033[1;32mProject is clean. Ready for a fresh build.\033[0m"
read -n 1 -s -r -p "Press any key to exit..."