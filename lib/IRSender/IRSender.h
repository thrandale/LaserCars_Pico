#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

// import the assembled PIO state machine programs
#include "IRSender_burst.pio.h"
#include "IRSender_control.pio.h"

// public API
class IRSender
{
public:
    IRSender(PIO pio, uint pin, uint numPins);
    void Send(uint8_t data, uint pin);

private:
    PIO pio;
    uint controlProgramOffset;
    uint burstProgramOffset;
    int burstSM;
    int controlSM;

    uint32_t Encode(uint8_t data);
};
