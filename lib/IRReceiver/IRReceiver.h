
#include "hardware/clocks.h" // for clock_get_hz()
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <map>
#include <stdio.h>

#include "IRReceiver.pio.h"

class IRReceiver
{
public:
    IRReceiver(PIO pio, uint pin);
    bool nec_decode_frame(uint32_t sm, uint8_t *p_address, uint8_t *p_data);
    uint8_t Decode(uint32_t frame);
    uint32_t Receive();

private:
    PIO pio;
    int sm;
};
