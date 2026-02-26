# ==============================================================================
# NGN Library - Dedicated RG35XX Forge (ARM64 Docker SDK)
# Version: 1.21.0 - "Static Monolith Edition"
# TARGET: Anbernic RG35XX (Cortex-A53)
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# 0. LTO SUPPORT CHECK (Global Optimization)
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)

# 1. COLORS
string(ASCII 27 Esc)
set(ColorReset  "${Esc}[m")
set(BoldBlue    "${Esc}[1;34m")
set(BoldGreen   "${Esc}[1;32m")

# 2. TARGET CONFIGURATION (RG35XX)
set(CMAKE_BASE_PATH "/usr/aarch64-linux-gnu")

# Cleaned architecture flags to prevent LTO switch conflicts.
# -mcpu=cortex-a53 implies both the architecture (armv8-a) and the tuning.
set(ARCH_FLAGS "-mcpu=cortex-a53")
set(TARGET_DEF "TARGET_RG35XX")

project(ngn_library LANGUAGES CXX C VERSION 1.21.0.6)
include(GNUInstallDirs) 

message(STATUS "${BoldBlue}Forging NGN SDK for Anbernic RG35XX (ARM64 - Static)${ColorReset}")

# 3. SEARCH PATHS
set(EXTRA_LIB_PATHS "${CMAKE_BASE_PATH}/lib" "${CMAKE_BASE_PATH}/lib64")
set(EXTRA_INCLUDES 
    "${CMAKE_BASE_PATH}/include" 
    "${CMAKE_BASE_PATH}/include/SDL2" 
    "${CMAKE_BASE_PATH}/include/SFML"
)

# 4. DEPENDENCIES (Static Mode)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")

find_library(SDL2_LIB NAMES SDL2 REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB NAMES SDL2_ttf REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYSTEM_LIB NAMES sfml-system sfml-system-s REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUDIO_LIB NAMES sfml-audio sfml-audio-s REQUIRED PATHS ${EXTRA_LIB_PATHS})

find_library(LIB_M m)
find_library(LIB_DL dl)
find_library(LIB_PTHREAD pthread)
find_library(LIB_RT rt)

set(SYSTEM_DEPS ${LIB_M} ${LIB_DL} ${LIB_PTHREAD} ${LIB_RT})
set(NGINE_DEPS ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB})

message(STATUS "Static Libs Detected: ${BoldGreen}YES${ColorReset}")

# 5. SOURCES
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

# 6. FACTORY
function(build_ngn_variant target_name output_name is_shared is_debug)
    
    add_library(${target_name} STATIC ${SOURCES})

    set_target_properties(${target_name} PROPERTIES 
        OUTPUT_NAME ${output_name}
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    target_compile_features(${target_name} PRIVATE cxx_std_20)
    
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NGN>
        ${EXTRA_INCLUDES}
    )

    # --- CRITICAL FLAGS VERIFICATION ---
    # OS_LINUX is PUBLIC: Propagates to any consumer of the library.
    target_compile_definitions(${target_name} PUBLIC OS_LINUX ${TARGET_DEF} SFML_STATIC)
    target_compile_options(${target_name} PRIVATE ${ARCH_FLAGS})

    if(is_debug)
        target_compile_definitions(${target_name} PRIVATE MODE_DEBUG)
        target_compile_options(${target_name} PRIVATE -g -Og -Wall -Wextra)
    else()
        # MODE_RELEASE is PRIVATE (only for building the lib) but essential.
        target_compile_definitions(${target_name} PRIVATE MODE_RELEASE)
        
        # Extreme compiler optimizations
        # Added IEEE 754 safe math flags and -ffat-lto-objects
        target_compile_options(${target_name} PRIVATE 
            -O3 
            -fno-math-errno 
            -fno-trapping-math
            -ffat-lto-objects
            -fomit-frame-pointer 
            -Wno-stringop-overflow
        )
        
        # Enable LTO (Link Time Optimization) if supported
        if(lto_supported)
            set_property(TARGET ${target_name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
            message(STATUS "LTO Enabled for ${target_name}")
        else()
            target_compile_options(${target_name} PRIVATE -flto=auto)
        endif()
    endif()

    target_link_libraries(${target_name} PUBLIC ${NGINE_DEPS} ${SYSTEM_DEPS})

    install(TARGETS ${target_name}
        EXPORT NGNTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()

# 7. VARIANT GENERATION
# The first call (FALSE FALSE) generates libngn.a with MODE_RELEASE and OS_LINUX
build_ngn_variant(ngn_s    "ngn"     FALSE FALSE) 
build_ngn_variant(ngn_d_s  "ngn_d"   FALSE TRUE)  

# 8. HEADER INSTALLATION
install(DIRECTORY source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NGN FILES_MATCHING PATTERN "*.h")

# 9. MODERN CONFIGURATION
include(CMakePackageConfigHelpers)
set(NGN_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/NGN")
set(NGN_INSTALL_PKG_DIR   "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(EXPORT NGNTargets FILE NGNTargets.cmake NAMESPACE NGN:: DESTINATION ${NGN_INSTALL_CMAKE_DIR})

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config-version.cmake"
    VERSION ${PROJECT_VERSION} 
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "cmake/ngn-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config.cmake"
    INSTALL_DESTINATION ${NGN_INSTALL_CMAKE_DIR}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config-version.cmake"
    DESTINATION ${NGN_INSTALL_CMAKE_DIR}
)

set(PKG_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(PKG_LIBDIR "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
set(PKG_INCDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")

configure_file("cmake/ngn.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" DESTINATION ${NGN_INSTALL_PKG_DIR})

message(STATUS "${BoldGreen}NGN SDK Static Forge (RG35XX) Ready!${ColorReset}")