#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include <array>
#include <map>
#include <stdio.h>
#include <string>

#include "Adafruit_NeoPixel.h"
#include "Animation.h"
#include "ConnectedAnimation.h"
#include "ConnectingAnimation.h"
#include "Constants.h"
#include "DeathAnimation.h"
#include "HitAnimation.h"

#define NUM_LIGHTS 3
#define NUM_ZONES 3

enum Zone
{
    FRONT = 0,
    MIDDLE = 1,
    BACK = 2
};

typedef void (*FunctionPtr)();

class LightController
{
public:
    static void Init();

    static void SetColor(uint8_t r, uint8_t g, uint8_t b, Zone zone);

    static void PlayHit();
    static void PlayDeath();

    static void PlayConnecting();
    static void PlayConnected();

    static void Run();

    static void HandleBTSetZone(std::string data);
    static void HandleBTPlayAnim(std::string data);

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

private:
    static Adafruit_NeoPixel pixels;
    static Animation *currentAnimation;
    static int currentFrame;
    static uint64_t timeOfLastFrame;

    static HitAnimation hitAnimation;
    static DeathAnimation deathAnimation;
    static ConnectingAnimation connectingAnimation;
    static ConnectedAnimation connectedAnimation;

    static std::map<int, FunctionPtr> animations;

    static std::array<uint8_t, NUM_ZONES> r;
    static std::array<uint8_t, NUM_ZONES> g;
    static std::array<uint8_t, NUM_ZONES> b;

    static std::array<bool, NUM_ZONES> changed;
    static double brightness;

    static void StartAnimation();
    static void SetAllChanged();
};

#endif