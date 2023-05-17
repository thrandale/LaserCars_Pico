#include "DeathAnimation.h"

DeathAnimation::DeathAnimation() : Animation()
{
    numFrames = 1;
    msPerFrame = 250;
    shouldLoop = false;
}

double DeathAnimation::GetFrame(int frame)
{
    return 0.0;
}
