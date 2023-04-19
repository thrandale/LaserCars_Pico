#include "WeaponController.h"

WeaponController::WeaponController()
{
    IRReceiver receiver(PIO_INSTANCE, receiverPins, NUM_RECEIVER_PINS);
    hitData = new uint8_t[NUM_RECEIVER_PINS];
    weaponData = new uint8_t[NUM_RECEIVER_PINS];

    for (int i = 0; i < NUM_RECEIVER_PINS; i++)
    {
        hitData[i] = -1;
        weaponData[i] = -1;
    }
}

WeaponController::~WeaponController()
{
    delete[] hitData;
    delete[] weaponData;
}

void WeaponController::CollectHitData()
{
    uint32_t *rx_frames = receiver.Receive();
    uint8_t rx_data = -1;
    for (int i = 0; i < NUM_RECEIVER_PINS; i++)
    {
        if (rx_frames[i] != -1)
        {
            rx_data = receiver.Decode(rx_frames[i]);

            if (rx_data != -1 && hitData[i] == -1)
            {
                hitData[i] = rx_data;
            }
            else if (rx_data == -1)
            {
                printf("Receiver %d received unknown data: %08x\n", i, rx_frames[i]);
            }
        }
    }

    delete[] rx_frames;
}

void WeaponController::CollectWeaponData()
{
    uint32_t collectedData = 0;

    for (int i = 0; i < NUM_MPLEX_CHANNELS; i++)
    {
        collectedData |= ReadMPlex(i) << (i * NUM_MPLEX_DATA_PINS);
    }
    collectedData |= gpio_get(LONE_PIN) << (NUM_MPLEX_CHANNELS * NUM_MPLEX_DATA_PINS);

    for (int i = 0; i < NUM_WEAPONS; i++)
    {
        uint8_t data = (collectedData >> (i * 5)) & 0x1F;
        if (weaponData[i] != data)
        {
            weaponData[i] = data;
            weaponDataChanged = true;
        }
    }
}

uint8_t WeaponController::ReadMPlex(uint8_t address)
{
    uint8_t data = 0;

    for (int i = 0; i < NUM_MPLEX_ADDRESS_PINS; i++)
    {
        gpio_put(addressPins[i], (address >> i) & 1);
    }

    for (int i = 0; i < NUM_MPLEX_DATA_PINS; i++)
    {
        data |= gpio_get(mPlexDataPins[i]) << i;
    }

    return data;
}
