#ifndef DEATHANIMATION_H
#define DEATHANIMATION_H
#include "Animation.h"
#include "pico/stdlib.h"

class DeathAnimation : public Animation
{
public:
    DeathAnimation();

    double GetFrame(int frame);
};

#endif