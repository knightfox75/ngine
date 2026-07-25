#!/usr/bin/env bash

# ==============================================================================
# Clean Script - Linux/UNIX Version
# ==============================================================================

# Resolve the absolute path where the script is located
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

echo "Cleaning active build artifacts in: $BASE_DIR"
echo "---------------------------------------"

# Change to the base directory to safely use wildcards
cd "$BASE_DIR" || exit 1

# 1. Remove all directories starting with "build"
# The trailing slash ensures it only matches directories.
for d in build*/; do
    # Check if directory actually exists (prevents literal "build*/" fallback)
    if [ -d "$d" ]; then
        # Strip trailing slash for cleaner output
        DIR_NAME="${d%/}"
        echo "   [REMOVING] $DIR_NAME..."
        rm -rf "$d"
    fi
done

# 2. Remove specific artifact directories
for d in temp exports; do
    if [ -d "$d" ]; then
        echo "   [REMOVING] $d..."
        rm -rf "$d"
    fi
done

echo "---------------------------------------"
echo "Project is clean."

# Equivalent to 'pause' in Windows
read -n 1 -s -r -p "Press any key to continue..."
echo ""
exit 0