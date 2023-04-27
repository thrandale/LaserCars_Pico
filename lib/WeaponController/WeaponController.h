#ifndef WEAPONCONTROLLER_H
#define WEAPONCONTROLLER_H

#include "IRReceiver.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

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
    static const uint addressPins[NUM_MPLEX_ADDRESS_PINS];
    static const uint mPlexDataPins[NUM_MPLEX_DATA_PINS];

    static uint8_t *hitData;
    static uint8_t *weaponData;
    static bool *weaponDataChanged;

    static uint8_t ReadMPlex(uint8_t address);

    static queue_t *hitQueue;
    static queue_t *weaponQueue;

    static void CollectHitData();
    static void CollectWeaponData();
};

#endif