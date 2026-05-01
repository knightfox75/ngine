# ==============================================================================
# N'GINE PROJECT - RPi4 Cross-Compile Master CMakeLists
# ==============================================================================
# Target:       Raspberry Pi 4 (ARM64 / Cortex-A72)
# OS:           Linux (Debian/RPi OS)
# Description:  Strict Release Mode. Static NGN linking. Dynamic Deps.
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# 1. PROJECT INITIALIZATION
if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "Sample_Project")
endif()

if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION "1.0.0")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION ${PROJECT_VERSION})

message(STATUS "--------------------------------------------------------")
message(STATUS " N'GINE BUILD SYSTEM - RPi4 Cross-Compiler")
message(STATUS " Target: ${BINARY_NAME} v${PROJECT_VERSION}")
message(STATUS " Architecture: ARM64 (Cortex-A72)")
message(STATUS "--------------------------------------------------------")

# 2. CROSS-COMPILATION DEFINITIONS
set(TARGET_DEF "TARGET_RASPBERRY")
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# The Dockerfile installed custom compiled libs here:
set(CROSS_ROOT "/usr/aarch64-linux-gnu")

# Header search paths
set(EXTRA_INCLUDES 
    source 
    ${CROSS_ROOT}/include 
    ${CROSS_ROOT}/include/NGN 
    ${CROSS_ROOT}/include/SDL2 
)

# Library search paths (Merged Custom Sysroot + Debian Multiarch Sysroot)
set(EXTRA_LIB_PATHS 
    ${CROSS_ROOT}/lib 
    /usr/lib/aarch64-linux-gnu 
)

# 3. DYNAMIC DEPENDENCY RESOLUTION (Shared Objects .so)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

find_library(SDL2_LIB       NAMES SDL2              PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SDL2_TTF_LIB   NAMES SDL2_ttf          PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SFML_SYS_LIB   NAMES sfml-system       PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SFML_AUD_LIB   NAMES sfml-audio        PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)

# Xiph Multimedia Libraries
find_library(THEORADEC_LIB  NAMES theoradec theora  PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(VORBIS_LIB     NAMES vorbis            PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(OGG_LIB        NAMES ogg               PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)

# 4. STATIC SDK RESOLUTION (Force Static .a)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(NGN_LIB NAMES ngn PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)

# Validation: Check critical underlying libraries.
foreach(LIB_VAR NGN_LIB SDL2_LIB SDL2_TTF_LIB SFML_SYS_LIB SFML_AUD_LIB THEORADEC_LIB VORBIS_LIB OGG_LIB)
    if(NOT ${LIB_VAR})
        message(FATAL_ERROR "CRITICAL: Dependency ${LIB_VAR} NOT FOUND. Check your library paths.")
    endif()
endforeach()

message(STATUS " Resolving dependencies...")
message(STATUS "  - NGN Core (Static): ${NGN_LIB}")
message(STATUS "  - SDL2 (Shared):     ${SDL2_LIB}")
message(STATUS "  - SFML (Shared):     ${SFML_SYS_LIB}")
message(STATUS "  - Xiph Video/Audio:  Resolved")

# 5. SOURCE & EXECUTABLE
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")
add_executable(${BINARY_NAME} ${SOURCES})

# Target requirements: C++20
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_include_directories(${BINARY_NAME} PRIVATE ${EXTRA_INCLUDES})

# DEFINES
target_compile_definitions(${BINARY_NAME} PRIVATE 
    OS_LINUX 
    TARGET_RASPBERRY 
    MODE_RELEASE 
    _REENTRANT 
)

# 6. PERFORMANCE TUNING (Cortex-A72 Specific)
# Added -ffat-lto-objects to safely mix C/C++ objects inside static archives during LTO
target_compile_options(${BINARY_NAME} PRIVATE 
    -mcpu=cortex-a72 
    -mtune=cortex-a72 
    -O3 
    -fno-math-errno 
    -fno-trapping-math 
    -flto=auto 
    -ffat-lto-objects
    -fomit-frame-pointer 
    -ffunction-sections 
    -fdata-sections
    -Wno-psabi 
    -Wno-stringop-overflow
)

# 7. LINKER CONFIGURATION
target_link_options(${BINARY_NAME} PRIVATE 
    -mcpu=cortex-a72 
    -O3 
    -flto=auto 
    -Wl,--gc-sections 
    -Wl,--disable-new-dtags
)

# RPATH for Portability on RPi
set_target_properties(${BINARY_NAME} PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE 
    INSTALL_RPATH "\$ORIGIN/libs/aarch64-linux-gnu"
)

# Linkage order resolving the cascade
# By wrapping NGN_LIB in whole-archive, we prevent internal archive symbol 
# dropping (like theoraplay.o) due to LTO or sequential forward-scan misses.
target_link_libraries(${BINARY_NAME} PRIVATE 
    -Wl,--whole-archive 
    ${NGN_LIB} 
    -Wl,--no-whole-archive
    ${THEORADEC_LIB}
    ${VORBIS_LIB}
    ${OGG_LIB}
    ${SDL2_LIB} 
    ${SDL2_TTF_LIB} 
    ${SFML_SYS_LIB} 
    ${SFML_AUD_LIB}
    pthread
)

message(STATUS " Build configuration complete.")
message(STATUS "--------------------------------------------------------")