#include "BTController.h"

int BTController::le_notification_enabled;
hci_con_handle_t BTController::con_handle;
uint16_t BTController::counter;
char BTController::counter_str[10];
int BTController::counter_str_len;
btstack_packet_callback_registration_t BTController::hci_event_callback_registration;

async_at_time_worker_t BTController::heartbeat_worker = {.do_work = BTController::heartbeat_handler};

std::string BTController::value1 = "Hello";
std::string BTController::value2 = "World";

const uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0xA, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'L', 'a', 's', 'e', 'r', ' ', 'C', 'a', 'r',
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

    // set one-shot btstack timer
    async_context_add_at_time_worker_in_ms(cyw43_arch_async_context(), &heartbeat_worker, HEARTBEAT_PERIOD_MS);

    // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);
}

std::string BTController::GetValue1()
{
    return value1;
}

std::string BTController::GetValue2()
{
    return value2;
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
        break;
    case ATT_EVENT_CAN_SEND_NOW:
        // called during att_server_request_can_send_now_event()
        att_server_notify(con_handle, CUSTOM_CHARACTERISTIC_VALUE_HANDLE, (uint8_t *)counter_str, counter_str_len);
        break;
    default:
        break;
    }
}

void BTController::heartbeat_handler(async_context_t *context, async_at_time_worker_t *worker)
{
    // increment the counter
    increment();

    // send notification if enabled
    if (le_notification_enabled)
    {
        att_server_request_can_send_now_event(con_handle);
    }

    // Invert the led
    static int led_on = true;
    led_on = !led_on;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);

    // Restart timer
    async_context_add_at_time_worker_in_ms(context, &heartbeat_worker, HEARTBEAT_PERIOD_MS);
}

int BTController::att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    switch (att_handle)
    {
    case CUSTOM_CHARACTERISTIC_CLIENT_CONFIGURATION_HANDLE:
        le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
        con_handle = connection_handle;
        if (le_notification_enabled)
        {
            att_server_request_can_send_now_event(con_handle);
        }
        break;
    case CUSTOM_CHARACTERISTIC_VALUE_HANDLE:
        value1 = std::string((char *)buffer);
        break;
    case CUSTOM_CHARACTERISTIC_2_VALUE_HANDLE:
        value2 = std::string((char *)buffer);
        break;
    }
    return 0;
}

void BTController::increment(void)
{
    counter++;
    counter %= 1000;
    counter_str_len = sprintf(counter_str, "%d", counter);
}