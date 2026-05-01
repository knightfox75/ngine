#!/bin/bash

# ==============================================================================
# N'GINE SDK - Automated Dependency Bundler
# ==============================================================================
# Description: Recursively analyzes ELF binaries for Shared Object (.so) 
#              dependencies, filters out core system libraries, and bundles 
#              required middleware for portable distribution.
# ==============================================================================

# Input Arguments
BIN_PATH=$1
DEST_LIBS=$2

# Validate input binary existence
if [ ! -f "$BIN_PATH" ]; then
    echo " [ERROR] Target binary '$BIN_PATH' not found."
    exit 1
fi

# ------------------------------------------------------------------------------
# EXCLUSION REGEX: Core System Libraries
# ------------------------------------------------------------------------------
# We exclude libraries that are part of the base Linux/POSIX environment to 
# avoid GLIBC version conflicts on the target host (e.g., SteamOS).
# These should always be provided by the host OS.
# ------------------------------------------------------------------------------
EXCLUDE_REGEX="^(libc\.|libstdc\+\+|libgcc_s|libm\.|libpthread|libdl|librt|ld-linux|libresolv|libnsl|libutil|libcrypt|libX|libxcb|libGL|libglapi|libdbus|libasound|libgbm|libdrm|libwayland|libpulse|libexpat|libz\.|libuuid|libsndio|libfontconfig|libbsd|libmd)"

# Associative array to prevent redundant processing and infinite recursion
declare -A PROCESSED_LIBS

# Locates the physical filesystem path of a requested library
resolve_lib_path() {
    local lib_name=$1
    local local_path="$(dirname "$BIN_PATH")/$lib_name"
    
    # Priority 1: Check if the library is already present in the build folder
    if [ -f "$local_path" ]; then
        echo "$local_path"
        return 0
    fi
    
    # Priority 2: Check for Steamworks API in the project source root
    # Since we run inside the Docker container, we know the source is mapped to /workspace
    if [ -f "/workspace/steamworks/$lib_name" ]; then
        echo "/workspace/steamworks/$lib_name"
        return 0
    fi

    # Priority 3: Check NGN custom installation path (Docker-specific)
    if [ -f "/usr/local/lib/$lib_name" ]; then
        echo "/usr/local/lib/$lib_name"
        return 0
    fi
    
    # Priority 4: Query the system dynamic linker cache (ldconfig)
    local sys_path=$(ldconfig -p | grep -F "$lib_name" | head -n 1 | awk -F ' => ' '{print $2}')
    if [ -n "$sys_path" ] && [ -f "$sys_path" ]; then
        echo "$sys_path"
        return 0
    fi
    
    return 1
}

# Copies, dereferences, and optimizes (strips) the library for production
copy_library() {
    local lib_name=$1
    
    # Filter out core OS libraries based on the defined regex
    if [[ "$lib_name" =~ $EXCLUDE_REGEX ]]; then
        return 2
    fi
    
    local src_path=$(resolve_lib_path "$lib_name")
    if [ -n "$src_path" ]; then
        # -L ensures we follow symlinks to copy the actual binary, not a link
        if cp -L "$src_path" "$DEST_LIBS/$lib_name" 2>/dev/null; then
            # Strip unneeded symbols to reduce payload size
            # Note: strip is generally safe for steam_api, but implies trust in the source lib
            strip --strip-unneeded "$DEST_LIBS/$lib_name" 2>/dev/null
            return 0
        fi
    fi
    return 1
}

# Recursively parses ELF 'NEEDED' headers to build the dependency tree
resolve_recursive() {
    local file=$1
    # Use objdump to read the dynamic section and extract library requirements
    local deps=$(objdump -p "$file" 2>/dev/null | awk '/NEEDED/ {print $2}')
    
    for dep in $deps; do
        # Only process if we haven't encountered this library in the current run
        if [[ -z "${PROCESSED_LIBS[$dep]}" ]]; then
            PROCESSED_LIBS["$dep"]=1
            
            copy_library "$dep"
            local result=$?
            
            if [ $result -eq 0 ]; then
                # Successfully bundled: Proceed to scan its own sub-dependencies
                echo "       -> Bundled: $dep"
                resolve_recursive "$DEST_LIBS/$dep"
            elif [ $result -eq 1 ]; then
                # Library was not in exclusion list but could not be found
                echo "       [WARNING] Library not found: $dep"
            fi
        fi
    done
}

echo " Analyzing dependency tree for: $(basename "$BIN_PATH")"
echo " Destination: $DEST_LIBS"
echo "--------------------------------------------------------"

# Kickstart recursion from the main executable
resolve_recursive "$BIN_PATH"

echo "--------------------------------------------------------"
echo " Dependency bundling complete."