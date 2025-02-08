# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ====================================================================================

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.12)

# Set the board
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Set the name of the project
project(LaserCar C CXX ASM)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialize the SDK
pico_sdk_init()

# Add executable. Default name is the project name
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# To get the Constants.h file
include_directories(lib)

# Link any libraries here
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_cyw43_arch_none
    hardware_pio
    pico_multicore
    pico_btstack_cyw43
    pico_neopixel
    pico_sync

    # Add other libraries here
    HitAnimation
    Animation
    LightController
    FireController
    DriveController
    BTController
    IRSender
    IRReceiver
    WeaponController
)

# Add other subdirectories here
add_subdirectory(lib)

# Enable uart output and disable usb output
# Switch this if not using picoprobe
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
