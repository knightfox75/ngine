cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "Ngine_Project")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION 0.1.0)

# 1. ARQUITECTURA Y DEFINES
set(ARCH_RASPBERRY "Raspberry")
set(CMAKE_ARCH_TYPE ${ARCH_RASPBERRY})

# 2. RUTAS SDK (Entorno Docker)
set(CMAKE_BASE_PATH "/usr/local")
set(EXTRA_INCLUDES 
    source
    ${CMAKE_BASE_PATH}/include 
    ${CMAKE_BASE_PATH}/include/NGN
    ${CMAKE_BASE_PATH}/include/SDL2 
    ${CMAKE_BASE_PATH}/include/SFML
    /usr/include
)
set(EXTRA_LIB_PATHS ${CMAKE_BASE_PATH}/lib)

# 3. BÚSQUEDA DE LIBRERÍAS
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
find_library(NGN_LIB      NAMES ngn         REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_LIB     NAMES SDL2        REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SDL2_TTF_LIB NAMES SDL2_ttf    REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_SYS_LIB sfml-system REQUIRED PATHS ${EXTRA_LIB_PATHS})
find_library(SFML_AUD_LIB sfml-audio  REQUIRED PATHS ${EXTRA_LIB_PATHS})

# 4. FUENTES
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

# 5. TARGET
add_executable(${BINARY_NAME} ${SOURCES})
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_include_directories(${BINARY_NAME} PRIVATE ${EXTRA_INCLUDES})

# DEFINES DE TU CMAKE
target_compile_definitions(${BINARY_NAME} PRIVATE
    OS_LINUX
    TARGET_RASPBERRY
    MODE_RELEASE
)

# FLAGS DE OPTIMIZACIÓN
target_compile_options(${BINARY_NAME} PRIVATE 
    -march=armv8-a -mcpu=cortex-a72 -ffast-math -O3 -flto=auto -Wno-stringop-overflow
)

# 6. LINKING Y RPATH
set(LIB_SUBDIR "aarch64-linux-gnu")

# Usamos '\$ORIGIN' (con barra) para que CMake no lo evalúe y llegue el símbolo $ al Linker.
set_target_properties(${BINARY_NAME} PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN/libs/${LIB_SUBDIR}"
)

target_link_options(${BINARY_NAME} PRIVATE 
    -flto=auto -s -Wno-stringop-overflow
    -Wl,--disable-new-dtags
)

target_link_libraries(${BINARY_NAME} PRIVATE 
    ${NGN_LIB} ${SDL2_LIB} ${SDL2_TTF_LIB} ${SFML_SYS_LIB} ${SFML_AUD_LIB}
)