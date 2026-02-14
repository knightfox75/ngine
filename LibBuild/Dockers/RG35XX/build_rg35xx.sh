#!/bin/bash

# ==============================================================================
# N'GINE - RG35XX ARM64 Builder (Linux/macOS version)
# ==============================================================================

# Header
echo "-------------------------------------------------------"
echo "  N'GINE - RG35XX ARM64 Builder"
echo "-------------------------------------------------------"

# 🐳 [1/2] Building/Verifying Forge image
echo "🐳 [1/2] Building/Verifying Forge image..."
docker build -t ngn-rg35xx-builder -f Dockerfile.rg35xx .

if [ $? -ne 0 ]; then
    echo "❌ [ERROR] Failed to build Docker image."
    exit 1
fi

echo ""
# 🚀 [2/2] Compiling N'GINE for RG35XX (ARM64)
echo "🚀 [2/2] Compiling N'GINE for RG35XX (ARM64)..."
echo "Current directory: $(pwd)"

# Calculate available processor cores for parallel make
CORES=$(nproc 2>/dev/null || echo 4)

# Execute container:
# -v maps current path to /workspace
docker run --rm -v "$(pwd):/workspace" ngn-rg35xx-builder bash -c "
    mkdir -p build_rg35xx && \
    cd build_rg35xx && \
    cmake .. -DCMAKE_SYSTEM_NAME=Linux \
             -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
             -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
             -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
             -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu && \
    make -j$CORES"

if [ $? -ne 0 ]; then
    echo ""
    echo "❌ [ERROR] Compilation failed."
    exit 1
fi

echo ""
echo "✅ Process completed. N'GINE SDK compiled and installed correctly."

# Optional pause for interactive terminals
read -p "Press [Enter] to continue..."