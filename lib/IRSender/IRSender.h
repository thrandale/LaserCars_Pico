#ifndef IRSENDER_H
#define IRSENDER_H

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

#include "IRSender_burst.pio.h"
#include "IRSender_control.pio.h"

class IRSender
{
public:
    IRSender(PIO pio, uint startPin, uint numPins);

    int init();
    void send(uint8_t data);

private:
    // Private members
    PIO pio;
    uint startPin;
    uint numPins;
    uint burstOffset;
    uint controlOffset;
    uint burstSm;
    uint controlSm;

    uint32_t encodeMessage(uint8_t data);
};

#endif