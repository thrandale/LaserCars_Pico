#ifndef IRReceiver_H
#define IRReceiver_H

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <map>
#include <stdio.h>

#include "IRReceiver.pio.h"

class IRReceiver
{
public:
    IRReceiver(PIO pio, const uint pins[], int pinCount);

    void getData();

private:
    PIO pio;
    int sms[4] = {-1, -1, -1, -1};
    int pins[4];
    int offset;
    int pinCount;

    void init();
};

#endif