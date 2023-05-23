#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "Constants.h"
// import the assembled PIO state machine programs
#include "IRSender_burst.pio.h"
#include "IRSender_control.pio.h"

// public API
class IRSender
{
public:
    static void Init();
    static void Send(uint8_t data, uint8_t sides);

private:
    static int burstSM;
    static int controlSM;

    static uint32_t Encode(uint8_t data);
};
