#ifndef DRIVE_H
#define DRIVE_H
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdio.h>

struct Motor
{
    uint8_t pin1;
    uint8_t pin2;
};

class Drive
{
public:
    static void Init();
    static void Mecanum(double angle, double magnitude, double rotation);
    static void Tank(double magnitude, double rotation);
    static void Stop();

private:
    static void SetMotor(Motor motor, double power);

    static const Motor MOTORS[4];
};

#endif