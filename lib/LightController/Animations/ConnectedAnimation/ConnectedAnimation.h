#ifndef CONNECTEDANIMATION_H
#define CONNECTEDANIMATION_H
#include "Animation.h"
#include "pico/stdlib.h"

class ConnectedAnimation : public Animation
{
public:
    ConnectedAnimation();

    double GetFrame(int frame);
};

#endif