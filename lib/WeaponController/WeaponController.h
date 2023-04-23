#ifndef WEAPONCONTROLLER_H
#define WEAPONCONTROLLER_H
#include "IRReceiver.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

#define NUM_RECEIVER_PINS 4
#define NUM_MPLEX_ADDRESS_PINS 3
#define NUM_MPLEX_DATA_PINS 3
#define NUM_MPLEX_CHANNELS 8
#define NUM_WEAPONS 5
#define LONE_PIN 22
#define MAX_PLAYERS 32
#define PIO_INSTANCE pio1

typedef struct
{
    uint8_t side;
    uint8_t data;
} queue_entry_t;

class WeaponController
{
public:
    WeaponController(queue_t *hitQueue, queue_t *weaponQueue);
    ~WeaponController();

    void Run();

private:
    IRReceiver receiver;
    uint receiverPins[NUM_RECEIVER_PINS] = {18, 19, 20, 21};

    uint8_t *hitData;
    uint8_t *weaponData;
    bool *weaponDataChanged;

    uint addressPins[NUM_MPLEX_ADDRESS_PINS] = {15, 16, 17};
    uint mPlexDataPins[NUM_MPLEX_DATA_PINS] = {26, 27, 28};

    uint8_t ReadMPlex(uint8_t address);

    queue_t *hitQueue;
    queue_t *weaponQueue;

    void CollectHitData();
    void CollectWeaponData();
};

#endif