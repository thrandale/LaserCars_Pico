#include "LightController.h"

Adafruit_NeoPixel LightController::pixels;
uint64_t LightController::timeOfLastFrame;
Animation *LightController::currentAnimation;
int LightController::currentFrame;

HitAnimation LightController::hitAnimation;
DeathAnimation LightController::deathAnimation;
ConnectingAnimation LightController::connectingAnimation;
ConnectedAnimation LightController::connectedAnimation;

std::array<uint8_t, NUM_ZONES> LightController::r;
std::array<uint8_t, NUM_ZONES> LightController::g;
std::array<uint8_t, NUM_ZONES> LightController::b;

std::array<bool, NUM_ZONES> LightController::changed;
double LightController::brightness = 1;

void LightController::Init()
{
    pixels = Adafruit_NeoPixel(NUM_LIGHTS, LIGHT_PIN);

    r.fill(0);
    g.fill(0);
    b.fill(0);
    changed.fill(false);

    timeOfLastFrame = 0;
    currentAnimation = nullptr;
    currentFrame = 0;

    hitAnimation = HitAnimation();
    deathAnimation = DeathAnimation();
    connectingAnimation = ConnectingAnimation();
    connectedAnimation = ConnectedAnimation();

    PlayConnecting();
}

/// @brief Sets the color of the lights in the specified zone
/// @param color
/// @param zone
void LightController::SetColor(uint8_t r, uint8_t g, uint8_t b, Zone zone)
{
    LightController::r[zone] = r;
    LightController::g[zone] = g;
    LightController::b[zone] = b;
    changed[zone] = true;
}

void LightController::PlayHit()
{
    currentAnimation = &hitAnimation;
    StartAnimation();
}

void LightController::PlayDeath()
{
    currentAnimation = &deathAnimation;
    StartAnimation();
}

void LightController::PlayConnecting()
{
    LightController::SetColor(22, 100, 221, Zone::FRONT);
    LightController::SetColor(22, 100, 221, Zone::MIDDLE);
    LightController::SetColor(22, 100, 221, Zone::BACK);

    currentAnimation = &connectingAnimation;
    StartAnimation();
}

void LightController::PlayConnected()
{
    currentAnimation = &connectedAnimation;
    StartAnimation();
}

void LightController::StartAnimation()
{
    currentFrame = 0;
    timeOfLastFrame = to_ms_since_boot(get_absolute_time());
    brightness = currentAnimation->GetFrame(currentFrame);
    SetAllChanged();
}

void LightController::Run()
{
    if (currentAnimation != nullptr)
    {
        uint64_t currentTime = to_ms_since_boot(get_absolute_time());
        int msPerFrame = currentAnimation->GetMsPerFrame();

        if (currentTime - timeOfLastFrame >= msPerFrame)
        {
            // update the time
            uint64_t timeExceeded = (currentTime - timeOfLastFrame - msPerFrame) % msPerFrame;
            timeOfLastFrame = currentTime - timeExceeded;

            // update the frame
            currentFrame++;

            // check if we need to loop or stop
            if (currentFrame >= currentAnimation->GetNumFrames())
            {
                currentFrame = 0;
                if (!currentAnimation->GetShouldLoop())
                {
                    currentAnimation = nullptr;
                    brightness = 1;
                }
                else
                {
                    brightness = currentAnimation->GetFrame(currentFrame);
                }
            }
            else
            {
                brightness = currentAnimation->GetFrame(currentFrame);
            }

            SetAllChanged();
        }
    }

    bool didChange = false;

    for (int i = 0; i < NUM_ZONES; i++)
    {
        if (changed[i])
        {
            uint32_t color = pixels.Color(r[i] * brightness, g[i] * brightness, b[i] * brightness);
            switch ((Zone)i)
            {
            case FRONT:
                pixels.fill(color, 0, 2);
                break;
            case MIDDLE:
                pixels.fill(color, 2, 2);
                pixels.fill(color, 6, 2);
                break;
            case BACK:
                pixels.fill(color, 4, 2);
                break;
            }
            changed[i] = false;
            didChange = true;
        }
    }

    if (didChange)
    {
        pixels.show();
    }
}

uint32_t LightController::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return pixels.Color(r, g, b);
}

void LightController::SetAllChanged()
{
    for (int i = 0; i < NUM_ZONES; i++)
    {
        changed[i] = true;
    }
}