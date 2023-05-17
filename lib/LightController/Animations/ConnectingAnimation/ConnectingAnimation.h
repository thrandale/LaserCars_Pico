#ifndef CONNECTINGANIMATION_H
#define CONNECTINGANIMATION_H
#include "Animation.h"
#include "math.h"
#include "pico/stdlib.h"

#define ANIMATION_PI 3.14159265

class ConnectingAnimation : public Animation
{
public:
    ConnectingAnimation();

    double GetFrame(int frame);
};

#endif