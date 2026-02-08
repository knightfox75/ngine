# ==============================================================================
# N'GINE Library - Dedicated RG35XX Forge (ARM64 Docker SDK)
# Version: 1.21.0.1 - "Static Monolith Edition"
# TARGET: Anbernic RG35XX (Cortex-A53)
# ==============================================================================

cmake_minimum_required(VERSION 3.10)

string(ASCII 27 Esc)
set(ColorReset  "${Esc}[m")
set(BoldBlue    "${Esc}[1;34m")
set(BoldGreen   "${Esc}[1;32m")

set(CMAKE_BASE_PATH "/usr/aarch64-linux-gnu")

# Optimized flags for target hardware (Cortex-A53)
set(ARCH_FLAGS 
    "-march=armv8-a"
    "-mtune=cortex-a53"
    "-mcpu=cortex-a53"
)

# Global definition to activate code #ifdefs (Platform specific)
set(TARGET_DEF "TARGET_RG35XX")

project(ngn_library LANGUAGES CXX C VERSION 1.21.0.1)
message(STATUS "${BoldBlue}Forging N'GINE SDK for Anbernic RG35XX (ARM64 - Static)${ColorReset}")

# Search paths within Docker Sysroot
set(EXTRA_LIB_PATHS "${CMAKE_BASE_PATH}/lib" "${CMAKE_BASE_PATH}/lib64")
set(EXTRA_INCLUDES 
    "${CMAKE_BASE_PATH}/include" 
    "${CMAKE_BASE_PATH}/include/SDL2" 
    "${CMAKE_BASE_PATH}/include/SFML"
)

# --- Static Search Configuration ---
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")

# Dependency Search
find_library(SDL2_LIB NAMES SDL2 REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB NAMES SDL2_ttf REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYSTEM_LIB NAMES sfml-system sfml-system-s REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUDIO_LIB NAMES sfml-audio sfml-audio-s REQUIRED PATHS ${EXTRA_LIB_PATHS})

# Basic System Dependencies
find_library(LIB_M m)
find_library(LIB_DL dl)
find_library(LIB_PTHREAD pthread)
find_library(LIB_RT rt)

set(SYSTEM_DEPS ${LIB_M} ${LIB_DL} ${LIB_PTHREAD} ${LIB_RT})
set(NGINE_DEPS ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB})

message(STATUS "Static Libs Detected: ${BoldGreen}YES${ColorReset}")

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

# ------------------------------------------------------------------------------
# FACTORY
# ------------------------------------------------------------------------------
function(build_ngn_variant target_name output_name is_shared is_debug)
    
    if(is_shared)
        add_library(${target_name} SHARED ${SOURCES})
    else()
        add_library(${target_name} STATIC ${SOURCES})
    endif()

    set_target_properties(${target_name} PROPERTIES 
        OUTPUT_NAME ${output_name}
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    target_compile_features(${target_name} PRIVATE cxx_std_20)
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:include/NGN>
        ${EXTRA_INCLUDES}
    )

    # [KEY MODIFICATION]: PUBLIC so the flag propagates with the library
    target_compile_definitions(${target_name} PUBLIC OS_LINUX ${TARGET_DEF} SFML_STATIC)

    target_compile_options(${target_name} PRIVATE ${ARCH_FLAGS})

    if(is_debug)
        target_compile_definitions(${target_name} PRIVATE MODE_DEBUG)
        target_compile_options(${target_name} PRIVATE -g -Og -Wall -Wextra)
    else()
        target_compile_definitions(${target_name} PRIVATE MODE_RELEASE)
        target_compile_options(${target_name} PRIVATE -O3 -flto=auto -s -Wno-stringop-overflow)
        target_link_options(${target_name} PRIVATE -flto=auto -s)
    endif()

    target_link_libraries(${target_name} PUBLIC ${NGINE_DEPS} ${SYSTEM_DEPS})

    install(TARGETS ${target_name}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )
endfunction()

# Static variants only for RG35XX
build_ngn_variant(ngn_s    "ngn_s"   FALSE FALSE) # Release
build_ngn_variant(ngn_d_s  "ngn_d_s" FALSE TRUE)  # Debug

# Header Installation
install(DIRECTORY source/ DESTINATION include/NGN FILES_MATCHING PATTERN "*.h")

# Pkg-config generation
set(NGN_INSTALL_PKG_DIR "lib/pkgconfig")
configure_file("cmake/ngn.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" DESTINATION ${NGN_INSTALL_PKG_DIR})

message(STATUS "${BoldGreen}NGN SDK Static Forge (RG35XX) Ready!${ColorReset}")