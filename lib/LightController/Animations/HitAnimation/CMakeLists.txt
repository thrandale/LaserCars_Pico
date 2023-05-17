# build Library
add_library(HitAnimation HitAnimation.cpp)

target_link_libraries(
  HitAnimation
  pico_stdlib
  Animation
)

target_include_directories (HitAnimation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)