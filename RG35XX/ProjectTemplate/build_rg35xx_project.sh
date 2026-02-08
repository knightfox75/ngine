#!/bin/bash

# ==============================================================================
# N'GINE SDK - RG35XX Project Builder (Linux/macOS Template)
# ==============================================================================

# [CONFIGURATION] Define your project name here
PROJECT_NAME="Ngine_Project"
DOCKER_IMAGE="ngn-arm64-builder"

# Header
echo "===================================================="
echo "     $PROJECT_NAME - RG35XX Cross-Compiler"
echo "===================================================="

echo ""
echo "🐳 [1/2] Verifying Docker image..."

# Check if the image exists
if ! docker image inspect "$DOCKER_IMAGE" > /dev/null 2>&1; then
    echo "❌ [ERROR] The image '$DOCKER_IMAGE' does not exist."
    echo "Please run the SDK Forge/Builder script first."
    exit 1
fi

echo ""
echo "🚀 [2/2] Launching compilation container..."
echo "Project Directory: $(pwd)"

# Run the container
# We pass $PROJECT_NAME as the first argument to build_internal.sh
docker run --rm -v "$(pwd):/workspace" "$DOCKER_IMAGE" bash -c "chmod +x scripts/build_internal.sh && ./scripts/build_internal.sh $PROJECT_NAME"

# Check exit status
if [ $? -ne 0 ]; then
    echo ""
    echo "❌ [ERROR] Compilation in Docker failed."
    exit 1
fi

echo ""
echo "✅ Compilation Finished."
echo "The binary '$PROJECT_NAME' is waiting in the 'export/RG35XX' folder."
echo "Copy it to your console alongside the 'libs' folder."
echo ""