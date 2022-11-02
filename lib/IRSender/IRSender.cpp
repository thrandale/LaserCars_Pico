#include "IRSender.h"

IRSender::IRSender(PIO pio, uint startPin, uint numPins)
{
    this->pio = pio;
    this->startPin = startPin;
    this->numPins = numPins;
}

int IRSender::init()
{
    // Constructor
}

IRSender::~IRSender()
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