# ==============================================================================
# N'GINE UTILITIES - Universal Master CMakeLists
# ==============================================================================
# Targets: Windows (MinGW), Linux (x86_64), Raspberry Pi (Cortex-A72)
# ==============================================================================

cmake_minimum_required(VERSION 3.14)

# ------------------------------------------------------------------------------
# 1. TOOL IDENTITY & ARCHITECTURE
# ------------------------------------------------------------------------------
if(NOT DEFINED TOOL_NAME)
    set(TOOL_NAME "NGN_CollisionMap")
endif()

project(${TOOL_NAME} LANGUAGES CXX)

set(ARCH_WINDOWS_X86_64 "Windows_x86_64")
set(ARCH_LINUX_X86_64   "Linux_x86_64")
set(ARCH_RASPBERRY      "Raspberry")

if(NOT CMAKE_ARCH_TYPE)
    message(FATAL_ERROR "CMAKE_ARCH_TYPE must be defined.")
endif()

# ------------------------------------------------------------------------------
# 2. SOURCE & TARGET INITIALIZATION
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    enable_language(RC)
    set(RES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.rc")
    if(EXISTS "${RES_FILE}")
        list(APPEND SOURCES "${RES_FILE}")
    endif()
endif()

add_executable(${TOOL_NAME} ${SOURCES})

target_compile_features(${TOOL_NAME} PRIVATE cxx_std_20)
target_include_directories(${TOOL_NAME} PRIVATE source)

# ------------------------------------------------------------------------------
# 3. GLOBAL PREPROCESSOR DEFINITIONS
# ------------------------------------------------------------------------------
target_compile_definitions(${TOOL_NAME} PRIVATE 
    MODE_CONSOLE
    $<$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_WINDOWS_X86_64}>:OS_WINDOWS TARGET_X86>
    $<$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_LINUX_X86_64}>:OS_LINUX TARGET_PC_LINUX>
    $<$<STREQUAL:${CMAKE_ARCH_TYPE},${ARCH_RASPBERRY}>:OS_LINUX TARGET_RASPBERRY>
    $<$<CONFIG:Debug>:MODE_DEBUG _DEBUG>
    $<$<CONFIG:Release>:MODE_RELEASE NDEBUG>
)

# ------------------------------------------------------------------------------
# 4. COMPILATION & LINKING STRATEGY
# ------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TOOL_NAME} PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(${TOOL_NAME} PRIVATE 
        -O3 -flto=auto -fno-math-errno -fomit-frame-pointer -fdata-sections -ffunction-sections
    )
    target_link_options(${TOOL_NAME} PRIVATE 
        -O3 -flto=auto -s -static -Wl,--gc-sections
    )
endif()

# ------------------------------------------------------------------------------
# 5. ARCHITECTURE TUNING & EXPORT CONFIG
# ------------------------------------------------------------------------------
# Default suffix is empty
set(NAME_SUFFIX "")

if(CMAKE_ARCH_TYPE STREQUAL ARCH_WINDOWS_X86_64)
    set(PLATFORM_DIR "windows")
    set(EXE_EXT ".exe")
    target_compile_options(${TOOL_NAME} PRIVATE -march=x86-64-v2 -mtune=generic)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_options(${TOOL_NAME} PRIVATE -static-libgcc -static-libstdc++)
    endif()

elseif(CMAKE_ARCH_TYPE STREQUAL ARCH_LINUX_X86_64)
    set(PLATFORM_DIR "linux")
    set(EXE_EXT "")
    target_compile_options(${TOOL_NAME} PRIVATE -march=x86-64-v2 -mtune=generic)

elseif(CMAKE_ARCH_TYPE STREQUAL ARCH_RASPBERRY)
    set(PLATFORM_DIR "raspberry")
    set(EXE_EXT "")
    set(NAME_SUFFIX "_Raspbian") # Suffix applied here
    target_compile_options(${TOOL_NAME} PRIVATE -mcpu=cortex-a72 -mtune=cortex-a72)
endif()

# ------------------------------------------------------------------------------
# 6. POST-BUILD: ORGANIZED DEPLOYMENT
# ------------------------------------------------------------------------------
set(EXPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/exports/${PLATFORM_DIR}")

add_custom_command(TARGET ${TOOL_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${EXPORT_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TOOL_NAME}> "${EXPORT_PATH}/${TOOL_NAME}${NAME_SUFFIX}${EXE_EXT}"
    COMMENT "Deploying standalone binary to: exports/${PLATFORM_DIR}/${TOOL_NAME}${NAME_SUFFIX}${EXE_EXT}"
)