# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set the name of the project
project(LaserCar C CXX ASM)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set the board
set(PICO_BOARD "pico_w")

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialize the SDK
pico_sdk_init()

# Add executable. Default name is the project name
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link any libraries here
target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  hardware_pio
  pico_multicore
  pico_btstack_cyw43
  pico_neopixel
  # Add other libraries here
  HitAnimation
  Animation
  LightController
  FireController
  Drive
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
