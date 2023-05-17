#ifndef HITANIMATION_H
#define HITANIMATION_H
#include "Animation.h"
#include "pico/stdlib.h"
#include <stdio.h>

class HitAnimation : public Animation
{
public:
    HitAnimation();

    double GetFrame(int frame);
};

#endif