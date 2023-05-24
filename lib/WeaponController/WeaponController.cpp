#include "WeaponController.h"

const std::array<uint, NUM_MPLEX_ADDRESS_PINS> WeaponController::addressPins = PIN_MPLEX_ADDRESSES;
const std::array<uint, NUM_MPLEX_DATA_PINS> WeaponController::mPlexDataPins = PIN_MPLEX_DATA;

std::array<uint8_t, NUM_RECEIVERS> WeaponController::hitData;
std::array<uint8_t, NUM_WEAPONS> WeaponController::weaponData;
std::array<bool, NUM_WEAPONS> WeaponController::weaponDataChanged;

queue_t *WeaponController::hitQueue;
queue_t *WeaponController::weaponQueue;

/// @brief Initializes the WeaponController
/// @param hitQueue
/// @param weaponQueue
void WeaponController::Init(queue_t *hitQueue, queue_t *weaponQueue)
{
    IRReceiver::Init();

    hitData.fill(-1);
    weaponData.fill(-1);
    weaponDataChanged.fill(false);

    WeaponController::hitQueue = hitQueue;
    WeaponController::weaponQueue = weaponQueue;
}

/// @brief Collects the hit and weapon data and puts it in the appropriate queues
/// @note This function is non-blocking and should be called from the main loop
void WeaponController::Run()
{
    CollectHitData();
    CollectWeaponData();

    for (int i = 0; i < NUM_WEAPONS; i++)
    {
        if (weaponDataChanged[i])
        {
            queue_entry_t entry = {(uint8_t)i, weaponData[i]};
            if (queue_try_add(weaponQueue, &entry))
            {
                weaponDataChanged[i] = false;
            }
        }
    }

    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        if (hitData[i] != (uint8_t)-1)
        {
            queue_entry_t entry = {(uint8_t)i, hitData[i]};
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
    std::array<uint32_t, NUM_RECEIVERS> rx_frames = IRReceiver::Receive();
    uint8_t rx_data = -1;
    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        if (rx_frames[i] != (uint32_t)-1)
        {
            rx_data = IRReceiver::Decode(rx_frames[i]);

            if (rx_data != (uint8_t)-1 && hitData[i] == (uint8_t)-1)
            {
                hitData[i] = rx_data;
                printf("\tReceiver %d received data: %02x\n", i, rx_data);
            }
            else if (rx_data == (uint8_t)-1)
            {
                printf("\tReceiver %d received unknown data: %08x\n", i, rx_frames[i]);
            }
        }
    }
}

/// @brief Collects the weapon data from the multiplexers
void WeaponController::CollectWeaponData()
{
    uint32_t collectedData = 0;

    for (int i = 0; i < NUM_MPLEX_CHANNELS; i++)
    {
        collectedData |= ReadMPlex(i) << (i * NUM_MPLEX_DATA_PINS);
    }
    collectedData |= gpio_get(PIN_LONE_WEAPON_DATA) << (NUM_MPLEX_CHANNELS * NUM_MPLEX_DATA_PINS);

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
