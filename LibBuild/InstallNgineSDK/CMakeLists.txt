# ==============================================================================
# NGN Library - Master CMakeLists
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# 1. COLORES PARA TERMINAL
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColorReset  "${Esc}[m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
endif()

# 2. SELECCIÓN DE ARQUITECTURA
set(ARCH "x86" CACHE STRING "Target architecture (x86, raspberry)")

if(ARCH STREQUAL "raspberry")
    set(ARCH_FLAGS "-march=armv8-a;-mcpu=cortex-a72")
    set(TARGET_DEF "TARGET_RASPBERRY")
else()
    set(ARCH_FLAGS "-m64;-mtune=generic")
    set(TARGET_DEF "TARGET_X86")
endif()

# 3. PROYECTO
project(ngn_library LANGUAGES CXX C VERSION 1.21.0.3)
include(GNUInstallDirs)

message(STATUS "${BoldBlue}Configuring NGN Library v${PROJECT_VERSION} for ${ARCH}${ColorReset}")

# 4. DEPENDENCIAS Y RUTAS
# Definimos explícitamente dónde están los headers porque instalamos manualmente en /usr/local
set(TARGET_INCLUDES 
    /usr/local/include 
    /usr/local/include/SDL2 
)

find_library(SDL2_LIB SDL2 REQUIRED)
find_library(SDL2_TTF_LIB SDL2_ttf REQUIRED)
find_library(SFML_SYSTEM_LIB sfml-system REQUIRED)
find_library(SFML_AUDIO_LIB sfml-audio REQUIRED)

set(ALL_LIBS ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB})

# 5. FUENTES
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

# 6. FUNCIÓN "FACTORY"
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
    
    # AQUI ESTÁ LA CORRECCIÓN: Añadimos ${TARGET_INCLUDES}
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NGN>
        ${TARGET_INCLUDES} 
    )

    target_compile_definitions(${target_name} PRIVATE OS_LINUX ${TARGET_DEF})
    target_compile_options(${target_name} PRIVATE ${ARCH_FLAGS})

    if(is_debug)
        target_compile_definitions(${target_name} PRIVATE MODE_DEBUG)
        target_compile_options(${target_name} PRIVATE -g -Og -Wall -Wextra)
    else()
        target_compile_definitions(${target_name} PRIVATE MODE_RELEASE)
        target_compile_options(${target_name} PRIVATE -O3 -flto=auto -ffast-math -fomit-frame-pointer -s -Wno-stringop-overflow)
        target_link_options(${target_name} PRIVATE -flto=auto -s -Wno-stringop-overflow)
    endif()

    target_link_libraries(${target_name} PUBLIC ${ALL_LIBS})

    install(TARGETS ${target_name}
        EXPORT NGNTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()

# 7. GENERACIÓN DE LAS 4 VARIANTES
build_ngn_variant(ngn_s    "ngn_s"   FALSE FALSE) 
build_ngn_variant(ngn_d_s  "ngn_d_s" FALSE TRUE)  
build_ngn_variant(ngn      "ngn"     TRUE  FALSE) 
build_ngn_variant(ngn_d    "ngn_d"   TRUE  TRUE)  

# 8. INSTALACIÓN DE CABECERAS
install(DIRECTORY source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NGN FILES_MATCHING PATTERN "*.h")

# 9. CONFIGURACIÓN MODERNA
include(CMakePackageConfigHelpers)

set(NGN_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/NGN")
set(NGN_INSTALL_PKG_DIR   "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(EXPORT NGNTargets FILE NGNTargets.cmake NAMESPACE NGN:: DESTINATION ${NGN_INSTALL_CMAKE_DIR})

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config-version.cmake"
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
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

message(STATUS "${BoldGreen}NGN Setup Ready!${ColorReset}")