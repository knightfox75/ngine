#!/bin/bash
set -e # Abort if error

# [INPUT] Get Project Name from argument (default to "Ngine_Project" if empty)
PROJECT_NAME=${1:-"Ngine_Project"}

echo "🔧 [Docker] Starting internal compilation for: ${PROJECT_NAME}..."

# Define paths
BUILD_DIR="build_rg35xx_docker"
EXPORT_DIR="export/RG35XX"

# Cleanup and directory creation
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
mkdir -p $EXPORT_DIR

cd $BUILD_DIR

# CMake Configuration
# We pass -DBINARY_NAME to overwrite the variable in CMakeLists.txt
cmake .. \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_ARCH_TYPE="RG35XX" \
    -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
    -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
    -DBINARY_NAME="${PROJECT_NAME}"

echo "🔨 [Docker] Compiling..."
make -j$(nproc)

# Copy the resulting binary (using the dynamic name)
echo "📦 [Docker] Exporting binary..."

if [ -f "${PROJECT_NAME}" ]; then
    cp "${PROJECT_NAME}" ../$EXPORT_DIR/
    echo "✅ [Docker] Internal process finished. Binary: ${PROJECT_NAME}"
else
    echo "❌ [Docker] Error: Binary '${PROJECT_NAME}' not found after compilation."
    exit 1
fi