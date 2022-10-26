# build a normal library
add_library(IRRxGroup IRRxGroup.cpp)

# invoke pio_asm to assemble the state machine program
pico_generate_pio_header(IRRxGroup ${CMAKE_CURRENT_LIST_DIR}/nec_receive.pio)

target_link_libraries(IRRxGroup
  pico_stdlib
  hardware_pio
)

# add the `binary` directory so that the generated headers are included in the project
target_include_directories(IRRxGroup PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)
