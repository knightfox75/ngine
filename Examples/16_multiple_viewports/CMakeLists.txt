# ==============================================================================
# Sample Project - Master CMake File (Universal Architecture)
# ==============================================================================
# Targets: Windows (MinGW), Linux (x64), Raspberry Pi (Cortex-A72)
# Profiles: Debug, Release
# Features: Deterministic Math, Robust Linking, Auto-Run Deployment
# Note:     Consumes NGN SDK via Environment Variable (NGINE_SDK_PATH)
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# ------------------------------------------------------------------------------
# 0. GAME CONFIGURATION
# ------------------------------------------------------------------------------
set(GAME_EXECUTABLE_NAME    "Sample_Project")
set(GAME_VERSION            "1.0.0.0")
set(RUN_AFTER_BUILD         TRUE)

# ------------------------------------------------------------------------------
# 1. ARCHITECTURE & BUILD TYPE DEFINITIONS
# ------------------------------------------------------------------------------
set(ARCH_LINUX_X86_64   "Linux_x86_64")
set(ARCH_RASPBERRY      "Raspberry")
set(ARCH_WINDOWS_X86_64 "Windows_x86_64")

if(NOT CMAKE_ARCH_TYPE)
    # Default fallback if no preset is loaded
    set(CMAKE_ARCH_TYPE ${ARCH_LINUX_X86_64})
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# ------------------------------------------------------------------------------
# 2. COMPILER SELECTION
# ------------------------------------------------------------------------------
# We rely on the system PATH for compilers (MSYS2 MinGW-w64 or Native GCC)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER   "gcc")

if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    # We define windres here, but enable the language later
    set(CMAKE_RC_COMPILER "windres")
endif()

# ------------------------------------------------------------------------------
# 3. PROJECT INITIALIZATION
# ------------------------------------------------------------------------------
project(${GAME_EXECUTABLE_NAME} LANGUAGES CXX C VERSION ${GAME_VERSION})
set(BINARY_TARGET_NAME ${GAME_EXECUTABLE_NAME})

# ------------------------------------------------------------------------------
# 4. PATHS & LIBRARY SUFFIXES
# ------------------------------------------------------------------------------
if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    # Explicitly enable RC language only for Windows targets.
    enable_language(RC)

    # Fetch the SDK path from the Windows Environment Variable
    set(SDK_ROOT "$ENV{NGINE_SDK_PATH}")

    # Fail-fast safeguard
    if("${SDK_ROOT}" STREQUAL "")
        message(FATAL_ERROR "Environment variable NGINE_SDK_PATH is not set. Please define it in your OS settings.")
    endif()

    # Normalize Windows backslashes (\) to CMake forward slashes (/)
    file(TO_CMAKE_PATH "${SDK_ROOT}" SDK_ROOT)

    # Map the SDK structure (Assumes standard NGN SDK layout)
    set(EXTRA_INCLUDES 
        "${SDK_ROOT}/ngn/include" 
        "${SDK_ROOT}/sdl2/include" 
        "${SDK_ROOT}/sfml/include"
    )
    set(EXTRA_LIB_PATHS 
        "${SDK_ROOT}/ngn/lib" 
        "${SDK_ROOT}/sdl2/lib" 
        "${SDK_ROOT}/sfml/lib"
    )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a")
else()
    set(CMAKE_BASE_PATH "/usr/local")
    set(EXTRA_INCLUDES 
        ${CMAKE_BASE_PATH}/include 
        ${CMAKE_BASE_PATH}/include/NGN
        ${CMAKE_BASE_PATH}/include/SDL2 
        ${CMAKE_BASE_PATH}/include/SFML
        /usr/include
    )
    set(EXTRA_LIB_PATHS ${CMAKE_BASE_PATH}/lib /usr/local/lib)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
endif()

# ------------------------------------------------------------------------------
# 5. LIBRARY NAMES (DEBUG vs RELEASE)
# ------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_NAME_NGN "ngn_d")
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
        set(LIB_NAME_SFML_SYS "sfml-system-d")
        set(LIB_NAME_SFML_AUD "sfml-audio-d")
    else()
        set(LIB_NAME_SFML_SYS "sfml-system")
        set(LIB_NAME_SFML_AUD "sfml-audio")
    endif()
else()
    set(LIB_NAME_NGN "ngn")
    set(LIB_NAME_SFML_SYS "sfml-system")
    set(LIB_NAME_SFML_AUD "sfml-audio")
endif()

# ------------------------------------------------------------------------------
# 6. LIBRARY DISCOVERY
# ------------------------------------------------------------------------------
find_library(NGN_LIB        NAMES ${LIB_NAME_NGN}      REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_LIB       NAMES SDL2                 REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB   NAMES SDL2_ttf             REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYS_LIB   NAMES ${LIB_NAME_SFML_SYS} REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUD_LIB   NAMES ${LIB_NAME_SFML_AUD} REQUIRED PATHS ${EXTRA_LIB_PATHS})

# ------------------------------------------------------------------------------
# 7. SOURCE FILES & RESOURCES
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    set(RES_FILE "resource/resource.rc")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${RES_FILE}")
        list(APPEND SOURCES ${RES_FILE})
    endif()
endif()

# ------------------------------------------------------------------------------
# 8. TARGET & COMPILATION CONFIGURATION
# ------------------------------------------------------------------------------
add_executable(${BINARY_TARGET_NAME} ${SOURCES})

target_compile_features(${BINARY_TARGET_NAME} PRIVATE cxx_std_20)
target_include_directories(${BINARY_TARGET_NAME} PRIVATE source ${EXTRA_INCLUDES})

target_compile_definitions(${BINARY_TARGET_NAME} PRIVATE
    $<$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_WINDOWS_X86_64}>:OS_WINDOWS CMAKE_WINDOWS TARGET_X86 SDL_MAIN_HANDLED>
    $<$<NOT:$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_WINDOWS_X86_64}>>:OS_LINUX>
    $<$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_RASPBERRY}>:TARGET_RASPBERRY>
    $<$<CONFIG:Debug>:MODE_DEBUG>
    $<$<CONFIG:Release>:MODE_RELEASE>
)

# ------------------------------------------------------------------------------
# 9. OPTIMIZATION FLAGS (THE CONSTITUTION)
# ------------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Global Release Optimizations
    target_compile_options(${BINARY_TARGET_NAME} PRIVATE 
        -O3 -flto=auto -fno-math-errno -fno-trapping-math -fomit-frame-pointer -Wno-stringop-overflow
    )
    target_link_options(${BINARY_TARGET_NAME} PRIVATE -flto=auto -s)
    
    # x86_64 (PC) Specific
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64 OR CMAKE_ARCH_TYPE STREQUAL ARCH_LINUX_X86_64)
        target_compile_options(${BINARY_TARGET_NAME} PRIVATE -march=x86-64-v2 -mtune=generic)
    endif()

    # ARMv8 (Raspberry Pi 4) Specific
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_RASPBERRY)
        target_compile_options(${BINARY_TARGET_NAME} PRIVATE -mcpu=cortex-a72 -mtune=cortex-a72)
    endif()

    # Windows Release: Entry point and Subsystem
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
        target_link_options(${BINARY_TARGET_NAME} PRIVATE -mwindows -Wl,--entry=mainCRTStartup)
    endif()
else()
    # Debug: Standard symbols
    target_compile_options(${BINARY_TARGET_NAME} PRIVATE -g -O0)
endif()

# ------------------------------------------------------------------------------
# 10. LINKING & SYSTEM DEPENDENCIES
# ------------------------------------------------------------------------------
if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    target_link_libraries(${BINARY_TARGET_NAME} PRIVATE 
        mingw32
        ${NGN_LIB} 
        ${SDL2_LIB} 
        ${SDL2_TTF_LIB} 
        ${SFML_AUD_LIB} 
        ${SFML_SYS_LIB}
        setupapi imm32 version winmm gdi32 ole32 oleaut32 uuid comdlg32 shlwapi shell32 user32 advapi32
    )
else()
    target_link_libraries(${BINARY_TARGET_NAME} PRIVATE 
        ${NGN_LIB} ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_AUD_LIB} ${SFML_SYS_LIB}
    )
    
    # RPATH Subdirectory
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_LINUX_X86_64)
        set(LIB_SUBDIR "x86_64-linux-gnu")
    elseif(CMAKE_ARCH_TYPE STREQUAL ARCH_RASPBERRY)
        set(LIB_SUBDIR "aarch64-linux-gnu")
    endif()

    set_target_properties(${BINARY_TARGET_NAME} PROPERTIES 
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN/libs/${LIB_SUBDIR}"
    )
    target_link_options(${BINARY_TARGET_NAME} PRIVATE -Wl,--disable-new-dtags -Wl,--gc-sections)
endif()

# ------------------------------------------------------------------------------
# 11. POST-BUILD: DEPLOYMENT & AUTO-RUN
# ------------------------------------------------------------------------------
if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    set(EXE_EXT ".exe")
else()
    set(EXE_EXT "")
endif()

# Deployment to Project Root
add_custom_command(TARGET ${BINARY_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BINARY_TARGET_NAME}> ${CMAKE_CURRENT_SOURCE_DIR}/${BINARY_TARGET_NAME}${EXE_EXT}
    COMMENT "Deploying binary to root: ${BINARY_TARGET_NAME}${EXE_EXT}"
)

# Auto-Run Logic (Host Architecture Check)
if(RUN_AFTER_BUILD)
    set(CAN_RUN FALSE)
    
    # Windows Host -> Windows Target
    if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64 AND WIN32)
        set(CAN_RUN TRUE)
    
    # Linux x64 Host -> Linux x64 Target
    elseif(CMAKE_ARCH_TYPE STREQUAL ARCH_LINUX_X86_64 AND UNIX AND NOT APPLE)
        execute_process(COMMAND uname -m OUTPUT_VARIABLE HOST_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(HOST_ARCH STREQUAL "x86_64")
            set(CAN_RUN TRUE)
        endif()
        
    # Raspberry Pi Host -> Raspberry Target
    elseif(CMAKE_ARCH_TYPE STREQUAL ARCH_RASPBERRY AND UNIX)
        execute_process(COMMAND uname -m OUTPUT_VARIABLE HOST_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(HOST_ARCH STREQUAL "aarch64")
            set(CAN_RUN TRUE)
        endif()
    endif()

    if(CAN_RUN)
        add_custom_command(TARGET ${BINARY_TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${BINARY_TARGET_NAME}${EXE_EXT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Launching ${BINARY_TARGET_NAME}..."
        )
    endif()
endif()