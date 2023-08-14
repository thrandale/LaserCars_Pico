#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <array>
#include <map>
#include <stdio.h>

#include "Constants.h"
#include "Debug.h"
#include "IRReceiver.pio.h"

class IRReceiver
{
public:
    static void Init();
    static uint8_t Decode(uint32_t frame);
    static std::array<uint32_t, NUM_RECEIVERS> Receive();

private:
    static int sms[NUM_RECEIVERS];
    static uint pins[NUM_RECEIVERS];
};
