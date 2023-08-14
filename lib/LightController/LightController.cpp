#include "LightController.h"

Adafruit_NeoPixel LightController::pixels;
uint64_t LightController::timeOfLastFrame;
Animation *LightController::currentAnimation;
int LightController::currentFrame;

HitAnimation LightController::hitAnimation;
DeathAnimation LightController::deathAnimation;
ConnectingAnimation LightController::connectingAnimation;
ConnectedAnimation LightController::connectedAnimation;

std::map<int, FunctionPtr> LightController::animations = {{0, &PlayHit}, {1, &PlayDeath}};

std::array<uint8_t, NUM_ZONES> LightController::r;
std::array<uint8_t, NUM_ZONES> LightController::g;
std::array<uint8_t, NUM_ZONES> LightController::b;

std::array<uint8_t, NUM_ZONES> LightController::rTemp;
std::array<uint8_t, NUM_ZONES> LightController::gTemp;
std::array<uint8_t, NUM_ZONES> LightController::bTemp;

std::array<bool, NUM_ZONES> LightController::changed;
std::array<bool, NUM_ZONES> LightController::tempChanged;

mutex_t LightController::colorMutex;

double LightController::brightness = 1;

void LightController::Init()
{
    pixels = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NEOPIXELS);

    r.fill(0);
    g.fill(0);
    b.fill(0);

    rTemp.fill(0);
    gTemp.fill(0);
    bTemp.fill(0);

    changed.fill(false);
    tempChanged.fill(false);

    mutex_init(&colorMutex);

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
    mutex_enter_blocking(&colorMutex);
    if (currentAnimation == nullptr)
    {
        LightController::r[zone] = r;
        LightController::g[zone] = g;
        LightController::b[zone] = b;
        changed[zone] = true;
    }
    else
    {
        rTemp[zone] = r;
        gTemp[zone] = g;
        bTemp[zone] = b;
        tempChanged[zone] = true;
    }
    mutex_exit(&colorMutex);
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
    PRINTF("Playing connecting\n");
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
    else
    {
        mutex_enter_blocking(&colorMutex);
        for (int i = 0; i < NUM_ZONES; i++)
        {
            if (tempChanged[i])
            {
                r[i] = rTemp[i];
                g[i] = gTemp[i];
                b[i] = bTemp[i];
                changed[i] = true;
                tempChanged[i] = false;
            }
        }
        mutex_exit(&colorMutex);
    }

    bool didChange = false;

    mutex_enter_blocking(&colorMutex);
    for (int i = 0; i < NUM_ZONES; i++)
    {
        if (changed[i])
        {
            uint32_t color = pixels.Color(r[i] * brightness, g[i] * brightness, b[i] * brightness);
            switch ((Zone)i)
            {
            case FRONT:
                if (NUM_LIGHTS == 8)
                {
                    pixels.fill(color, 0, 2);
                }
                else
                {
                    pixels.fill(color, 0, 1);
                }
                break;
            case MIDDLE:
                if (NUM_LIGHTS == 8)
                {
                    pixels.fill(color, 2, 2);
                    pixels.fill(color, 6, 2);
                }
                else
                {
                    pixels.fill(color, 1, 1);
                }
                break;
            case BACK:
                if (NUM_LIGHTS == 8)
                {
                    pixels.fill(color, 4, 2);
                }
                else
                {
                    pixels.fill(color, 2, 1);
                }
                break;
            }
            changed[i] = false;
            didChange = true;
        }
    }
    mutex_exit(&colorMutex);

    if (didChange)
    {
        pixels.show();
    }
}

void LightController::HandleBTSetZone(std::string data)
{
    // convert the data to hex
    try
    {
        uint8_t zone = std::stoi(data.substr(0, 1), nullptr, 16);
        uint8_t r = std::stoi(data.substr(1, 2), nullptr, 16);
        uint8_t g = std::stoi(data.substr(3, 2), nullptr, 16);
        uint8_t b = std::stoi(data.substr(5, 2), nullptr, 16);

        LightController::SetColor(r, g, b, (Zone)zone);
    }
    catch (const std::exception &e)
    {
        PRINTF("Error: %s\n", e.what());
        return;
    }
}

void LightController::HandleBTPlayAnim(std::string data)
{
    PRINTF("Playing animation %s\n", data.c_str());
    int animation = std::stoi(data);

    try
    {
        if (animations.find(animation) != animations.end())
        {
            animations[animation]();
        }
    }
    catch (const std::exception &e)
    {
        PRINTF("Error: %s\n", e.what());
        return;
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