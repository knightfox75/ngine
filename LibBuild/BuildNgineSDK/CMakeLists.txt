# ==============================================================================
# NGN LIBRARY - WINDOWS MASTER FORGE (MinGW x86_64) V2.0 (VS Code Edition)
# ==============================================================================
# Architecture:   x86-64-v2 (Matches Game Binary for LTO Harmony)
# Platform:       Windows (MinGW-w64 via MSYS2)
# Standards:      C++20 (Mandatory), C11
# Compatibility:  Legacy Code::Blocks Naming & Flat Include Structure
# Optimization:   LTO Enabled (Static), Deterministic IEEE-754 Math
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# ------------------------------------------------------------------------------
# 1. TOOLCHAIN & COMPILER CONFIGURATION
# ------------------------------------------------------------------------------
# We rely on the system PATH for compilers (MSYS2 MinGW-w64).
# Explicitly setting these ensures CMake doesn't pick up a wrong concurrent compiler.
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER   "gcc")
set(CMAKE_RC_COMPILER  "windres")

# LTO Specialized Wrappers
# We use gcc-ar and gcc-ranlib instead of the standard binutils to correctly 
# handle GIMPLE bytecode indexing in static libraries (.a) when LTO is active.
# These must be available in the system PATH (Standard in MinGW64).
set(CMAKE_AR           "gcc-ar")
set(CMAKE_RANLIB       "gcc-ranlib")

# ------------------------------------------------------------------------------
# 2. PROJECT INITIALIZATION
# ------------------------------------------------------------------------------
# We enable CXX (C++), C, and RC (Windows Resources) languages.
project(ngn_library LANGUAGES CXX C RC VERSION 1.22.0.7)

message(STATUS "--------------------------------------------------------")
message(STATUS " N'GINE BUILD SYSTEM - Windows MinGW Forge")
message(STATUS " Standards: C++20 / C11")
message(STATUS " Target:    x86-64-v2 (Optimized for Modern PCs)")
message(STATUS "--------------------------------------------------------")

# ------------------------------------------------------------------------------
# 3. EXTERNAL DEPENDENCIES (Middleware)
# ------------------------------------------------------------------------------
# Fetch the SDK path from the Windows Environment Variable
set(WIN_LIBS_PATH "$ENV{NGINE_SDK_PATH}")

# Fail-fast safeguard
if("${WIN_LIBS_PATH}" STREQUAL "")
    message(FATAL_ERROR "Environment variable NGINE_SDK_PATH is not set. Please define it in your OS settings.")
endif()

# Normalize Windows backslashes (\) to CMake forward slashes (/)
file(TO_CMAKE_PATH "${WIN_LIBS_PATH}" WIN_LIBS_PATH)

set(EXTRA_INCLUDES 
    "${WIN_LIBS_PATH}/sdl2/include" 
    "${WIN_LIBS_PATH}/sfml/include"
    "${WIN_LIBS_PATH}/theora/include"
    "${WIN_LIBS_PATH}/vorbis/include"
    "${WIN_LIBS_PATH}/ogg/include"
)
set(EXTRA_LIB_PATHS 
    "${WIN_LIBS_PATH}/sdl2/lib" 
    "${WIN_LIBS_PATH}/sfml/lib"
    "${WIN_LIBS_PATH}/theora/lib"
    "${WIN_LIBS_PATH}/vorbis/lib"
    "${WIN_LIBS_PATH}/ogg/lib"
)

# Prioritize .dll.a (Import Libraries) and then fallback to .a (Static Libraries)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".a")

find_library(SDL2_LIB       NAMES SDL2                 PATHS ${EXTRA_LIB_PATHS} REQUIRED)
find_library(SDL2_TTF_LIB   NAMES SDL2_ttf             PATHS ${EXTRA_LIB_PATHS} REQUIRED)
find_library(SFML_SYSTEM_LIB NAMES sfml-system         PATHS ${EXTRA_LIB_PATHS} REQUIRED)
find_library(SFML_AUDIO_LIB NAMES sfml-audio           PATHS ${EXTRA_LIB_PATHS} REQUIRED)

# Video decoding dependencies
find_library(THEORAPLAY_LIB NAMES theoraplay           PATHS ${EXTRA_LIB_PATHS}) # Optional
find_library(THEORADEC_LIB  NAMES theoradec theora     PATHS ${EXTRA_LIB_PATHS} REQUIRED)
find_library(VORBIS_LIB     NAMES vorbis               PATHS ${EXTRA_LIB_PATHS} REQUIRED)
find_library(OGG_LIB        NAMES ogg                  PATHS ${EXTRA_LIB_PATHS} REQUIRED)

# Base list of strictly required libraries
set(ALL_LIBS 
    ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB}
    ${THEORADEC_LIB} ${VORBIS_LIB} ${OGG_LIB}
)

# Safely append Theoraplay if it exists in the SDK
if(THEORAPLAY_LIB)
    list(APPEND ALL_LIBS ${THEORAPLAY_LIB})
endif()

# ------------------------------------------------------------------------------
# 4. WINDOWS SYSTEM LIBRARIES
# ------------------------------------------------------------------------------
# These are essential Windows API libraries. If omitted, the DLL will fail to 
# load (silent crash) because it won't resolve symbols for timers, windows, 
# or system services.
set(WIN_SYS_LIBS 
    kernel32 user32 gdi32 winmm imm32 ole32 oleaut32 
    version uuid comdlg32 shlwapi shell32 advapi32 setupapi
)

# ------------------------------------------------------------------------------
# 5. EXPORT DIRECTORY & ASSET DEPLOYMENT
# ------------------------------------------------------------------------------
set(EXPORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/export")

# Define where binaries go immediately after compilation
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${EXPORT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${EXPORT_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXPORT_DIR}/bin")

# Flat Header Deployment: Headers are copied to the root of /include to 
# maintain backward compatibility with legacy Code::Blocks projects.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/source/" 
     DESTINATION "${EXPORT_DIR}/include" 
     FILES_MATCHING PATTERN "*.h")

# ------------------------------------------------------------------------------
# 6. SOURCE DISCOVERY
# ------------------------------------------------------------------------------
# PRAGMATIC FIX: Added "source/*.c" to capture standard C middleware like theoraplay.c
file(GLOB_RECURSE BASE_SOURCES "source/*.cpp" "source/*.c" "source/*.h")

# Suppress benign warnings for 3rd-party C middleware to keep build logs clean
set_source_files_properties("source/theoraplay/theoraplay.c" PROPERTIES COMPILE_FLAGS "-Wno-cast-function-type -Wno-unused-parameter")

# ------------------------------------------------------------------------------
# 7. VARIANT FACTORY (Target Generator)
# ------------------------------------------------------------------------------
# This function handles the creation of Static, Shared, Debug, and Release targets.
function(build_ngn_variant target_name output_name is_shared is_debug)
    
    set(VARIANT_SOURCES ${BASE_SOURCES})

    # Resource Injection: Attach .rc files only to Shared (DLL) targets.
    if(is_shared)
        if(is_debug)
            set(RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resource/resource_d.rc")
        else()
            set(RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.rc")
        endif()
        if(EXISTS ${RC_FILE})
            list(APPEND VARIANT_SOURCES ${RC_FILE})
        endif()
    endif()

    # Target Creation
    if(is_shared)
        add_library(${target_name} SHARED ${VARIANT_SOURCES})
    else()
        add_library(${target_name} STATIC ${VARIANT_SOURCES})
    endif()

    # Target Properties & Naming:
    # We force Code::Blocks legacy naming conventions.
    if(is_shared)
        set_target_properties(${target_name} PROPERTIES 
            OUTPUT_NAME ${output_name} 
            PREFIX "" 
            IMPORT_PREFIX "lib" 
            IMPORT_SUFFIX ".a"
            WINDOWS_EXPORT_ALL_SYMBOLS ON # Guarantees all API symbols are exported even without __declspec
        )
    else()
        set_target_properties(${target_name} PROPERTIES 
            OUTPUT_NAME ${output_name} 
            PREFIX "lib"
        )
    endif()

    # Standards Enforcement
    set_target_properties(${target_name} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_compile_features(${target_name} PRIVATE cxx_std_20)

    # Header Includes Interface
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:${EXPORT_DIR}/include>
        ${EXTRA_INCLUDES} 
    )

    # Global Definitions
    target_compile_definitions(${target_name} PRIVATE OS_WINDOWS CMAKE_WINDOWS TARGET_X86)

    # Architecture Flags
    set(ARCH_FLAGS "-m64" "-march=x86-64-v2" "-mtune=generic")

    if(is_debug)
        target_compile_definitions(${target_name} PRIVATE MODE_DEBUG)
        target_compile_options(${target_name} PRIVATE -g -Og -Wall -Wextra ${ARCH_FLAGS})
    else()
        target_compile_definitions(${target_name} PRIVATE MODE_RELEASE)
        
        if(is_shared)
            # Pragmatic Robustness Fix:
            # MinGW + LTO + DLL often causes silent load crashes because the LTO linker
            # violently strips the CRT initialization (_DllMainCRTStartup).
            # We explicitly disable LTO for the DLL to protect the entry points.
            target_compile_options(${target_name} PRIVATE 
                -O3 
                -fno-math-errno 
                -fno-trapping-math 
                -fomit-frame-pointer 
                -Wno-stringop-overflow 
                ${ARCH_FLAGS}
            )
            
            # Link static runtimes to prevent silent crashes due to missing standard DLLs on target PC
            target_link_options(${target_name} PRIVATE 
                ${ARCH_FLAGS}
                -static-libgcc 
                -static-libstdc++
            )
        else()
            # Static libraries can safely use full LTO and fat objects. 
            # The optimization is deferred to the final executable link stage.
            target_compile_options(${target_name} PRIVATE 
                -O3 
                -flto=auto 
                -ffat-lto-objects 
                -fno-math-errno 
                -fno-trapping-math 
                -fomit-frame-pointer 
                -Wno-stringop-overflow 
                ${ARCH_FLAGS}
            )
            target_link_options(${target_name} PRIVATE -flto=auto ${ARCH_FLAGS})
        endif()
    endif()

    # Library Linkage: Combine Middleware and the Windows System Stack.
    target_link_libraries(${target_name} PUBLIC mingw32 ${ALL_LIBS} ${WIN_SYS_LIBS})

endfunction()

# ------------------------------------------------------------------------------
# 8. BUILD VARIANT GENERATION
# ------------------------------------------------------------------------------
# 1. Static Release -> export/lib/libngn_s.a
build_ngn_variant(ngn_s    "ngn_s"   FALSE FALSE) 

# 2. Static Debug   -> export/lib/libngn_d_s.a
build_ngn_variant(ngn_d_s  "ngn_d_s" FALSE TRUE)  

# 3. Shared Release -> export/bin/ngn.dll  & export/lib/libngn.a
build_ngn_variant(ngn      "ngn"     TRUE  FALSE) 

# 4. Shared Debug   -> export/bin/ngn_d.dll & export/lib/libngn_d.a
build_ngn_variant(ngn_d    "ngn_d"   TRUE  TRUE)  

message(STATUS "--------------------------------------------------------")
message(STATUS " N'GINE FORGE:  Build Configuration Successful")
message(STATUS " Binary Style:  Code::Blocks Legacy (Import lib as .a)")
message(STATUS " Includes:      Flat structure deployed to /export/include")
message(STATUS "--------------------------------------------------------")