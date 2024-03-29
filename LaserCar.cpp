#include "hardware/pio.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include <stdio.h>

#include "Adafruit_NeoPixel.h"
#include "BTController.h"
#include "Constants.h"
#include "DriveController.h"
#include "FireController.h"
#include "LightController.h"
#include "WeaponController.h"

queue_t hitQueue;
queue_t weaponQueue;
queue_t sendQueue;

// Core 1 runs the WeaponController (And subsequently the IR Receiver)
// as well as the LightController
void core1_entry()
{
    WeaponController::Init(&hitQueue, &weaponQueue);
    LightController::Init();

    while (true)
    {
        WeaponController::Run();
        LightController::Run();
        sleep_ms(5);
    }
}

int main()
{
    // initialize the pico
    stdio_init_all();

    // initialize the wifi chip
    cyw43_arch_init();

    // Initialize the queues
    queue_init(&hitQueue, sizeof(queue_entry_t), 32);
    queue_init(&weaponQueue, sizeof(queue_entry_t), 32);

    // Start the second core
    multicore_launch_core1(core1_entry);

    BTController::Start();
    DriveController::Init();
    FireController::Init();

    while (true)
    {

        // receive hits
        queue_entry_t hitEntry;
        while (queue_try_remove(&hitQueue, &hitEntry))
        {
            BTController::NotifyHit(hitEntry);
        }

        // receive weapon data
        queue_entry_t weaponEntry;
        while (queue_try_remove(&weaponQueue, &weaponEntry))
        {
            BTController::NotifyWeaponData(weaponEntry);
        }
    }
}