#ifndef IRRXGROUP_H
#define IRRXGROUP_H

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "nec_receive.pio.h"

class IRRxGroup
{
public:
    IRRxGroup(PIO pio, const uint pins[], int pinCount);

    void getData();

private:
    PIO pio;
    int sms[4];
    int pins[4];
    int offset;
    int pinCount;

    void init();
};

#endif