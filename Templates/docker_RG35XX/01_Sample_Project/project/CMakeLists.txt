# ==============================================================================
# N'GINE PROJECT - RG35XX HYBRID BUILDER
# Target: Cortex-A53 (Anbernic RG35XX)
# ==============================================================================
cmake_minimum_required(VERSION 3.14)

# [CONFIG]
if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "ngine_game")
endif()

if(NOT DEFINED BINARY_VERSION)
    set(BINARY_VERSION "0.0.0.1")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION ${BINARY_VERSION})

# 1. OPTIMIZATION FLAGS
# Defines global macros, including _REENTRANT for thread safety in SDL2/SFML
add_definitions(
    -DMODE_RELEASE 
    -DOS_LINUX 
    -DTARGET_RG35XX 
    -DSFML_STATIC 
    -DNDEBUG
    -D_REENTRANT
)

set(CPU_FLAGS "-march=armv8-a" "-mtune=cortex-a53" "-mcpu=cortex-a53")
set(OPT_FLAGS "-O3" "-flto=auto" "-fno-math-errno" "-fno-trapping-math" "-fomit-frame-pointer")

add_compile_options(${CPU_FLAGS} ${OPT_FLAGS} -Wno-stringop-overflow)

# [RPATH]
set(CMAKE_INSTALL_RPATH "$ORIGIN/libs")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
add_link_options("-Wl,-rpath,$ORIGIN/libs")
add_link_options(${CPU_FLAGS} ${OPT_FLAGS} -s)

# 2. PATHS & INCLUDES
set(SDK_ROOT "/usr/aarch64-linux-gnu")
set(SYS_ROOT_ARM "/usr/lib/aarch64-linux-gnu")

include_directories(
    source
    "${SDK_ROOT}/include"
    "${SDK_ROOT}/include/SDL2"
    "${SDK_ROOT}/include/SFML"
    "${SDK_ROOT}/include/NGN"
)

# ==============================================================================
# 3. STATIC LIBRARIES (.a)
# ==============================================================================
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")

# Core
find_library(LIB_NGN      NAMES ngn ngn_s       REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SDL2_TTF NAMES SDL2_ttf        REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SFML_SYS NAMES sfml-system-s   REQUIRED PATHS ${SDK_ROOT}/lib)
find_library(LIB_SFML_AUD NAMES sfml-audio-s    REQUIRED PATHS ${SDK_ROOT}/lib)

# Dependencies for Freetype
find_library(LIB_PNG      NAMES png png16       REQUIRED PATHS ${SDK_ROOT}/lib ${SYS_ROOT_ARM})
find_library(LIB_Z        NAMES z               REQUIRED PATHS ${SDK_ROOT}/lib ${SYS_ROOT_ARM})

# Searching for BZ2 but it is NOT mandatory
find_library(LIB_BZ2      NAMES bz2 bzip2       PATHS ${SDK_ROOT}/lib ${SYS_ROOT_ARM})
if(NOT LIB_BZ2)
    set(LIB_BZ2 "")
    message(STATUS "Optional libbz2 not found - skipping")
else()
    message(STATUS "Found libbz2: ${LIB_BZ2}")
endif()

# Codecs via PkgConfig (Handles OGG, Vorbis, FLAC, OpenAL, etc.)
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "${SDK_ROOT}/lib/pkgconfig:${SDK_ROOT}/pkgconfig")
pkg_check_modules(CODECS REQUIRED IMPORTED_TARGET 
    freetype2 vorbis vorbisfile vorbisenc ogg flac openal
)

# ==============================================================================
# 4. DYNAMIC LIBRARIES (.so)
# ==============================================================================
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

# Dynamic SDL2 (libs folder on the device)
find_library(LIB_SDL2 NAMES SDL2 REQUIRED PATHS ${SDK_ROOT}/lib)

# Xiph Multimedia Libraries (Theora)
find_library(LIB_THEORADEC NAMES theoradec theora REQUIRED PATHS ${SDK_ROOT}/lib ${SYS_ROOT_ARM})

# System Libs
find_library(SYS_M        m         REQUIRED)
find_library(SYS_DL       dl        REQUIRED)
find_library(SYS_PTHREAD  pthread   REQUIRED)
find_library(SYS_RT       rt        REQUIRED)
find_library(SYS_ALSA     asound    REQUIRED)
find_library(SYS_DRM      drm       REQUIRED)
find_library(SYS_GBM      gbm       REQUIRED)
find_library(SYS_UDEV     udev      REQUIRED)

# 5. BUILD TARGET
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")
add_executable(${BINARY_NAME} ${SOURCES})
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)

# ==============================================================================
# 6. LINKING ORDER
# ==============================================================================
target_link_libraries(${BINARY_NAME} PRIVATE
    "-Wl,--whole-archive" ${LIB_NGN} "-Wl,--no-whole-archive"
    
    ${LIB_THEORADEC}
    
    ${LIB_SFML_AUD}
    ${LIB_SFML_SYS}
    ${LIB_SDL2_TTF}
    
    ${LIB_SDL2} 
    
    PkgConfig::CODECS
    ${LIB_PNG}
    ${LIB_Z}
    ${LIB_BZ2}
    
    ${SYS_ALSA} ${SYS_DRM} ${SYS_GBM} ${SYS_UDEV}
    ${SYS_PTHREAD} ${SYS_DL} ${SYS_RT} ${SYS_M}
)

message(STATUS "Build Configuration: RELEASE | Target: ${BINARY_NAME}")