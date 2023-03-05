#include "hardware/pio.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "BTController.h"
#include "IRReceiver.h"
#include "IRSender.h"

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

    // start the second core
    multicore_launch_core1(core1_entry);

    // Start the BT controller
    BTController::Start();

    // initialize the IR sender
    IRSender sender(pio0, 2, 2);
    uint8_t tx_data = 0x00;
    int pin = 0;

    while (true)
    {
        printf("------------------------\n");
        // Read the values from the BT controller
        printf("BT Value 1: %s\n", BTController::GetValue1().c_str());
        printf("BT Value 2: %s\n", BTController::GetValue2().c_str());

        // create a 32-bit frame and add it to the transmit FIFO
        // alternating between pins 1 and 2 of the sender
        sender.Send(tx_data, pin + 1);
        printf("Sent IR: %02x\n", tx_data);

        // increment the data and pin
        tx_data += 1;
        pin = (pin + 1) % 2;

        sleep_ms(500);
    }
}