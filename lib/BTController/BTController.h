#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include "btstack.h"
#include "laser_car.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <string>

#include "DriveController.h"
#include "FireController.h"
#include "LightController.h"
#include "WeaponController.h"

#define APP_AD_FLAGS 0x06
#define HEARTBEAT_PERIOD_MS 1000

// clang-format off
#define DRIVE_VALUE_HANDLE  ATT_CHARACTERISTIC_10000001_6a5c_4ebb_8da6_a4471e0965ef_01_VALUE_HANDLE
#define FIRE_VALUE_HANDLE   ATT_CHARACTERISTIC_20000002_6a5c_4ebb_8da6_a4471e0965ef_01_VALUE_HANDLE
#define HIT_CLIENT_HANDLE   ATT_CHARACTERISTIC_20000003_6a5c_4ebb_8da6_a4471e0965ef_01_CLIENT_CONFIGURATION_HANDLE
#define HIT_VALUE_HANDLE    ATT_CHARACTERISTIC_20000003_6a5c_4ebb_8da6_a4471e0965ef_01_VALUE_HANDLE
// clang-format on

extern uint8_t const profile_data[];

class BTController
{
public:
    static void Start();

    static void NotifyHit(queue_entry_t hitEntry);

private:
    static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
    static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);

    static btstack_packet_callback_registration_t hci_event_callback_registration;
    static int hit_notification_enabled;
    static hci_con_handle_t hit_con_handle;
};

#endif