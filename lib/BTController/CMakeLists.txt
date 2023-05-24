add_library(BTController STATIC
    BTController.cpp
)

target_link_libraries(BTController
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_adc
    DriveController
    LightController
    FireController
    WeaponController
)

target_include_directories (BTController PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/generated
	)

target_include_directories(BTController PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )

pico_btstack_make_gatt_header(BTController PRIVATE "${CMAKE_CURRENT_LIST_DIR}/laser_car.gatt")