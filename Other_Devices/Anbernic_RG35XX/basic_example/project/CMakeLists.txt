# ==============================================================================
# N'GINE SDK - RG35XX Build Script
# Strategy: Build-time Compatibility + Runtime RPATH Vendoring
# ==============================================================================
cmake_minimum_required(VERSION 3.10)

if(DEFINED CMAKE_ARCH_TYPE)
    message(STATUS "Architecture Type passed by builder: ${CMAKE_ARCH_TYPE}")
endif()

# [TEMPLATE CONFIGURATION]
if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "RG35XX_ngine_demo")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION 0.1.0)

message(STATUS "Configuring build for binary: ${BINARY_NAME}")

# Global Definitions
add_definitions(-DMODE_RELEASE -DOS_LINUX -DTARGET_RG35XX -DSFML_STATIC)

# --- COMPILER FLAGS ---
# Optimization flags for Cortex-A53 (Anbernic RG35XX)
set(CPU_FLAGS "-march=armv8-a" "-mtune=cortex-a53" "-mcpu=cortex-a53")
set(OPT_FLAGS "-O3" "-flto=auto" "-ffast-math" "-fomit-frame-pointer" "-s")
add_compile_options(${CPU_FLAGS} ${OPT_FLAGS} -Wno-stringop-overflow)

# [CRITICAL] RPATH CONFIGURATION
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN/libs'")
add_link_options(${OPT_FLAGS})

# --- PATHS ---
set(SDK_ROOT "/usr/aarch64-linux-gnu")
set(SYS_ROOT_LIB "/usr/lib/aarch64-linux-gnu")
set(CMAKE_PREFIX_PATH ${SDK_ROOT})

include_directories(
    source
    "${SDK_ROOT}/include"
    "${SDK_ROOT}/include/NGN"
    "${SDK_ROOT}/include/SDL2"
    "${SDK_ROOT}/include/SFML"
)

# ==============================================================================
# 1. STATIC LIBRARY SEARCH (.a)
# ==============================================================================
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(LIB_NGN      NAMES ngn			  REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SDL2_TTF NAMES SDL2_ttf      REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SFML_SYS NAMES sfml-system-s REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SFML_AUD NAMES sfml-audio-s  REQUIRED PATHS ${SDK_ROOT}/lib)

# Static dependencies via PkgConfig
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig")
pkg_check_modules(STAT_VORBIS REQUIRED vorbis vorbisfile vorbisenc)
pkg_check_modules(STAT_OGG    REQUIRED ogg)
pkg_check_modules(STAT_FLAC   REQUIRED flac)
pkg_check_modules(STAT_OPENAL REQUIRED openal)
pkg_check_modules(STAT_FREETYPE REQUIRED freetype2)

# ==============================================================================
# 2. DYNAMIC LIBRARY SEARCH (.so)
# ==============================================================================
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".so.1" ".so.2" ".so.0")

# [GLIBC ERROR SOLUTION]
find_library(LIB_SDL2_BUILDTIME     
    NAMES SDL2 SDL2-2.0
    REQUIRED 
    PATHS ${SDK_ROOT}/lib 
    NO_DEFAULT_PATH
)

# Basic system libraries (Console Runtime)
find_library(SYS_DRM      NAMES drm           REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(SYS_GBM      NAMES gbm           REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(SYS_ALSA     NAMES asound        REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(SYS_UDEV     NAMES udev          REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(LIB_M        NAMES m             REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(LIB_DL       NAMES dl            REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(LIB_PTHREAD  NAMES pthread       REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(LIB_RT       NAMES rt            REQUIRED PATHS ${SYS_ROOT_LIB})
find_library(LIB_ATOMIC   NAMES atomic        REQUIRED PATHS ${SYS_ROOT_LIB})

# ==============================================================================
# 3. CONSTRUCTION AND LINKING
# ==============================================================================
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")
add_executable(${BINARY_NAME} ${SOURCES})
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)

target_link_libraries(${BINARY_NAME} PRIVATE
    ${LIB_NGN}
    ${LIB_SFML_AUD}
    ${LIB_SFML_SYS}
    ${LIB_SDL2_TTF}
    ${LIB_SDL2_BUILDTIME}
    ${STAT_FREETYPE_LIBRARIES}
    ${STAT_OPENAL_LIBRARIES}
    ${STAT_VORBIS_LIBRARIES}
    ${STAT_OGG_LIBRARIES}
    ${STAT_FLAC_LIBRARIES}
    ${SYS_ALSA} ${SYS_DRM} ${SYS_GBM} ${SYS_UDEV}
    ${LIB_PTHREAD} ${LIB_DL} ${LIB_RT} ${LIB_M} ${LIB_ATOMIC}
)

install(TARGETS ${BINARY_NAME} RUNTIME DESTINATION bin)