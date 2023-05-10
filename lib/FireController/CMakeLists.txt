# build Library
add_library(FireController FireController.cpp)

target_link_libraries(
  FireController
  pico_stdlib
  IRSender
  hardware_pio
)

target_include_directories (FireController PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)