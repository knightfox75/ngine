#!/bin/bash
# ==============================================================================
# N'GINE SDK - Internal Production Build Pipeline
# ==============================================================================
# Execution Environment: Linux Container (ngn-linux-pc-builder)
# Responsibility: Orchestrates CMake configuration, compilation, binary 
#                 optimization, and post-build verification.
# ==============================================================================

# Exit immediately if a command exits with a non-zero status
set -e

# ARGS & PATH DEFINITIONS
# Extracts Project metadata with fallbacks for standalone execution
PROJECT_NAME=${1:-"Default_Game"}
PROJECT_VERSION=${2:-"1.0.0"}

ROOT_DIR="/workspace"
BUILD_DIR="$ROOT_DIR/build"
EXPORT_DIR="$ROOT_DIR/export"
LIBS_TARGET="$EXPORT_DIR/libs/x86_64-linux-gnu"

echo "--------------------------------------------------------"
echo " INITIALIZING RELEASE BUILD"
echo " Project: ${PROJECT_NAME}"
echo " Version: ${PROJECT_VERSION}"
echo "--------------------------------------------------------"

# 1. CLEANUP PREVIOUS ARTIFACTS
# Ensures a clean-room build by purging previous build and export trees
echo "[1/5] Preparing workspace environment..."
rm -rf "$BUILD_DIR" "$EXPORT_DIR"
mkdir -p "$BUILD_DIR" "$LIBS_TARGET"

# 2. COMPILATION PHASE
# Triggers CMake to generate the build system and starts the compilation
echo "[2/5] Configuring build system and compiling source..."
cd "$BUILD_DIR"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBINARY_NAME="${PROJECT_NAME}" \
    -DPROJECT_VERSION="${PROJECT_VERSION}"

# Multi-threaded compilation utilizing all available logical CPU cores
make -j$(nproc)

# 3. EXPORT & BINARY OPTIMIZATION
# Finalizing the binary for production by removing debug symbols
echo "[3/5] Stripping debug symbols and deploying binary..."
cp "${PROJECT_NAME}" "$EXPORT_DIR/"

# Stripping all symbols to reduce binary size and protect source logic
strip --strip-all "$EXPORT_DIR/${PROJECT_NAME}"

# 4. RPATH INTEGRITY VERIFICATION
# Crucial for SteamDeck/Linux portability. The binary must use $ORIGIN
# to locate its dependencies in a relative path.
echo "[4/5] Verifying RPATH/RUNPATH integrity for portability..."
RPATH_CHECK=$(readelf -d "$EXPORT_DIR/${PROJECT_NAME}" | grep -E "RPATH|RUNPATH" || echo "MISSING")

# Validation check for the $ORIGIN token in the ELF header
if [[ "$RPATH_CHECK" == *"\$ORIGIN"* ]]; then
    echo "      STATUS: RPATH Check Passed."
else
    echo ""
    echo "CRITICAL ERROR: RPATH is broken or missing!"
    echo "The binary will fail to locate its bundled libraries on the target OS."
    echo "Current Header Value: $RPATH_CHECK"
    exit 1
fi

# 5. DEPENDENCY BUNDLING
# External execution of the library collector script
echo "[5/5] Executing automated dependency bundling..."
bash "$ROOT_DIR/scripts/bundler.sh" "$EXPORT_DIR/${PROJECT_NAME}" "$LIBS_TARGET"

echo "--------------------------------------------------------"
echo " SUCCESS: Build pipeline completed for ${PROJECT_NAME}."
echo " Final artifacts located in: /export"
echo "--------------------------------------------------------"