
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <map>
#include <stdio.h>

#include "IRReceiver.pio.h"

class IRReceiver
{
public:
    IRReceiver(PIO pio, uint pin);
    uint8_t Decode(uint32_t frame);
    uint32_t Receive();

private:
    PIO pio;
    int sm;
};
