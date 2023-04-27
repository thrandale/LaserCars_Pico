
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <map>
#include <stdio.h>

#include "IRReceiver.pio.h"

#define NUM_RECEIVERS 4
#define IR_REC_START_PIN 18

#define IR_REC_PIO pio0

class IRReceiver
{
public:
    static void Init();
    static uint8_t Decode(uint32_t frame);
    static uint32_t *Receive();

private:
    static int sms[NUM_RECEIVERS];
    static uint pins[NUM_RECEIVERS];
};
