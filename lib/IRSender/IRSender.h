#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

// import the assembled PIO state machine programs
#include "IRSender_burst.pio.h"
#include "IRSender_control.pio.h"

#define IR_SEND_START_PIN 10
#define IR_SEND_NUM_PINS 5
#define BURST_PIO pio0
#define CONTROL_PIO pio1

// public API
class IRSender
{
public:
    static void Init();
    static void Send(uint8_t data, uint pin);

private:
    static int burstSM;
    static int controlSM;

    static uint32_t Encode(uint8_t data);
};
