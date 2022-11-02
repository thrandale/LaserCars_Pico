# build Library
add_library(IRSender IRSender.cpp)


pico_generate_pio_header(IRSender ${CMAKE_CURRENT_LIST_DIR}/IRSender_burst.pio)
pico_generate_pio_header(IRSender ${CMAKE_CURRENT_LIST_DIR}/IRSender_control.pio)


target_link_libraries(
  IRSender
  pico_stdlib
)

# add the `binary` directory so that the generated headers are included in the project
target_include_directories(IRSender PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)