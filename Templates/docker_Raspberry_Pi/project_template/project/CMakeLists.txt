# ==============================================================================
# N'GINE PROJECT - RPi4 Cross-Compile Master CMakeLists
# ==============================================================================
# Target:       Raspberry Pi 4 (ARM64 / Cortex-A72)
# OS:           Linux (Debian/RPi OS)
# Description:  Strict Release Mode. Static NGN linking. Dynamic Deps.
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# 1. PROJECT INITIALIZATION
if(NOT DEFINED BINARY_NAME)
    set(BINARY_NAME "Sample_Project")
endif()

if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION "1.0.0")
endif()

project(${BINARY_NAME} LANGUAGES CXX C VERSION ${PROJECT_VERSION})

message(STATUS "--------------------------------------------------------")
message(STATUS " N'GINE BUILD SYSTEM - RPi4 Cross-Compiler")
message(STATUS " Target: ${BINARY_NAME} v${PROJECT_VERSION}")
message(STATUS " Architecture: ARM64 (Cortex-A72)")
message(STATUS "--------------------------------------------------------")

# 2. CROSS-COMPILATION DEFINITIONS
set(TARGET_DEF "TARGET_RASPBERRY")
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# The Dockerfile installed libs here:
set(CROSS_ROOT "/usr/aarch64-linux-gnu")

# Header search paths
# FIX: Added ${CROSS_ROOT}/include/NGN so <ngn.h> can be found directly
set(EXTRA_INCLUDES 
    source 
    ${CROSS_ROOT}/include 
    ${CROSS_ROOT}/include/NGN 
    ${CROSS_ROOT}/include/SDL2 
)

# Library search paths
set(EXTRA_LIB_PATHS 
    ${CROSS_ROOT}/lib 
)

# 3. DYNAMIC DEPENDENCY RESOLUTION (Shared Objects .so)
# We want SDL2 and SFML to be dynamic to keep the binary small
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

find_library(SDL2_LIB NAMES SDL2 PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SDL2_TTF_LIB NAMES SDL2_ttf PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SFML_SYS_LIB sfml-system PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)
find_library(SFML_AUD_LIB sfml-audio PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)

# 4. STATIC SDK RESOLUTION (Force Static .a)
# We link NGN statically as requested.
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# We strictly search in our cross-root to avoid system mix-ups
find_library(NGN_LIB NAMES ngn PATHS ${EXTRA_LIB_PATHS} NO_DEFAULT_PATH)

# Validation
foreach(LIB_VAR NGN_LIB SDL2_LIB SDL2_TTF_LIB SFML_SYS_LIB SFML_AUD_LIB)
    if(NOT ${LIB_VAR})
        message(FATAL_ERROR "CRITICAL: Dependency ${LIB_VAR} NOT FOUND in ${CROSS_ROOT}.")
    endif()
endforeach()

message(STATUS " Resolving dependencies...")
message(STATUS "  - NGN Core (Static): ${NGN_LIB}")
message(STATUS "  - SDL2 (Shared):     ${SDL2_LIB}")
message(STATUS "  - SFML (Shared):     ${SFML_SYS_LIB}")

# 5. SOURCE & EXECUTABLE
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")
add_executable(${BINARY_NAME} ${SOURCES})

# Target requirements: C++20
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_include_directories(${BINARY_NAME} PRIVATE ${EXTRA_INCLUDES})

# DEFINES: Explicitly adding TARGET_RASPBERRY
target_compile_definitions(${BINARY_NAME} PRIVATE 
    OS_LINUX 
    TARGET_RASPBERRY 
    MODE_RELEASE
    _REENTRANT
)

# 6. PERFORMANCE TUNING (Cortex-A72 Specific)
# -mcpu=cortex-a72 is critical here.
target_compile_options(${BINARY_NAME} PRIVATE 
    -mcpu=cortex-a72 
    -mtune=cortex-a72 
    -O3 
    -fno-math-errno 
    -fno-trapping-math 
    -flto=auto 
    -fomit-frame-pointer 
    -ffunction-sections 
    -fdata-sections
    -Wno-psabi 
    -Wno-stringop-overflow
)

# 7. LINKER CONFIGURATION
target_link_options(${BINARY_NAME} PRIVATE 
    -mcpu=cortex-a72 
    -O3 
    -flto=auto 
    -Wl,--gc-sections 
    -Wl,--disable-new-dtags
)

# RPATH for Portability on RPi
set_target_properties(${BINARY_NAME} PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE 
    INSTALL_RPATH "\$ORIGIN/libs"
)

target_link_libraries(${BINARY_NAME} PRIVATE 
    ${NGN_LIB} 
    ${SDL2_LIB} 
    ${SDL2_TTF_LIB} 
    ${SFML_SYS_LIB} 
    ${SFML_AUD_LIB}
    # Cross-compiler system libs usually needed when static linking parts
    pthread
)

message(STATUS " Build configuration complete.")
message(STATUS "--------------------------------------------------------")