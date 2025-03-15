# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/pico/pico-sdk/cmake/pico-vscode.cmake)

if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# option(FREERTOS_KERNEL_PATH "Path to FreeRTOS-Kernel" ${CMAKE_BINARY_DIR}/freertos/src/FreeRTOS)
set(FREERTOS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/freertos)
set(FREERTOS_KERNEL_PATH ${FREERTOS_SOURCE_DIR})
message(STATUS "Using FreeRTOS Kernel Path: ${FREERTOS_KERNEL_PATH}")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(${FREERTOS_SOURCE_DIR}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# include(library.cmake)
project(hello_freertos C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(FREERTOS_INCLUDE_DIR ${FREERTOS_SOURCE_DIR}/include)
set(FREERTOS_PORT_DIR ${FREERTOS_SOURCE_DIR}/portable/ThirdParty/GCC/RP2040)

include_directories(${FREERTOS_INCLUDE_DIR})
include_directories(${FREERTOS_PORT_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR})

# Add executable. Default name is the project name, version 0.1
if(TARGET tinyusb_device)
    add_executable(${CMAKE_PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/main.c
    )

    # pull in common dependencies
    target_link_libraries(${CMAKE_PROJECT_NAME} pico_stdlib FreeRTOS-Kernel-Static)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)
    pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# add url via pico_set_program_url
elseif(PICO_ON_DEVICE)
    message("Skipping hello_serial because TinyUSB submodule is not initialized in the SDK")
endif()
