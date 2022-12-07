#include "hardware/pio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "IRReceiver.h"
#include "IRSender.h"

void core1_entry()
{
    PIO pio = pio1;
    uint8_t rx_data = -1;
    uint pins[] = {4, 5};
    uint numPins = sizeof(pins) / sizeof(pins[0]);
    IRReceiver receiver(pio, pins, numPins);

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
                    // printf("\treceived: %02x %02x %02x %02x", rx_frame >> 24, (rx_frame >> 16) & 0xff, (rx_frame >> 8) & 0xff, rx_frame & 0xff);
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
    stdio_init_all();
    multicore_launch_core1(core1_entry);

    PIO pio = pio0;
    IRSender sender(pio, 2, 2);

    // transmit and receive frames
    uint8_t tx_data = 0x00;
    int pin = 0;
    while (true)
    {
        // create a 32-bit frame and add it to the transmit FIFO
        // alternating between pins 1 and 2 of the sender
        sender.Send(tx_data, pin + 1);
        printf("\nsent: %02x", tx_data);

        sleep_ms(500);

        // increment the data and pin
        tx_data += 1;
        pin = (pin + 1) % 2;
    }
}