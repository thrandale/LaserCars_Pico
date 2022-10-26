#include "IRRxGroup.h"

IRRxGroup::IRRxGroup(PIO pio, const uint pins[], int pinCount)
{
    this->pio = pio;
    this->offset = 0;
    this->pinCount = pinCount;

    for (int i = 0; i < this->pinCount; i++)
    {
        this->pins[i] = pins[i];
    }

    init();
}

void IRRxGroup::init()
{
    // Disable pullups on all pins
    for (int i = 0; i < pinCount; i++)
    {
        gpio_disable_pulls(pins[i]);
    }

    // Add the program to the PIO
    if (pio_can_add_program(pio, &nec_receive_program))
    {
        offset = pio_add_program(pio, &nec_receive_program);
    }
    else
    {
        printf("Error: PIO program could not be added\n");
    }

    for (int i = 0; i < pinCount; i++)
    {
        // claim unused sm
        sms[i] = pio_claim_unused_sm(pio, true);
        if (sms[i] == -1)
        {
            printf("Error: No free state machines available\n");
        }
        else
        {
            // init the program in the sm
            nec_receive_program_init(pio, sms[i], offset, pins[i]);
        }
    }
}

void IRRxGroup::getData()
{
    for (int i = 0; i < pinCount; i++)
    {
        union
        {
            uint32_t raw;
            struct
            {
                uint8_t address;
                uint8_t inverted_address;
                uint8_t data;
                uint8_t inverted_data;
            };
        } f;
        if (!pio_sm_is_rx_fifo_empty(pio, sms[i]))
        {
            f.raw = pio_sm_get(pio, sms[i]);
        }

        if (f.address != (f.inverted_address ^ 0xff) ||
            f.data != (f.inverted_data ^ 0xff))
        {
            printf("Received: %08x\n", f.raw);
        }
        else
        {
            printf("Received: %02x, %02x\n", f.address, f.data);
        }
    }
}
