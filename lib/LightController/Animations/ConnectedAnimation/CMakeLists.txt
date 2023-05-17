# build Library
add_library(ConnectedAnimation ConnectedAnimation.cpp)

target_link_libraries(
  ConnectedAnimation
  pico_stdlib
  Animation
)

target_include_directories (ConnectedAnimation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)