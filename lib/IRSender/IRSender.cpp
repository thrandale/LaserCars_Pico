#include "IRSender.h"

IRSender::IRSender(PIO pio, uint startPin, uint numPins)
{
    this->pio = pio;
    this->startPin = startPin;
    this->numPins = numPins;
}

int IRSender::init()
{
    // install the burst program in the PIO shared instruction space
    if (pio_can_add_program(pio, &IRSender_burst_program))
    {
        burstOffset = pio_add_program(pio, &IRSender_burst_program);
    }
    else
    {
        return -1;
    }

    // claim an unused state machine on this PIO
    int burstSm = pio_claim_unused_sm(pio, true);
    if (burstSm == -1)
    {
        return -1;
    }

    // configure and enable the state machine
    IRSender_burst_program_init(pio, burstSm, burstOffset, startPin, numPins, 38.222e3); // 38.222 kHz carrier

    // install the control program in the PIO shared instruction space
    if (pio_can_add_program(pio, &IRSender_control_program))
    {
        controlOffset = pio_add_program(pio, &IRSender_control_program);
    }
    else
    {
        return -1;
    }

    // claim an unused state machine on this PIO
    int controlSm = pio_claim_unused_sm(pio, true);
    if (controlSm == -1)
    {
        return -1;
    }

    // configure and enable the state machine
    IRSender_control_program_init(pio, controlSm, controlOffset, startPin, numPins);

    return 0;
}

void IRSender::send(uint8_t data)
{
    // message is 8 bits, 5 bits for the car id, 3 bits for the side.

    // get the side
    uint8_t side = data & 0b00000111;

    // set the pin to activate
    uint8_t pinToActivate = 1 << side;

    // create the asm set command
    // set pindirs, side
    // set(111) delay(00000) dest(100) data(00000)
    uint16_t command = 0xE080 | pinToActivate;

    // send the pindirs command
    pio_sm_put_blocking(pio, burstSm, command);

    // encode the message
    uint32_t message = encodeMessage(data);

    // send the message
    // blocking because we need to wait for the command to finish
    pio_sm_put_blocking(pio, burstSm, message);
}

uint32_t IRSender::encodeMessage(uint8_t data)
{
    // duplicate the data 4 times
    uint32_t message = (data << 24) | (data << 16) | (data << 8) | data;

    return message;
}