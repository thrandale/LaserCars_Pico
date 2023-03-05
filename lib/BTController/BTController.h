#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include "btstack.h"
#include "laser_car.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <string>

#define APP_AD_FLAGS 0x06
#define HEARTBEAT_PERIOD_MS 1000

// clang-format off
#define CUSTOM_CHARACTERISTIC_VALUE_HANDLE                  ATT_CHARACTERISTIC_00000001_6a5c_4ebb_8da6_a4471e0965ef_01_VALUE_HANDLE
#define CUSTOM_CHARACTERISTIC_CLIENT_CONFIGURATION_HANDLE   ATT_CHARACTERISTIC_00000001_6a5c_4ebb_8da6_a4471e0965ef_01_CLIENT_CONFIGURATION_HANDLE
#define CUSTOM_CHARACTERISTIC_2_VALUE_HANDLE                ATT_CHARACTERISTIC_00000002_6a5c_4ebb_8da6_a4471e0965ef_01_VALUE_HANDLE
// clang-format on

extern uint8_t const profile_data[];

class BTController
{
public:
    static void Start();

    static std::string GetValue1();
    static std::string GetValue2();

private:
    static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
    static void heartbeat_handler(async_context_t *context, async_at_time_worker_t *worker);
    static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
    static void increment();

    static async_at_time_worker_t heartbeat_worker;
    static btstack_packet_callback_registration_t hci_event_callback_registration;

    static int le_notification_enabled;
    static hci_con_handle_t con_handle;
    static uint16_t counter;
    static char counter_str[10];
    static int counter_str_len;

    static std::string value1;
    static std::string value2;
};

#endif