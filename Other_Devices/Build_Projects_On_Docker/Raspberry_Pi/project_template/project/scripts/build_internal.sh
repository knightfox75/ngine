#!/bin/bash
set -e

PROJECT_NAME=${1:-"Idunns_Grace"}
echo "🔧 [Docker] Starting Master Build for: ${PROJECT_NAME}"

# Usamos rutas absolutas dentro del contenedor para evitar errores de directorio
ROOT_DIR="/workspace"
BUILD_DIR="$ROOT_DIR/build_rpi4_docker"
EXPORT_DIR="$ROOT_DIR/game_export"
LIBS_TARGET="$EXPORT_DIR/libs/aarch64-linux-gnu"

# 1. Preparar directorios (Limpieza total)
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$LIBS_TARGET"

# 2. Compilar
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release -DBINARY_NAME="${PROJECT_NAME}"
make -j$(nproc)

# 3. Exportar binario a la carpeta de exportación
cp "${PROJECT_NAME}" "$EXPORT_DIR/"

echo "🧪 Checking RPATH integrity..."
RPATH_CHECK=$(readelf -d "$EXPORT_DIR/${PROJECT_NAME}" | grep -E "RPATH|RUNPATH" || echo "MISSING")
echo "   Result: $RPATH_CHECK"

if [[ "$RPATH_CHECK" == *"\$ORIGIN"* ]]; then
    echo "   ✅ RPATH looks correct (contains \$ORIGIN)."
else
    echo "   ❌ ERROR: RPATH is broken or missing \$ORIGIN!"
fi

# 4. Ejecutar el Bundler Recursivo
# Le pasamos la ruta del binario y la ruta de destino de librerías
chmod +x "$ROOT_DIR/scripts/bundler.sh"
"$ROOT_DIR/scripts/bundler.sh" "$EXPORT_DIR/${PROJECT_NAME}" "$LIBS_TARGET"

echo "✅ [Docker] Deployment package ready in /game_export"