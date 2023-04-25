add_library(pico_neopixel INTERFACE)

pico_generate_pio_header(pico_neopixel ${CMAKE_CURRENT_LIST_DIR}/ws2812byte.pio)

target_sources(pico_neopixel INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Adafruit_NeoPixel.cpp
)

target_include_directories(pico_neopixel INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(pico_neopixel INTERFACE
  pico_stdlib
  pico_malloc
  pico_mem_ops
)