#ifndef CONNECTINGANIMATION_H
#define CONNECTINGANIMATION_H
#include "Animation.h"
#include "math.h"
#include "pico/stdlib.h"

class ConnectingAnimation : public Animation
{
public:
    ConnectingAnimation();

    double GetFrame(int frame);
};

#endif