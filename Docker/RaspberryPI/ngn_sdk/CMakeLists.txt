# ==============================================================================
# NGN Library - Master CMakeLists (Universal Architecture)
# ==============================================================================

cmake_minimum_required(VERSION 3.14) # Subimos version min para soportar features modernas

# 1. COLORES PARA TERMINAL
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColorReset  "${Esc}[m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
endif()

# 2. SELECCIÓN DE ARQUITECTURA
set(ARCH "x86" CACHE STRING "Target architecture (x86, raspberry, arm_eabi_hf)")

if(ARCH STREQUAL "arm_eabi_hf")
    set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_C_COMPILER   "/usr/bin/arm-linux-gnueabihf-gcc")
    set(ARCH_FLAGS "-marm;-mcpu=cortex-a7;-mfpu=neon-vfpv4;-mfloat-abi=hard")
    set(CMAKE_BASE_PATH "/usr/local/arm-linux-gnueabihf") # Rutas donde buscar LIBS (Build-time)
    set(TARGET_DEF "TARGET_ARM_EABI_HF")
elseif(ARCH STREQUAL "raspberry")
    set(ARCH_FLAGS "-march=armv8-a;-mcpu=cortex-a72")
    set(CMAKE_BASE_PATH "/usr/local")
    set(TARGET_DEF "TARGET_RASPBERRY")
else()
    set(ARCH_FLAGS "-m64;-mtune=generic")
    set(CMAKE_BASE_PATH "/usr/local")
    set(TARGET_DEF "TARGET_X86")
endif()

# 3. PROYECTO
project(ngn_library LANGUAGES CXX C VERSION 1.21.0.2)

# Modulo estandar para rutas de instalacion (lib vs lib64, etc.)
include(GNUInstallDirs)

message(STATUS "${BoldBlue}Configuring NGN Library v${PROJECT_VERSION} for ${ARCH}${ColorReset}")

# 4. RUTAS DE BÚSQUEDA (Build Time)
set(EXTRA_LIB_PATHS ${CMAKE_BASE_PATH}/lib /usr/local/lib)
set(EXTRA_INCLUDES ${CMAKE_BASE_PATH}/include ${CMAKE_BASE_PATH}/include/SDL2 ${CMAKE_BASE_PATH}/include/SFML)

# 5. BÚSQUEDA DE LIBRERÍAS
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
find_library(SDL2_LIB SDL2 REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB SDL2_ttf REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYSTEM_LIB sfml-system REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUDIO_LIB sfml-audio REQUIRED PATHS ${EXTRA_LIB_PATHS})

# Macro de reporte
macro(ngn_report_lib lib_name lib_path)
    if(${lib_path})
        message(STATUS "${lib_name} detected: ${BoldGreen}YES${ColorReset}")
    else()
        message(STATUS "${lib_name} detected: ${BoldRed}NO${ColorReset}")
    endif()
endmacro()

ngn_report_lib("SDL2" SDL2_LIB)
ngn_report_lib("SDL2_ttf" SDL2_TTF_LIB)
ngn_report_lib("SFML System" SFML_SYSTEM_LIB)
ngn_report_lib("SFML Audio" SFML_AUDIO_LIB)

set(ALL_LIBS ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYSTEM_LIB} ${SFML_AUDIO_LIB})

# 6. FUENTES
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

# 7. FUNCIÓN "FACTORY" PARA VARIANTES
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
    
    # Separamos los includes.
    # BUILD_INTERFACE: Donde están los headers mientras compilo el SDK.
    # INSTALL_INTERFACE: Donde estarán los headers cuando el usuario instale el SDK.
    target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/NGN>
        ${EXTRA_INCLUDES}
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

    # Instalación con EXPORT.
    # Esto registra este target en la lista "NGNTargets" para exportarlo luego.
    install(TARGETS ${target_name}
        EXPORT NGNTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()

# 8. GENERACIÓN DE LAS 4 VARIANTES
build_ngn_variant(ngn_s    "ngn_s"   FALSE FALSE) 
build_ngn_variant(ngn_d_s  "ngn_d_s" FALSE TRUE)  
build_ngn_variant(ngn      "ngn"     TRUE  FALSE) 
build_ngn_variant(ngn_d    "ngn_d"   TRUE  TRUE)  

# 9. INSTALACIÓN DE CABECERAS
install(DIRECTORY source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NGN FILES_MATCHING PATTERN "*.h")

# 10. CONFIGURACIÓN MODERNA (CMake Export & Pkg-config)
include(CMakePackageConfigHelpers)

# Rutas relativas de instalación para los archivos de configuración
set(NGN_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/NGN")
set(NGN_INSTALL_PKG_DIR   "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# A. Generar NGNTargets.cmake (El mapa de los targets exportados arriba)
install(EXPORT NGNTargets
    FILE NGNTargets.cmake
    NAMESPACE NGN::
    DESTINATION ${NGN_INSTALL_CMAKE_DIR}
)

# B. Generar Versión del Paquete
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# C. Configurar el archivo principal del paquete (ngn-config.cmake)
# Usamos configure_package_config_file para soportar reubicación (relocatable paths)
configure_package_config_file(
    "cmake/ngn-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config.cmake"
    INSTALL_DESTINATION ${NGN_INSTALL_CMAKE_DIR}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# D. Instalar los archivos de configuración de CMake
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ngn-config-version.cmake"
    DESTINATION ${NGN_INSTALL_CMAKE_DIR}
)

# E. Pkg-config
# Pasamos las variables correctas al .pc.in
set(PKG_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(PKG_LIBDIR "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
set(PKG_INCDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")

configure_file("cmake/ngn.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ngn.pc" DESTINATION ${NGN_INSTALL_PKG_DIR})

message(STATUS "${BoldGreen}NGN Library Master Setup Ready!${ColorReset}")