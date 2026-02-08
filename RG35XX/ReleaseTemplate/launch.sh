#!/bin/sh
# ==============================================================================
# N'GINE SDK - RG35XX Launcher Template
# Target: Anbernic RG35XX (Cortex-A53) / Actions Semi / Allwinner
# Strategy: Private Library Vendoring + RPATH + Hardware Direct Access
# ==============================================================================

# [CONFIGURATION] Define your binary name here
BINARY_NAME="Ngine_Project"

# 1. WORK ENVIRONMENT
# Determine the absolute path to avoid relative path errors when launching from menus
progdir=$(dirname "$0")
cd "$progdir"

# 2. LOW-LEVEL PERMISSIONS (First-Principles)
# Force full access to inputs, framebuffer (screen), and vibration motor (uinput)
chmod 666 /dev/input/* /dev/fb0 /dev/uinput 2>/dev/null

# 3. LIBRARY MANAGEMENT
# Although the binary uses RPATH, we reinforce LD_LIBRARY_PATH to ensure 
# our rescued libSDL2-2.0.so.0 (v2.28.5) is loaded before any system library.
export LD_LIBRARY_PATH="$progdir/libs:/usr/lib:/lib:/usr/lib/aarch64-linux-gnu:$LD_LIBRARY_PATH"

# 4. CONTROL CONFIGURATION (RG35XX Standard Mapping)
# Note: If A/B or X/Y are inverted, swap b0/b1 or b2/b3 in this string.
export SDL_GAMECONTROLLERCONFIG="19002cb4010000000100000000010000,ANBERNIC-keys,a:b1,b:b0,x:b2,y:b3,back:b6,start:b7,guide:b8,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,lefttrigger:b9,righttrigger:b10,leftstick:b10,rightstick:b11,platform:Linux,"

# 5. LAUNCH & LOGS
# Overwrite the log on each start to monitor performance and haptics
echo "--- Starting ${BINARY_NAME} (N'GINE SDK) ---" > ./exec_log.txt
echo "Timestamp: $(date)" >> ./exec_log.txt
echo "Library Path: $progdir/libs" >> ./exec_log.txt

# The binary will automatically load ./libs/libSDL2-2.0.so.0 via RPATH
./"${BINARY_NAME}" >> ./exec_log.txt 2>&1

# 6. SAFE SHUTDOWN
# Ensure savegames and logs are physically written to the SD card before exiting
sync

# Clean exit to the console menu
exit 0