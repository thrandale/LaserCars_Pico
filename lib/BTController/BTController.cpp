#include "BTController.h"

int BTController::le_notification_enabled;
hci_con_handle_t BTController::con_handle;
btstack_packet_callback_registration_t BTController::hci_event_callback_registration;

const uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0xC, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'L', 'a', 's', 'e', 'r', ' ', 'C', 'a', 'r', ' ', '1',
    // Service UUIDs
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x00, 0x00};
const uint8_t adv_data_len = sizeof(adv_data);

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
        printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));

        memset(null_addr, 0, 6);
        gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
        assert(adv_data_len <= 31); // ble limitation
        gap_advertisements_set_data(adv_data_len, (uint8_t *)adv_data);
        gap_advertisements_enable(1);
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        // called when the connection was closed
        le_notification_enabled = 0;
        Drive::Stop();
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

    std::string data = std::string((char *)buffer);

    printf("BT Received: %s\n", data.c_str());

    switch (att_handle)
    {
    case DRIVE_VALUE_HANDLE:
        Drive::Move(data);
        break;
    }
    return 0;
}
