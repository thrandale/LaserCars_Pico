#include "ConnectingAnimation.h"

ConnectingAnimation::ConnectingAnimation() : Animation()
{
    numFrames = 80;
    msPerFrame = 20;
    shouldLoop = true;
}

double ConnectingAnimation::GetFrame(int frame)
{
    return std::abs(std::sin(((double)frame / (double)numFrames) * ANIMATION_PI));
}