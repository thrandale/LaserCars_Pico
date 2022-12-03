#include "IRReceiver.h"

IRReceiver::IRReceiver(PIO pio, uint pin)
{

    this->pio = pio;
    this->sm = -1;

    // disable pull-up and pull-down on gpio pin
    gpio_disable_pulls(pin);

    // install the program in the PIO shared instruction space
    uint offset;
    if (pio_can_add_program(pio, &IRReceiver_program))
    {
        offset = pio_add_program(pio, &IRReceiver_program);
    }
    else
    {
        printf("Could not add program");
    }

    // claim an unused state machine on this PIO
    sm = pio_claim_unused_sm(pio, true);
    if (sm == -1)
    {
        printf("Could not claim unused SM");
    }

    // configure and enable the state machine
    IRReceiver_program_init(pio, sm, offset, pin);
}

// Decode a frame
// returns the data if 3 of the 4 bytes match
// otherwise returns -1
uint8_t IRReceiver::Decode(uint32_t frame)
{
    std::map<uint8_t, uint8_t> dataMap;

    for (int j = 0; j < 4; j++)
    {
        uint8_t data = (frame >> (j * 8)) & 0xFF;
        if (j > 1)
        {
            data = data ^ 0xFF;
        }

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
            return data;
        }
    }

    return -1;
}

// Receive a frame
// checks if there is a frame in the FIFO and returns it
// otherwise returns -1
uint32_t IRReceiver::Receive()
{
    if (!pio_sm_is_rx_fifo_empty(pio, sm))
    {
        return pio_sm_get(pio, sm);
    }

    return -1;
}
