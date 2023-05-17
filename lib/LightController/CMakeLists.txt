# build Library
add_library(LightController LightController.cpp)

target_link_libraries(
  LightController
  pico_stdlib
  Animation
  HitAnimation
  DeathAnimation
  ConnectingAnimation
  ConnectedAnimation
  pico_neopixel
  hardware_pio
  pico_time
)

target_include_directories (LightController PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_subdirectory(Animations)