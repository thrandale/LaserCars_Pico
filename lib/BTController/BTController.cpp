#include "BTController.h"

int BTController::hit_notification_enabled;
int BTController::weapon_notification_enabled;
hci_con_handle_t BTController::hit_con_handle;
hci_con_handle_t BTController::weapon_con_handle;
btstack_packet_callback_registration_t BTController::hci_event_callback_registration;

const uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0xC, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'L', 'a', 's', 'e', 'r', ' ', 'C', 'a', 'r', ' ', '1',
    // Service UUIDs
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x00, 0x00};
const uint8_t adv_data_len = sizeof(adv_data);

/// @brief Starts the BTController
void BTController::Start()
{
    l2cap_init();
    sm_init();
    att_server_init(profile_data, NULL, att_write_callback);

    // inform about BTstack state
    hci_event_callback_registration.callback = packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);

    // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);
}

void BTController::NotifyHit(queue_entry_t hitEntry)
{
    if (!hit_notification_enabled)
        return;

    uint16_t data = hitEntry.side << 8 | hitEntry.data;

    att_server_notify(hit_con_handle, HIT_VALUE_HANDLE, (uint8_t *)&data, sizeof(data));
}

void BTController::NotifyWeaponData(queue_entry_t weaponEntry)
{
    if (!weapon_notification_enabled)
        return;
    uint8_t data = weaponEntry.side << 5 | weaponEntry.data;

    att_server_notify(weapon_con_handle, WEAPON_VALUE_HANDLE, &data, sizeof(data));
}

void BTController::packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    bd_addr_t local_addr;
    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint8_t event_type = hci_event_packet_get_type(packet);

    // setup advertisements
    uint16_t adv_int_min = 800;
    uint16_t adv_int_max = 800;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;

    switch (event_type)
    {
    case BTSTACK_EVENT_STATE:
        // called when the bluetooth stack is enabled
        if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
            return;
        gap_local_bd_addr(local_addr);
        PRINTF("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));

        memset(null_addr, 0, 6);
        gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
        assert(adv_data_len <= 31); // ble limitation
        gap_advertisements_set_data(adv_data_len, (uint8_t *)adv_data);
        gap_advertisements_enable(1);
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        // called when the connection was closed
        hit_notification_enabled = 0;
        DriveController::Stop();
        LightController::PlayConnecting();
        break;
    case ATT_EVENT_CONNECTED:
        LightController::PlayConnected();
        break;
    case ATT_EVENT_CAN_SEND_NOW:
        // called during att_server_request_can_send_now_event()
        break;
    default:
        break;
    }
}

int BTController::att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    if (buffer_size == 0)
        return 0;

    std::string data = std::string((char *)buffer, buffer_size);

    PRINTF("BT Received: %s\n", data.c_str());

    switch (att_handle)
    {
    case DRIVE_VALUE_HANDLE:
        DriveController::Move(data);
        break;
    case FIRE_VALUE_HANDLE:
        FireController::HandleBTData(data);
        break;
    case WEAPON_CLIENT_HANDLE:
        weapon_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
        weapon_con_handle = connection_handle;
        break;
    case HIT_CLIENT_HANDLE:
        hit_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
        hit_con_handle = connection_handle;
        break;
    case LED_SET_ZONE_HANDLE:
        LightController::HandleBTSetZone(data);
        break;
    case LED_PLAY_ANIM_HANDLE:
        LightController::HandleBTPlayAnim(data);
        break;
    }

    return 0;
}
