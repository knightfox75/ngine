#!/bin/bash
# ==============================================================================
# N'GINE SDK - Internal RPi4 Build Pipeline
# ==============================================================================
# Execution Environment: Docker (ngn-rpi4-builder_production)
# ==============================================================================

set -e

# ARGS: 1=Name, 2=Version
PROJECT_NAME=${1:-"Default_Game"}
PROJECT_VERSION=${2:-"1.0.0"}

ROOT_DIR="/workspace"
BUILD_DIR="$ROOT_DIR/build"
EXPORT_DIR="$ROOT_DIR/export"
# lib path for RPi
LIBS_TARGET="$EXPORT_DIR/libs/aarch64-linux-gnu"

# TOOLCHAIN DEFINITIONS (From Docker Env)
STRIP_BIN="${STRIP:-aarch64-linux-gnu-strip}"

echo "--------------------------------------------------------"
echo " INITIALIZING RPI4 RELEASE BUILD"
echo " Project: ${PROJECT_NAME}"
echo " Version: ${PROJECT_VERSION}"
echo "--------------------------------------------------------"

# 1. CLEANUP
echo "[1/5] Preparing workspace..."
rm -rf "$BUILD_DIR" "$EXPORT_DIR"
mkdir -p "$BUILD_DIR" "$LIBS_TARGET"

# 2. COMPILATION
echo "[2/5] Configuring CMake (Cross-Compile)..."
cd "$BUILD_DIR"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBINARY_NAME="${PROJECT_NAME}" \
    -DPROJECT_VERSION="${PROJECT_VERSION}"

echo "Compiling..."
make -j$(nproc)

# 3. EXPORT & STRIP
echo "[3/5] Deploying and Stripping binary..."
cp "${PROJECT_NAME}" "$EXPORT_DIR/"

echo "      Using: $STRIP_BIN"
$STRIP_BIN --strip-all "$EXPORT_DIR/${PROJECT_NAME}"

# 4. RPATH VERIFICATION
echo "[4/5] Verifying RPATH integrity..."
# We use the cross-compiler readelf, or standard if compatible (readelf usually handles ELF regardless)
RPATH_CHECK=$(readelf -d "$EXPORT_DIR/${PROJECT_NAME}" | grep -E "RPATH|RUNPATH" || echo "MISSING")

if [[ "$RPATH_CHECK" == *"\$ORIGIN"* ]]; then
    echo "      STATUS: RPATH Check Passed ($RPATH_CHECK)."
else
    echo "CRITICAL ERROR: RPATH broken. Binary will not find libs on RPi."
    exit 1
fi

# 5. DEPENDENCY BUNDLING
echo "[5/5] Executing Dependency Bundler..."
# We pass the CROSS_LIB_PATH to the bundler
CROSS_LIB_PATH="/usr/aarch64-linux-gnu/lib"
bash "$ROOT_DIR/scripts/bundler.sh" "$EXPORT_DIR/${PROJECT_NAME}" "$LIBS_TARGET" "$CROSS_LIB_PATH"

echo "--------------------------------------------------------"
echo " SUCCESS: RPi4 Build Pipeline Completed."
echo "--------------------------------------------------------"