#include "hardware/pio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "IRReceiver.h"
#include "IRSender.h"

void core1_entry()
{
    uint receiverPins[] = {2, 3, 4, 5};
    IRReceiver receiver(pio1, receiverPins, sizeof(receiverPins) / sizeof(receiverPins[0]));

    while (true)
    {
        receiver.getData();
    }
}

uint32_t encodeMessage(uint8_t data)
{
    return (data << 24) | (data << 16) | (data << 8) | data;
}

int main()
{
    stdio_init_all();
    multicore_launch_core1(core1_entry);

    uint senderPin = 6;
    PIO pio = pio0;

    printf("Hello, world!\n\n");

    uint8_t data = 0x00;
    while (true)
    {
        // Send the IR signal
        uint32_t frame = encodeMessage(data);
        printf("\nSent      : %02x\n", data);

        sleep_ms(1000);
        data++;
    }
}