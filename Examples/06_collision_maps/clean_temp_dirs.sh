#!/bin/bash

# Clean Script
# Solo actúa sobre carpetas del nuevo pipeline de CMake

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
echo -e "\033[1;34mCleaning active build artifacts in: $BASE_DIR\033[0m"

# Eliminar las carpetas que CMake recreará (y temp si es legacy)
for dir in build temp; do
    if [ -d "$BASE_DIR/$dir" ]; then
        echo "   [REMOVING] $dir..."
        rm -rf "$BASE_DIR/$dir"
    fi
done

echo -e "\033[1;32mProject is clean.\033[0m"
read -n 1 -s -r -p "Press any key to exit..."
echo