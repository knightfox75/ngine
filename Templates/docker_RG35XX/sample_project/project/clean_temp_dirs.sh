#!/bin/bash

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
echo -e "\033[1;34mCleaning active build artifacts in: $BASE_DIR\033[0m"

for dir in build export; do
    if [ -d "$BASE_DIR/$dir" ]; then
        echo "   [REMOVING] $dir..."
        rm -rf "$BASE_DIR/$dir"
    fi
done

echo -e "\033[1;32mProject is clean. Ready for a fresh build.\033[0m"
read -n 1 -s -r -p "Press any key to exit..."