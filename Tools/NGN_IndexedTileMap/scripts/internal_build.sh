#!/bin/bash
# ==============================================================================
# N'GINE SDK - Internal Multi-Arch Builder (V3.4)
# ==============================================================================
# Fix: Support for '_Raspbian' suffix in file optimization and verification.
# ==============================================================================

set -e

# ARGS
TOOL_NAME=${1:-"Utility_Tool"}
ARCH_TYPE=${2:-"Linux_x86_64"}

ROOT_DIR="/workspace"
BUILD_DIR="$ROOT_DIR/build_${ARCH_TYPE,,}"
EXPORT_DIR="$ROOT_DIR/exports"

echo "--------------------------------------------------------"
echo " N'GINE DOCKER PIPELINE"
echo " Tool: $TOOL_NAME | Arch: $ARCH_TYPE"
echo "--------------------------------------------------------"

# 1. CLEAN & PREPARE
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# 2. CONFIGURE & COMPILE
cd "$BUILD_DIR"
cmake "$ROOT_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_ARCH_TYPE="$ARCH_TYPE" \
    -DTOOL_NAME="$TOOL_NAME"

make -j$(nproc)

# 3. ARCHITECTURE DETECTION & SUFFIX LOGIC
if [[ "$ARCH_TYPE" == "Raspberry" ]]; then
    STRIP_TOOL="aarch64-linux-gnu-strip"
    PLATFORM="raspberry"
    NAME_SUFFIX="_Raspbian"
else
    STRIP_TOOL="strip"
    PLATFORM="linux"
    NAME_SUFFIX=""
fi

FINAL_EXE="$EXPORT_DIR/$PLATFORM/$TOOL_NAME$NAME_SUFFIX"

# 4. OPTIMIZATION
echo "[INFO] Optimizing binary: $FINAL_EXE"
if command -v $STRIP_TOOL >/dev/null 2>&1; then
    $STRIP_TOOL --strip-all "$FINAL_EXE"
else
    echo "[WARN] $STRIP_TOOL not found. Skipping extra strip."
fi

# 5. VERIFICATION
echo "[INFO] Verifying binary integrity..."
LDD_CHECK=$(ldd "$FINAL_EXE" 2>&1 || true)
if [[ "$LDD_CHECK" == *"not a dynamic executable"* ]]; then
    echo "      STATUS: Static Check Passed."
else
    echo "      WARNING: Dependency detected."
fi

echo "--------------------------------------------------------"
echo " BUILD SUCCESSFUL: $FINAL_EXE"
echo "--------------------------------------------------------"