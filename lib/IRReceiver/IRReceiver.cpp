#include "IRReceiver.h"

int IRReceiver::sms[4] = {-1, -1, -1, -1};
uint IRReceiver::pins[4];

/// @brief Initializes the IR receiver on pins 18-21
void IRReceiver::Init()
{
    // init the pins
    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        pins[i] = PIN_IR_REC_START + i;

        // disable pull-up and pull-down on gpio pins
        gpio_disable_pulls(pins[i]);
    }

    // install the program in the PIO shared instruction space
    uint offset;
    if (pio_can_add_program(PIO_IR_REC, &IRReceiver_program))
    {
        offset = pio_add_program(PIO_IR_REC, &IRReceiver_program);
    }
    else
    {
        printf("Could not add program");
    }

    // claim unused state machines on this PIO
    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        sms[i] = pio_claim_unused_sm(PIO_IR_REC, true);
        if (sms[i] == -1)
        {
            printf("Could not claim unused SM for pin %d", pins[i]);
        }
    }

    // configure and enable the state machine
    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        IRReceiver_program_init(PIO_IR_REC, sms[i], offset, pins[i]);
    }
}

/// @brief Decodes a frame
/// @param frame The frame to decode
/// @return the data if 3 of the 4 bytes match, otherwise returns -1
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

/// @brief Receives a frame
/// @details Checks if there is a frame in the FIFO and returns it, otherwise returns -1
/// @return an array of frames
std::array<uint32_t, NUM_RECEIVERS> IRReceiver::Receive()
{
    std::array<uint32_t, NUM_RECEIVERS> frames;

    for (int i = 0; i < NUM_RECEIVERS; i++)
    {
        if (!pio_sm_is_rx_fifo_empty(PIO_IR_REC, sms[i]))
        {
            frames[i] = pio_sm_get(PIO_IR_REC, sms[i]);
        }
        else
        {
            frames[i] = -1;
        }
    }
    return frames;
}
