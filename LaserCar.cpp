#include "hardware/pio.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "BTController.h"
#include "Drive.h"
#include "IRReceiver.h"
#include "IRSender.h"

// Core 1 runs the WeaponController (And subsequently the IR Receiver)
void core1_entry()
{
    uint8_t rx_data = -1;
    uint pins[] = {4, 5};
    uint numPins = sizeof(pins) / sizeof(pins[0]);
    IRReceiver receiver(pio1, pins, numPins);

    while (true)
    {
        uint32_t *rx_frames = receiver.Receive();
        for (int i = 0; i < numPins; i++)
        {
            if (rx_frames[i] != -1)
            {
                rx_data = receiver.Decode(rx_frames[i]);

                if (rx_data != -1)
                {
                    printf("\t%d received: %02x", i, rx_data);
                }
                else
                {
                    printf("\t%d received: %08x", i, rx_frames[i]);
                }
            }
        }

        delete[] rx_frames;
    }
}

int main()
{
    // initialize the pico
    stdio_init_all();

    // initialize the wifi chip
    cyw43_arch_init();

    gpio_init(15);
    gpio_init(16);
    gpio_init(17);
    gpio_init(22);
    gpio_init(26);

    Drive::Init();
    BTController::Start();

    while (true)
    {
        for (int i = 0; i < 6; i++)
        {
            gpio_put(15, 0);
            gpio_put(16, 0);
            gpio_put(17, 0);
            int pin22 = gpio_get(22);
            int pin26 = gpio_get(26);
            printf("22: %d, 26: %d\n", pin22, pin26);
        }

        gpio_put(15, 1);
        gpio_put(16, 0);
        gpio_put(17, 0);
        gpio_put(22, 0);
        gpio_put(26, 0);
    }
}