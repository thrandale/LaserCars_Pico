#ifndef WEAPONCONTROLLER_H
#define WEAPONCONTROLLER_H

#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include <array>

#include "IRReceiver.h"

#define NUM_MPLEX_ADDRESS_PINS 3
#define NUM_MPLEX_DATA_PINS 3
#define NUM_MPLEX_CHANNELS 8
#define NUM_WEAPONS 5
#define LONE_PIN 22
#define MAX_PLAYERS 32

typedef struct
{
    uint8_t side;
    uint8_t data;
} queue_entry_t;

class WeaponController
{
public:
    static void Init(queue_t *hitQueue, queue_t *weaponQueue);
    static void Run();

private:
    static const std::array<uint, NUM_MPLEX_ADDRESS_PINS> addressPins;
    static const std::array<uint, NUM_MPLEX_DATA_PINS> mPlexDataPins;

    static std::array<uint8_t, NUM_RECEIVERS> hitData;
    static std::array<uint8_t, NUM_WEAPONS> weaponData;
    static std::array<bool, NUM_WEAPONS> weaponDataChanged;

    static uint8_t ReadMPlex(uint8_t address);

    static queue_t *hitQueue;
    static queue_t *weaponQueue;

    static void CollectHitData();
    static void CollectWeaponData();
};

#endif