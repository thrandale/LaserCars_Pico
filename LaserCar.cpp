#include "hardware/pio.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include <stdio.h>

#include "Adafruit_NeoPixel.h"
#include "BTController.h"
#include "Drive.h"
#include "IRSender.h"
#include "LightController.h"
#include "WeaponController.h"

queue_t hitQueue;
queue_t weaponQueue;
queue_t sendQueue;

// Core 1 runs the WeaponController (And subsequently the IR Receiver)
void core1_entry()
{
    WeaponController::Init(&hitQueue, &weaponQueue);

    while (true)
    {
        WeaponController::Run();
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

    Drive::Init();
    BTController::Start();
    IRSender::Init();

    LightController::Init();
    
    while (true)
    {
        LightController::Run();
    }
}