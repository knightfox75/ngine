#!/bin/sh
# ==============================================================================
# N'GINE SDK - RG35XX Ultimate Launcher (Rescue Edition)
# Target: Anbernic RG35XX (Cortex-A53)
# Strategy: Private Library Vendoring + Auto-Rescue + Hardware Access
# ==============================================================================

# [CONFIGURATION]
# 1. Define your binary name here
BINARY_NAME="Ngine_Project"

# 2. VENDORING MODE (Auto-Rescue)
# Set to "true" to copy the optimized SDL2 from the console firmware to your SD card.
# Set to "false" for normal operation (recommended for final release if lib is included).
ENABLE_LIB_RESCUE="true"

# ------------------------------------------------------------------------------

# 1. WORK ENVIRONMENT
# Determine the absolute path
progdir=$(dirname "$0")
cd "$progdir"

# 2. LOW-LEVEL PERMISSIONS
# Force full access to inputs, framebuffer, and haptics
chmod 666 /dev/input/* /dev/fb0 /dev/uinput 2>/dev/null

# 3. LIBRARY RESCUE LOGIC (The "Vendoring" Strategy)
# This block ensures the optimized library exists locally to satisfy RPATH
if [ "$ENABLE_LIB_RESCUE" = "true" ]; then
    
    # Target file paths
    # This specific version (2.28.5) is known to have RenderGeometry + Haptics support on RG35XX
    SDL_SYSTEM_TARGET="/usr/lib/aarch64-linux-gnu/libSDL2-2.0.so.0.2800.5"
    SDL_LOCAL_DIR="$progdir/libs"
    SDL_LOCAL_FILE="$SDL_LOCAL_DIR/libSDL2-2.0.so.0"

    # Create local libs directory if it doesn't exist
    if [ ! -d "$SDL_LOCAL_DIR" ]; then
        mkdir -p "$SDL_LOCAL_DIR"
    fi

    # Check if we need to rescue the file
    if [ ! -f "$SDL_LOCAL_FILE" ]; then
        echo "--- [RESCUE MODE] Local SDL2 missing. Attempting rescue... ---" >> ./exec_log.txt
        
        if [ -f "$SDL_SYSTEM_TARGET" ]; then
            cp "$SDL_SYSTEM_TARGET" "$SDL_LOCAL_FILE"
            chmod +x "$SDL_LOCAL_FILE"
            echo "SUCCESS: Rescued $SDL_SYSTEM_TARGET to local libs." >> ./exec_log.txt
        else
            echo "WARNING: System target library not found. Game might fail if not compiled statically." >> ./exec_log.txt
        fi
    fi
fi

# 4. ENVIRONMENT SETUP
# We reinforce LD_LIBRARY_PATH to prioritize our local ./libs folder
export LD_LIBRARY_PATH="$progdir/libs:/usr/lib:/lib:/usr/lib/aarch64-linux-gnu:$LD_LIBRARY_PATH"

# 5. CONTROL CONFIGURATION (RG35XX Standard Mapping)
# Note: If A/B or X/Y are inverted, swap b0/b1 or b2/b3 in this string.
export SDL_GAMECONTROLLERCONFIG="19002cb4010000000100000000010000,ANBERNIC-keys,a:b1,b:b0,x:b2,y:b3,back:b6,start:b7,guide:b8,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,lefttrigger:b9,righttrigger:b10,leftstick:b10,rightstick:b11,platform:Linux,"

# 6. LAUNCH
echo "--- Starting ${BINARY_NAME} (N'GINE SDK) ---" > ./exec_log.txt
echo "Timestamp: $(date)" >> ./exec_log.txt
echo "Library Path: $progdir/libs" >> ./exec_log.txt

# Launch binary (RPATH will handle the rest)
./"${BINARY_NAME}" >> ./exec_log.txt 2>&1

# 7. SAFE SHUTDOWN
sync
exit 0