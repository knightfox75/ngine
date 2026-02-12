#!/bin/bash
# ==============================================================================
# NightFox SDK - Recursive Bundler for Docker (ARM64)
# ==============================================================================

BIN_PATH=$1
DEST_LIBS=$2

if [ ! -f "$BIN_PATH" ]; then
    echo "❌ Error: Binary $BIN_PATH not found."
    exit 1
fi

# Configuración de arquitectura interna de Docker
ARCH="aarch64-linux-gnu"
LIB_PATHS=(
    "/usr/local/lib"
    "/usr/lib/$ARCH"
    "/lib/$ARCH"
)

# Filtro de seguridad (Librerías base de la Pi que NO se deben mover)
EXCLUDE_REGEX="^(libc\.so|libstdc\+\+|libgcc_s|libm\.so|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX11|libxcb|libXau|libXdmcp|libGL|libdbus|libasound|libgbm|libdrm)"

declare -A PROCESSED_LIBS

copy_library() {
    local lib=$1
    if [[ "$lib" =~ $EXCLUDE_REGEX ]]; then
        return 2
    fi

    for path in "${LIB_PATHS[@]}"; do
        if [ -f "$path/$lib" ]; then
            # cp -L resuelve los enlaces simbólicos para que el archivo sea real
            if cp -L "$path/$lib" "$DEST_LIBS/$lib" 2>/dev/null; then
                return 0
            fi
        fi
    done
    return 1
}

resolve_recursive() {
    local file=$1
    # Analizar dependencias con objdump
    local deps=$(objdump -p "$file" 2>/dev/null | awk '/NEEDED/ {print $2}')
    
    for dep in $deps; do
        if [[ -z "${PROCESSED_LIBS[$dep]}" ]]; then
            PROCESSED_LIBS["$dep"]=1
            
            copy_library "$dep"
            local result=$?
            
            if [ $result -eq 0 ]; then
                echo "   -> Bundled: $dep"
                # Recursión para analizar la propia librería copiada
                resolve_recursive "$DEST_LIBS/$dep"
            elif [ $result -eq 1 ]; then
                echo "   [!] Skipped (Not found in SDK paths): $dep"
            fi
        fi
    done
}

echo "🔍 Analyzing dependencies for $(basename "$BIN_PATH")..."
resolve_recursive "$BIN_PATH"