# ==============================================================================
# NIGHTFOX BUILD SYSTEM - NGN Library Master Configuration
# ==============================================================================
# Target:       x86_64 Linux / SteamDeck (High Performance Release)
# Standards:    C++20, C11
# Artifacts:    Static (.a) and Shared (.so) variants
# ==============================================================================

cmake_minimum_required(VERSION 3.14)
project(ngn_library LANGUAGES CXX C VERSION 1.22.0.7)
include(GNUInstallDirs)

# 1. ENVIRONMENT & PATH CONFIGURATION
set(CMAKE_BASE_PATH "/usr/local")
set(EXTRA_LIB_PATHS ${CMAKE_BASE_PATH}/lib /usr/lib/x86_64-linux-gnu)
set(EXTRA_INCLUDES ${CMAKE_BASE_PATH}/include ${CMAKE_BASE_PATH}/include/SDL2)

set(ARCH_FLAGS "-m64" "-march=x86-64-v2" "-mtune=generic")
set(TARGET_DEF "TARGET_PC_LINUX")

# 2. DEPENDENCY RESOLUTION
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

# Implemented explicit checks to ensure build stability.
find_library(SDL2_LIB SDL2 PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYSTEM_LIB sfml-system PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUDIO_LIB sfml-audio PATHS ${EXTRA_LIB_PATHS})

# Video decoding dependencies
find_library(THEORAPLAY_LIB theoraplay PATHS ${EXTRA_LIB_PATHS}) # Optional (often built from source)
find_library(THEORADEC_LIB NAMES theoradec theora PATHS ${EXTRA_LIB_PATHS})
find_library(VORBIS_LIB vorbis PATHS ${EXTRA_LIB_PATHS})
find_library(OGG_LIB ogg PATHS ${EXTRA_LIB_PATHS})

foreach(LIB_VAR SDL2_LIB SDL2_TTF_LIB SFML_SYSTEM_LIB SFML_AUDIO_LIB THEORADEC_LIB VORBIS_LIB OGG_LIB)
    if(NOT ${LIB_VAR})
        message(FATAL_ERROR "CRITICAL: Dependency ${LIB_VAR} NOT FOUND. Environment setup is incomplete.")
    endif()
endforeach()

set(ALL_LIBS 
    ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB}
    ${THEORADEC_LIB} ${VORBIS_LIB} ${OGG_LIB}
)

# Safely append Theoraplay if it exists as a precompiled library in the Docker env
if(THEORAPLAY_LIB)
    list(APPEND ALL_LIBS ${THEORAPLAY_LIB})
endif()

# 3. SOURCE DISCOVERY
# PRAGMATIC FIX: Added "source/*.c" to capture standard C middleware like theoraplay.c
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c" "source/*.h")

# Suppress benign warnings for 3rd-party C middleware to keep build logs clean
set_source_files_properties("source/theoraplay/theoraplay.c" PROPERTIES COMPILE_FLAGS "-Wno-cast-function-type -Wno-unused-parameter")

# 4. BUILD VARIANT FACTORY
function(build_ngn_variant target_name output_name is_shared)
    if(is_shared)
        add_library(${target_name} SHARED ${SOURCES})
    else()
        add_library(${target_name} STATIC ${SOURCES})
    endif()

    set_target_properties(${target_name} PROPERTIES 
        OUTPUT_NAME ${output_name} 
        VERSION ${PROJECT_VERSION}
    )
    
    target_compile_features(${target_name} PRIVATE cxx_std_20)
    
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source> 
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NGN> 
        ${EXTRA_INCLUDES}
    )
    
    target_compile_definitions(${target_name} PRIVATE OS_LINUX ${TARGET_DEF} MODE_RELEASE)
    
    # Aggressive Optimization Pipeline (LTO + sections)
    if(is_shared)
        target_compile_options(${target_name} PRIVATE -O3 -flto=auto -ffat-lto-objects -fno-math-errno -fno-trapping-math -fomit-frame-pointer ${ARCH_FLAGS})
        target_link_options(${target_name} PRIVATE -flto=auto)
    else()
        target_compile_options(${target_name} PRIVATE -O3 -flto=auto -ffat-lto-objects -fno-math-errno -fno-trapping-math -fomit-frame-pointer -ffunction-sections -fdata-sections ${ARCH_FLAGS})
    endif()
    
    target_link_libraries(${target_name} PUBLIC ${ALL_LIBS})
    
    install(TARGETS ${target_name} 
        EXPORT NGNTargets 
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endfunction()

# Build variants
build_ngn_variant(ngn_s "ngn_s" FALSE)
build_ngn_variant(ngn   "ngn"   TRUE)

# 5. INSTALLATION & PACKAGING
install(DIRECTORY source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NGN FILES_MATCHING PATTERN "*.h")

include(CMakePackageConfigHelpers)
install(EXPORT NGNTargets FILE NGNTargets.cmake NAMESPACE NGN:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NGN)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc"
"prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=\${prefix}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}

Name: NGN SDK
Description: NightFox Engine Production SDK - High-Performance Multimedia Framework
Version: ${PROJECT_VERSION}
Libs: -L\${libdir} -lngn_s
Cflags: -I\${includedir} -I\${includedir}/NGN
")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)