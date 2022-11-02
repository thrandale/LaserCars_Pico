#include "IRReceiver.h"

IRReceiver::IRReceiver(PIO pio, const uint pins[], int pinCount)
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

void IRReceiver::init()
{
    // Disable pullups on all pins
    for (int i = 0; i < pinCount; i++)
    {
        gpio_disable_pulls(pins[i]);
    }

    // Add the program to the PIO
    if (pio_can_add_program(pio, &IRReceiver_program))
    {
        offset = pio_add_program(pio, &IRReceiver_program);
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
            IRReceiver_program_init(pio, sms[i], offset, pins[i]);
        }
    }
}

void IRReceiver::getData()
{
    for (int i = 0; i < pinCount; i++)
    {
        uint32_t raw;
        if (!pio_sm_is_rx_fifo_empty(pio, sms[i]))
        {
            raw = pio_sm_get(pio, sms[i]);
        }
        else
        {
            continue;
        }

        std::map<uint8_t, uint8_t> dataMap;

        bool found = false;
        for (int j = 0; j < 4; j++)
        {
            uint8_t data = (raw >> (j * 8)) & 0xFF;

            if (dataMap.find(data) == dataMap.end())
            {
                dataMap[data] = 1;
            }
            else
            {
                dataMap[data]++;
            }

            if (dataMap[data] >= 3)
            {
                printf("Received %d: %02x\n", i, data);
                found = true;
                break;
            }
        }

        if (!found)
        {
            printf("Received %d: %08x\n", i, raw);
        }

        // if (f.address == (f.inverted_address ^ 0xff) &&
        //     f.data == (f.inverted_data ^ 0xff))
        // {
        //     printf("Received %d: %02x, %02x\n", i, f.address, f.data);
        // }
        // else
        // {
        //     printf("Received %d: %08x\n", i, f.raw);
        // }
    }
}
