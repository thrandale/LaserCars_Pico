# build Library
add_library(ConnectingAnimation ConnectingAnimation.cpp)

target_link_libraries(
  ConnectingAnimation
  pico_stdlib
  Animation
)

target_include_directories (ConnectingAnimation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)