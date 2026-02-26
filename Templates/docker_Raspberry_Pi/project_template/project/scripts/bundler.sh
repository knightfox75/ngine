#!/bin/bash

# ==============================================================================
# N'GINE SDK - Automated Dependency Bundler (Cross-Compile Compatible)
# ==============================================================================

# Input Arguments
BIN_PATH=$1
DEST_LIBS=$2
SEARCH_ROOT=${3:-"/usr/lib"} 

# Toolchain overrides
OBJDUMP_BIN="${NM:-aarch64-linux-gnu-objdump}"
STRIP_BIN="${STRIP:-aarch64-linux-gnu-strip}"

if [[ "$OBJDUMP_BIN" == *"nm"* ]]; then
    OBJDUMP_BIN="aarch64-linux-gnu-objdump"
fi

if [ ! -f "$BIN_PATH" ]; then
    echo " [ERROR] Binary '$BIN_PATH' not found."
    exit 1
fi

# ------------------------------------------------------------------------------
# EXCLUSION REGEX (System libs usually present on RPi OS)
# ------------------------------------------------------------------------------
# Excluding basic glibc stuff, but keeping multimedia deps that might vary inside
EXCLUDE_REGEX="^(libc\.|libstdc\+\+|libgcc_s|libm\.|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX|libxcb|libGL|libglapi|libdbus|libasound|libgbm|libdrm|libwayland|libpulse|libexpat|libz\.)"

declare -A PROCESSED_LIBS

resolve_lib_path() {
    local lib_name=$1
    local local_path="$(dirname "$BIN_PATH")/$lib_name"
    
    # 1. Check local build folder (Project libs)
    if [ -f "$local_path" ]; then
        echo "$local_path"
        return 0
    fi
    
    # 2. Check the explicitly passed Cross-Compiler Root (Custom compiled SDL2/SFML)
    # Typically: /usr/aarch64-linux-gnu/lib
    if [ -f "$SEARCH_ROOT/$lib_name" ]; then
        echo "$SEARCH_ROOT/$lib_name"
        return 0
    fi
    
    # 3. Check Debian Multiarch System Path (APT installed libs: Freetype, Vorbis, etc)
    # This is where 'apt-get install libfreetype6:arm64' puts files.
    if [ -f "/usr/lib/aarch64-linux-gnu/$lib_name" ]; then
        echo "/usr/lib/aarch64-linux-gnu/$lib_name"
        return 0
    fi

    # 4. Fallback: Search recursively in Search Root (if organized in subfolders)
    local found=$(find "$SEARCH_ROOT" -name "$lib_name" -print -quit 2>/dev/null)
    if [ -n "$found" ]; then
        echo "$found"
        return 0
    fi

    return 1
}

copy_library() {
    local lib_name=$1
    
    if [[ "$lib_name" =~ $EXCLUDE_REGEX ]]; then
        return 2
    fi
    
    local src_path=$(resolve_lib_path "$lib_name")
    if [ -n "$src_path" ]; then
        if cp -L "$src_path" "$DEST_LIBS/$lib_name" 2>/dev/null; then
            # Optimization: Strip symbols to save space on RPi
            $STRIP_BIN --strip-unneeded "$DEST_LIBS/$lib_name" 2>/dev/null
            return 0
        fi
    fi
    return 1
}

resolve_recursive() {
    local file=$1
    # Use cross-objdump to read ARM64 headers
    local deps=$($OBJDUMP_BIN -p "$file" 2>/dev/null | awk '/NEEDED/ {print $2}')
    
    for dep in $deps; do
        if [[ -z "${PROCESSED_LIBS[$dep]}" ]]; then
            PROCESSED_LIBS["$dep"]=1
            
            copy_library "$dep"
            local result=$?
            
            if [ $result -eq 0 ]; then
                echo "       -> Bundled: $dep"
                resolve_recursive "$DEST_LIBS/$dep"
            elif [ $result -eq 1 ]; then
                echo "       [WARNING] Library not found: $dep"
            fi
        fi
    done
}

echo " Analyzing dependency tree..."
echo " Binary: $BIN_PATH"
echo " Roots: $SEARCH_ROOT & /usr/lib/aarch64-linux-gnu"
echo "--------------------------------------------------------"

resolve_recursive "$BIN_PATH"

echo "--------------------------------------------------------"
echo " Dependency bundling complete."