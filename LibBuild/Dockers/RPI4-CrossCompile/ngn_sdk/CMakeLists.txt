# ==============================================================================
# NGN Library - Raspberry Pi 4 Master Release (Cross-Compile Optimized)
# Target: Cortex-A72 (ARMv8-A)
# Features: Dual Build (Static & Shared) for Hybrid Linking
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

project(ngn_library LANGUAGES CXX C VERSION 1.22.0.7)
include(GNUInstallDirs)

# --- [1] ARCHITECTURE TUNING (Raspberry Pi 4) ---
# Specific optimization flags for the Broadcom BCM2711 SoC
# Cleaned architecture flags to prevent LTO switch conflicts.
set(RPI_FLAGS "-mcpu=cortex-a72")

# --- [2] GLOBAL OPTIMIZATIONS (Release Mode) ---
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DMODE_RELEASE -DNDEBUG -DOS_LINUX -DTARGET_RASPBERRY)
    
    # Aggressive Optimization Flags: O3, LTO, and Math optimizations
    set(OPT_FLAGS "-O3 -flto=auto -ffat-lto-objects -fno-math-errno -fno-trapping-math -fomit-frame-pointer -funroll-loops")
    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RPI_FLAGS} ${OPT_FLAGS} -Wall")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${RPI_FLAGS} ${OPT_FLAGS} -Wall")
else()
    message(WARNING "Building in NON-RELEASE mode. High-performance optimizations disabled.")
endif()

# --- [3] DEPENDENCIES & PATHS ---
# Restrict search to the target sysroot to prevent host contamination
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SFML COMPONENTS system audio REQUIRED)

# Xiph Multimedia Libraries (Required for Theoraplay decoding)
# Utilizing find_library since standard find_package modules might not exist in cross-sysroot natively
find_library(THEORADEC_LIB NAMES theoradec theora REQUIRED)
find_library(VORBIS_LIB NAMES vorbis REQUIRED)
find_library(OGG_LIB NAMES ogg REQUIRED)

# --- [4] SOURCE FILES ---
# Added "source/*.c" to capture C middleware (theoraplay.c)
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c" "source/*.h")

# Suppress benign warnings for 3rd-party C middleware
set_source_files_properties("source/theoraplay/theoraplay.c" PROPERTIES COMPILE_FLAGS "-Wno-cast-function-type -Wno-unused-parameter")

# --- [5] BUILD TARGETS ---

# Target: Static Library (Backward compatibility for existing projects)
add_library(ngn STATIC ${SOURCES})
set_target_properties(ngn PROPERTIES OUTPUT_NAME "ngn")

# Target: Shared Library (Optimized for dynamic batch builds)
add_library(ngn_shared SHARED ${SOURCES})
set_target_properties(ngn_shared PROPERTIES 
    OUTPUT_NAME "ngn"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE ON
)

# Shared Configuration for both targets
foreach(target ngn ngn_shared)
    target_include_directories(${target} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NGN>
    )
    target_link_libraries(${target} PRIVATE 
        SDL2::SDL2 
        SDL2_ttf::SDL2_ttf 
        sfml-system 
        sfml-audio
        ${THEORADEC_LIB}
        ${VORBIS_LIB}
        ${OGG_LIB}
    )
endforeach()

# --- [6] INSTALLATION ---
# Deploy both .a and .so to the system library directory
install(TARGETS ngn ngn_shared EXPORT NGNTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Deploy Headers
install(DIRECTORY source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NGN 
    FILES_MATCHING PATTERN "*.h"
)

# Export CMake configuration for find_package(NGN)
install(EXPORT NGNTargets FILE NGNTargets.cmake NAMESPACE NGN:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NGN)

message(STATUS "NGN RPi4 Release Forge Configured: Static and Shared targets ready.")