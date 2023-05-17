#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H
#include "Adafruit_NeoPixel.h"
#include "Animation.h"
#include "ConnectedAnimation.h"
#include "ConnectingAnimation.h"
#include "DeathAnimation.h"
#include "HitAnimation.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>

#define NUM_LIGHTS 8
#define LIGHT_PIN 1
#define NUM_ZONES 3

enum Zone
{
    FRONT = 0,
    MIDDLE = 1,
    BACK = 2
};

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

    static uint8_t r[NUM_ZONES];
    static uint8_t g[NUM_ZONES];
    static uint8_t b[NUM_ZONES];

    static bool changed[NUM_ZONES];
    static double brightness;

    static void StartAnimation();
    static void SetAllChanged();
};

#endif