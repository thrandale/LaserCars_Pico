#include "ConnectedAnimation.h"

ConnectedAnimation::ConnectedAnimation() : Animation()
{
    numFrames = 4;
    msPerFrame = 150;
    shouldLoop = false;
}

double ConnectedAnimation::GetFrame(int frame)
{
    return (frame + 1) % 2;
}
