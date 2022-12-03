#include "hardware/pio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "IRReceiver.h"
#include "IRSender.h"

void core1_entry()
{
    PIO pio = pio1;
    uint rx_gpio = 15;
    uint8_t rx_data = -1;

    IRReceiver receiver(pio, rx_gpio);

    while (true)
    {
        uint32_t rx_frame = receiver.Receive();
        if (rx_frame != -1)
        {
            rx_data = receiver.Decode(rx_frame);

            if (rx_data != -1)
            {
                printf("\treceived: %02x", rx_data);
                // printf("\treceived: %02x %02x %02x %02x", rx_frame >> 24, (rx_frame >> 16) & 0xff, (rx_frame >> 8) & 0xff, rx_frame & 0xff);
            }
            else
            {
                printf("\treceived: %08x", rx_frame);
            }
        }
    }
}

// uint32_t encodeMessage(uint8_t data)
// {
//     return (data << 24) | (data << 16) | (data << 8) | data;
// }

int main()
{
    stdio_init_all();

    PIO pio = pio0;    // choose which PIO block to use (RP2040 has two: pio0 and pio1)
    uint tx_gpio = 14; // choose which GPIO pin is connected to the IR LED

    IRSender sender(pio, tx_gpio);

    multicore_launch_core1(core1_entry);

    // transmit and receive frames
    uint8_t tx_data = 0x00;
    while (true)
    {
        // create a 32-bit frame and add it to the transmit FIFO
        sender.Send(tx_data);
        printf("\nsent: %02x", tx_data);

        // allow time for the frame to be transmitted (optional)
        sleep_ms(100);

        sleep_ms(900);
        tx_data += 1;
    }
}