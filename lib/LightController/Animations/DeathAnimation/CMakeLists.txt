# build Library
add_library(DeathAnimation DeathAnimation.cpp)

target_link_libraries(
  DeathAnimation
  pico_stdlib
  Animation
)

target_include_directories (DeathAnimation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)