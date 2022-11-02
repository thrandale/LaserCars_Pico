#include "hardware/pio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "IRRxGroup.h"
#include "nec_transmit.h"

void core1_entry()
{
    uint receiverPins[] = {2, 3, 4, 5};
    IRRxGroup receiver(pio1, receiverPins, sizeof(receiverPins) / sizeof(receiverPins[0]));

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

    int senderSM = nec_tx_init(pio, senderPin);

    pio_sm_set_consecutive_pindirs_with_mask(pio, senderSM, 1 << senderPin, 1 << senderPin, true);

    if (senderSM == -1)
    {
        printf("Error initializing PIO\n");
        return 1;
    }

    printf("Hello, world!\n\n");

    uint8_t data = 0x00;
    while (true)
    {
        // Send the IR signal
        uint32_t frame = encodeMessage(data);
        pio_sm_put(pio, senderSM, frame);
        printf("\nSent      : %02x\n", data);

        sleep_ms(1000);
        data++;
    }
}