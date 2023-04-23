#include "WeaponController.h"

/// @brief Construct a new WeaponController object
/// @param hitQueue the queue to put hit data in
/// @param weaponQueue the queue to put weapon data in
WeaponController::WeaponController(queue_t *hitQueue, queue_t *weaponQueue)
{
    IRReceiver receiver(PIO_INSTANCE, receiverPins, NUM_RECEIVER_PINS);
    hitData = new uint8_t[NUM_RECEIVER_PINS];
    weaponData = new uint8_t[NUM_WEAPONS];
    weaponDataChanged = new bool[NUM_WEAPONS];

    for (int i = 0; i < NUM_RECEIVER_PINS; i++)
    {
        hitData[i] = -1;
    }

    for (int i = 0; i < NUM_WEAPONS; i++)
    {
        weaponData[i] = -1;
        weaponDataChanged[i] = false;
    }

    this->hitQueue = hitQueue;
    this->weaponQueue = weaponQueue;
}

/// @brief Destroy the WeaponController object
WeaponController::~WeaponController()
{
    delete[] hitData;
    delete[] weaponData;
    delete[] weaponDataChanged;
}

/// @brief Collects the hit and weapon data and puts it in the appropriate queues
/// @note This function is non-blocking and should be called from the main loop
void WeaponController::Run()
{
    CollectHitData();
    CollectWeaponData();

    if (weaponDataChanged)
    {
        for (int i = 0; i < NUM_WEAPONS; i++)
        {
            if (weaponDataChanged[i])
            {
                queue_entry_t entry = {i, weaponData[i]};
                if (queue_try_add(weaponQueue, &entry))
                {
                    weaponDataChanged[i] = false;
                }
            }
        }
    }

    for (int i = 0; i < NUM_RECEIVER_PINS; i++)
    {
        if (hitData[i] != -1)
        {
            queue_entry_t entry = {i, hitData[i]};
            if (queue_try_add(hitQueue, &entry))
            {
                hitData[i] = -1;
            }
        }
    }
}

/// @brief Collects the hit data from the IR receivers
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

/// @brief Collects the weapon data from the multiplexers
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
            weaponDataChanged[i] = true;
        }
    }
}

/// @brief Reads the data from a multiplexer
/// @param address The address of the multiplexer to read from
/// @return The data from the multiplexer
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
