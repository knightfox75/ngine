#!/bin/bash
# ==============================================================================
# NightFox SDK - Recursive Bundler for RG35XX (Cross-Compiler)
# ==============================================================================
# NOTE: Intentionally ignores libSDL2 to use the patched version.
# ==============================================================================

BIN_PATH=$1
DEST_LIBS=$2
OBJDUMP="aarch64-linux-gnu-objdump"

if [ ! -f "$BIN_PATH" ]; then
    echo "❌ Error: Binary $BIN_PATH not found."
    exit 1
fi

# Paths where the Cross-Compiler searches for standard libraries
LIB_PATHS=(
    "/usr/aarch64-linux-gnu/lib"
    "/usr/lib/aarch64-linux-gnu"
    "/usr/local/lib"
)

# ------------------------------------------------------------------------------
# EXCLUSION FILTER
# ------------------------------------------------------------------------------
# 1. libSDL2: Excluded because we use an external patched version.
# 2. libc, libstdc++, etc: Excluded because they are system base (Garlic/Stock).
# 3. libdrm, libgbm, libasound: Usually provided by the firmware.
# ------------------------------------------------------------------------------
EXCLUDE_REGEX="^(libSDL2|libc\.so|libstdc\+\+|libgcc_s|libm\.so|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX11|libxcb|libXau|libXdmcp|libGL|libdbus|libasound|libgbm|libdrm|libudev)"

declare -A PROCESSED_LIBS

copy_library() {
    local lib=$1
    if [[ "$lib" =~ $EXCLUDE_REGEX ]]; then
        return 2 # Explicitly excluded (skip)
    fi

    for path in "${LIB_PATHS[@]}"; do
        if [ -f "$path/$lib" ]; then
            # Copy following symbolic links (-L)
            if cp -L "$path/$lib" "$DEST_LIBS/$lib" 2>/dev/null; then
                return 0
            fi
        fi
    done
    return 1 # Not found
}

resolve_recursive() {
    local file=$1
    # Use ARM64 architecture objdump
    local deps=$($OBJDUMP -p "$file" 2>/dev/null | awk '/NEEDED/ {print $2}')
    
    for dep in $deps; do
        if [[ -z "${PROCESSED_LIBS[$dep]}" ]]; then
            PROCESSED_LIBS["$dep"]=1
            
            copy_library "$dep"
            local result=$?
            
            if [ $result -eq 0 ]; then
                echo "   -> Bundled: $dep"
                resolve_recursive "$DEST_LIBS/$dep"
            elif [ $result -eq 1 ]; then
                echo "   [!] Warning: '$dep' needed but not found in Toolchain."
            elif [ $result -eq 2 ]; then
                # Silent for system libraries, but useful to know in debug
                # echo "   [i] Skipped (System/Manual): $dep"
                :
            fi
        fi
    done
}

echo "🔍 Analyzing dependencies for $(basename "$BIN_PATH")..."
resolve_recursive "$BIN_PATH"