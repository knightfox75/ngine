#!/bin/bash
set -e 

# [INPUT] Project Name, Version
PROJECT_NAME=${1:-"ngine_game"}
PROJECT_VERSION=${2:-"0.0.0.1"}

# Define absolute paths
ROOT_DIR="/workspace"
BUILD_DIR="$ROOT_DIR/build"
EXPORT_DIR="$ROOT_DIR/export"
LIBS_TARGET="$EXPORT_DIR/libs"

echo "🔧 [Docker] Initializing Hybrid Build for: ${PROJECT_NAME} (v${PROJECT_VERSION})"
echo "   Target: Cortex-A53"

# 1. Cleanup
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$EXPORT_DIR"
mkdir -p "$LIBS_TARGET"

cd "$BUILD_DIR"

# 2. CMake Configuration
cmake .. \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
    -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DBINARY_NAME="${PROJECT_NAME}" \
    -DBINARY_VERSION="${PROJECT_VERSION}"

echo "🔨 [Docker] Compiling..."
make -j$(nproc)

# 3. Verification and Export
if [ -f "${PROJECT_NAME}" ]; then
    echo "✂️  [Docker] Stripping symbols..."
    aarch64-linux-gnu-strip --strip-unneeded "${PROJECT_NAME}"
    
    echo "📦 [Docker] Exporting binary..."
    cp "${PROJECT_NAME}" "$EXPORT_DIR/"
    
    # --- RPATH CHECK ---
    RPATH_CHECK=$(aarch64-linux-gnu-readelf -d "$EXPORT_DIR/${PROJECT_NAME}" | grep -E "RPATH|RUNPATH" || echo "MISSING")
    if [[ "$RPATH_CHECK" == *"\$ORIGIN"* ]]; then
        echo "   ✅ RPATH OK: $RPATH_CHECK"
    else
        echo "   ⚠️ WARNING: RPATH missing \$ORIGIN. Libraries might fail to load."
    fi

    # --- BUNDLER EXECUTION ---
    echo "📦 [Docker] Running Dependency Bundler..."
    chmod +x "$ROOT_DIR/scripts/bundler.sh"
    "$ROOT_DIR/scripts/bundler.sh" "$EXPORT_DIR/${PROJECT_NAME}" "$LIBS_TARGET"

    SIZE=$(du -h "$EXPORT_DIR/${PROJECT_NAME}" | cut -f1)
    echo "✅ [Docker] SUCCESS. Binary '${PROJECT_NAME}' ready ($SIZE)."
    echo "   IMPORTANT: The bundler skipped libSDL2 by design."
    echo "   -> Ensure your PATCHED 'libSDL2.so' is placed in: export/libs/"
else
    echo "❌ [Docker] Error: Binary not created."
    exit 1
fi