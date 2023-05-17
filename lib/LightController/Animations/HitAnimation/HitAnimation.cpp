#include "HitAnimation.h"

HitAnimation::HitAnimation() : Animation()
{
    numFrames = 3;
    msPerFrame = 150;
    shouldLoop = false;
}

double HitAnimation::GetFrame(int frame)
{
    return frame % 2;
}
