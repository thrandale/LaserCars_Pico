# build Library
add_library(Animation Animation.cpp)

target_link_libraries(
  Animation
  pico_stdlib
)

target_include_directories (Animation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)