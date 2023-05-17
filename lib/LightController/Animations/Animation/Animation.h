#ifndef ANIMATION_H
#define ANIMATION_H
#include "pico/stdlib.h"

class Animation
{
public:
    Animation();
    virtual double GetFrame(int frame) = 0;

    int GetNumFrames() { return numFrames; }
    int GetMsPerFrame() { return msPerFrame; }
    bool GetShouldLoop() { return shouldLoop; }

protected:
    int numFrames;
    int msPerFrame;
    bool shouldLoop;
};

#endif