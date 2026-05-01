# ==============================================================================
# N'GINE PROJECT - Master CMakeLists
# ==============================================================================
# Target:       x86_64 Linux / SteamDeck (High Performance Release)
# Description:  Main project build configuration. Handles NGN SDK linking 
#               and RPATH configuration for portable distribution.
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# 1. PROJECT INITIALIZATION
# Allows the build system to override binary names and versions via CLI
if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "Sample_Project")
endif()

if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION "1.0.0")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION ${PROJECT_VERSION})

message(STATUS "--------------------------------------------------------")
message(STATUS " N'GINE BUILD SYSTEM - Project Configuration")
message(STATUS " Target: ${BINARY_NAME} v${PROJECT_VERSION}")
message(STATUS "--------------------------------------------------------")

# 2. PATHS & DEFINITIONS
set(TARGET_DEF "TARGET_PC_LINUX")
set(CMAKE_BASE_PATH "/usr/local")

# Header search paths: includes source, NGN SDK, and system multimedia libs
set(EXTRA_INCLUDES 
    source 
    ${CMAKE_BASE_PATH}/include 
    ${CMAKE_BASE_PATH}/include/NGN 
    ${CMAKE_BASE_PATH}/include/SDL2 
    /usr/include
)

set(EXTRA_LIB_PATHS 
    ${CMAKE_BASE_PATH}/lib 
    /usr/lib/x86_64-linux-gnu
)

# 3. DYNAMIC DEPENDENCY RESOLUTION
# Prioritizing Shared Objects (.so) for multimedia middleware
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
find_library(SDL2_LIB       NAMES SDL2          PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB   NAMES SDL2_ttf      PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYS_LIB   NAMES sfml-system   PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUD_LIB   NAMES sfml-audio    PATHS ${EXTRA_LIB_PATHS})

# Xiph Multimedia Libraries (Required to satisfy strict DSO linking for NGN and SFML)
find_library(THEORADEC_LIB  NAMES theoradec     PATHS ${EXTRA_LIB_PATHS})
find_library(THEORA_LIB     NAMES theora        PATHS ${EXTRA_LIB_PATHS})
find_library(VORBISFILE_LIB NAMES vorbisfile    PATHS ${EXTRA_LIB_PATHS})
find_library(VORBIS_LIB     NAMES vorbis        PATHS ${EXTRA_LIB_PATHS})
find_library(OGG_LIB        NAMES ogg           PATHS ${EXTRA_LIB_PATHS})

# 4. STATIC SDK RESOLUTION
# We link NGN statically to ensure core engine logic is optimized with the game
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(NGN_LIB NAMES ngn_s PATHS ${EXTRA_LIB_PATHS})

# FIX: Manual validation of dependencies for CMake 3.14 compatibility
# This replaces the 'REQUIRED' keyword which is only supported in find_library for CMake 3.18+
# Added Xiph libraries to ensure the Docker container has them installed.
foreach(LIB_VAR NGN_LIB SDL2_LIB SDL2_TTF_LIB SFML_SYS_LIB SFML_AUD_LIB THEORADEC_LIB VORBIS_LIB OGG_LIB)
    if(NOT ${LIB_VAR})
        message(FATAL_ERROR "CRITICAL: Dependency ${LIB_VAR} NOT FOUND. Check your environment paths.")
    endif()
endforeach()

message(STATUS " Resolving dependencies...")
message(STATUS "  - NGN Core: ${NGN_LIB}")
message(STATUS "  - SDL2:     ${SDL2_LIB}")
message(STATUS "  - SFML:     ${SFML_SYS_LIB}")
message(STATUS "  - Vorbis:   ${VORBIS_LIB}")

# 5. SOURCE & EXECUTABLE
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

add_executable(${BINARY_NAME} ${SOURCES})

# Target requirements: C++20 is mandatory for the NGN architecture
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)

target_include_directories(${BINARY_NAME} PRIVATE ${EXTRA_INCLUDES})

target_compile_definitions(${BINARY_NAME} PRIVATE 
    OS_LINUX 
    TARGET_PC_LINUX 
    MODE_RELEASE
)

# 6. PERFORMANCE TUNING
# -O3 + LTO: Maximize execution speed and cross-module optimization.
# -ffat-lto-objects: Critical for correctly linking against LTO-optimized static NGN.
# -ffunction-sections: Allows linker to strip unused code.
target_compile_options(${BINARY_NAME} PRIVATE 
    -m64 
    -march=x86-64-v2
    -mtune=generic 
    -O3 
    -fno-math-errno 
    -fno-trapping-math 
    -flto=auto 
    -fomit-frame-pointer 
    -ffunction-sections 
    -fdata-sections 
    -Wno-stringop-overflow
)

# 7. LINKER CONFIGURATION
# --gc-sections: Removes unreachable code to minimize binary footprint.
# --disable-new-dtags: Forces RPATH over RUNPATH for reliable $ORIGIN resolution.
target_link_options(${BINARY_NAME} PRIVATE 
    -flto=auto -Wl,--gc-sections -Wno-stringop-overflow -Wl,--disable-new-dtags
)

# Portability: Set RPATH to look for libraries in the relative './libs/...' folder.
# This ensures the game is standalone and SteamDeck compatible.
set_target_properties(${BINARY_NAME} PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE 
    INSTALL_RPATH "\$ORIGIN/libs/x86_64-linux-gnu"
)

# Linkage order is critical: NGN Engine first, then its video dependencies, then Audio/Middleware.
target_link_libraries(${BINARY_NAME} PRIVATE 
    ${NGN_LIB}
    ${THEORADEC_LIB}
    ${THEORA_LIB}
    ${VORBISFILE_LIB}
    ${VORBIS_LIB}
    ${OGG_LIB}
    ${SDL2_LIB} 
    ${SDL2_TTF_LIB} 
    ${SFML_SYS_LIB} 
    ${SFML_AUD_LIB}
)

message(STATUS " Build configuration complete.")
message(STATUS "--------------------------------------------------------")