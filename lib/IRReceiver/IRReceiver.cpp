#include "IRReceiver.h"

IRReceiver::IRReceiver(PIO pio, uint pins[], int numPins)
{

    this->pio = pio;
    this->numPins = numPins;

    for (int i = 0; i < 4; i++)
    {
        this->sms[i] = -1;
    }

    // disable pull-up and pull-down on gpio pins
    for (int i = 0; i < this->numPins; i++)
    {
        gpio_disable_pulls(pins[i]);
    }

    // install the program in the PIO shared instruction space
    uint offset;
    if (pio_can_add_program(this->pio, &IRReceiver_program))
    {
        offset = pio_add_program(this->pio, &IRReceiver_program);
    }
    else
    {
        printf("Could not add program");
    }

    // claim unused state machines on this PIO
    for (int i = 0; i < this->numPins; i++)
    {
        this->sms[i] = pio_claim_unused_sm(this->pio, true);
        if (this->sms[i] == -1)
        {
            printf("Could not claim unused SM for pin %d", pins[i]);
        }
    }

    // configure and enable the state machine
    for (int i = 0; i < this->numPins; i++)
    {
        IRReceiver_program_init(this->pio, this->sms[i], offset, pins[i]);
    }
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
uint32_t *IRReceiver::Receive()
{
    uint32_t *frames = new uint32_t[this->numPins];

    for (int i = 0; i < this->numPins; i++)
    {
        if (!pio_sm_is_rx_fifo_empty(this->pio, this->sms[i]))
        {
            frames[i] = pio_sm_get(this->pio, this->sms[i]);
        }
        else
        {
            frames[i] = -1;
        }
    }
    return frames;
}
