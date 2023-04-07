#ifndef DRIVE_H
#define DRIVE_H
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdio.h>
#include <string>

#define PI 3.14159265358979323846
#define MIN_SPEED 0.3

struct Motor
{
    uint8_t pin1;
    uint8_t pin2;
};

class Drive
{
public:
    static void Init();
    static void Move(std::string data);
    static void Stop();

private:
    static void SetMotor(Motor motor, double power);

    static const Motor MOTORS[4];
    static const double DEADZONE;

    static double ExtractMagnitude(std::string value);
    static double ExtractRotation(std::string value);
    static double ExtractAngle(std::string value);
    static bool CheckStop(std::string value);
};

#endif