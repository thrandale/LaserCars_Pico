#include "hardware/clocks.h" // for clock_get_hz()
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
    IRSender(PIO pio, uint pin);
    uint32_t nec_encode_frame(uint8_t address, uint8_t data);
    void Send(uint8_t data);

private:
    PIO pio;
    uint controlProgramOffset;
    uint burstProgramOffset;
    int burstSM;
    int controlSM;

    uint32_t Encode(uint8_t data);
};
