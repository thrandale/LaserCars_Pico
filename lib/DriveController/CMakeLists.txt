# build Library
add_library(DriveController DriveController.cpp)

target_link_libraries(
  DriveController
  pico_stdlib
  hardware_pwm
)

target_include_directories (DriveController PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)