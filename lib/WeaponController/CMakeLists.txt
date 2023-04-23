# build Library
add_library(WeaponController WeaponController.cpp)

target_link_libraries(
  WeaponController
  pico_stdlib
  pico_multicore
  IRReceiver
  hardware_pio
)

target_include_directories (WeaponController PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)