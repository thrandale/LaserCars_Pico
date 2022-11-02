# build a normal library
add_library(IRReceiver IRReceiver.cpp)

# invoke pio_asm to assemble the state machine program
pico_generate_pio_header(IRReceiver ${CMAKE_CURRENT_LIST_DIR}/IRReceiver.pio)

target_link_libraries(IRReceiver
  pico_stdlib
  hardware_pio
)

# add the `binary` directory so that the generated headers are included in the project
target_include_directories(IRReceiver PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)
