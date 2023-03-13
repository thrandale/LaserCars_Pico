# build Library
add_library(Drive Drive.cpp)

target_link_libraries(
  Drive
  pico_stdlib
  hardware_pwm
)

target_include_directories (Drive PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)