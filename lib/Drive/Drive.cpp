#include "Drive.h"

Motor const Drive::MOTORS[] = {
    {2, 3},
    {4, 5},
    {6, 7},
    {8, 9}};

const double Drive::DEADZONE = PI / 4;

void Drive::Init()
{
    for (Motor motor : MOTORS)
    {
        gpio_set_function(motor.pin1, GPIO_FUNC_PWM);
        gpio_set_function(motor.pin2, GPIO_FUNC_PWM);

        uint slice_num1 = pwm_gpio_to_slice_num(motor.pin1);
        uint slice_num2 = pwm_gpio_to_slice_num(motor.pin2);

        pwm_set_enabled(slice_num1, true);
        pwm_set_enabled(slice_num2, true);
    }
}

void Drive::Mecanum(double angle, double magnitude, double rotation)
{
    double ADPower = 0;
    double BCPower = 0;

    int closestAngle = std::round(angle / (PI / 4));

    magnitude = magnitude > 0 ? magnitude / 2 + 0.5 : 0;
    rotation = rotation != 0 ? rotation / 2 + (0.5 * rotation / std::abs(rotation)) : 0;

    switch (closestAngle)
    {
    case 0:
    case 8:
        ADPower = magnitude;
        BCPower = -magnitude;
        break;
    case 1:
        ADPower = magnitude;
        BCPower = 0;
        break;
    case 2:
        ADPower = magnitude;
        BCPower = magnitude;
        break;
    case 3:
        ADPower = 0;
        BCPower = magnitude;
        break;
    case 4:
        ADPower = -magnitude;
        BCPower = magnitude;
        break;
    case 5:
        ADPower = -magnitude;
        BCPower = 0;
        break;
    case 6:
        ADPower = -magnitude;
        BCPower = -magnitude;
        break;
    case 7:
        ADPower = 0;
        BCPower = -magnitude;
        break;
    }

    // check if turning power will interfere with normal translation
    // check ADPower to see if trying to apply rotation would put motor power over 1.0 or under -1.0
    double turningScale = std::max(std::abs(ADPower + rotation), std::abs(ADPower - rotation));
    // check BCPower to see if trying to apply rotation would put motor power over 1.0 or under -1.0
    turningScale = std::max(turningScale, std::max(std::abs(BCPower + rotation), std::abs(BCPower - rotation)));

    // adjust turn power scale correctly
    if (std::abs(turningScale) < 1.0)
    {
        turningScale = 1.0;
    }

    // set the motors, and divide them by turningScale to make sure none of them go over the top, which would alter the translation angle
    SetMotor(MOTORS[0], (ADPower + rotation) / turningScale);
    SetMotor(MOTORS[1], (BCPower + rotation) / turningScale);
    SetMotor(MOTORS[2], (BCPower - rotation) / turningScale);
    SetMotor(MOTORS[3], (ADPower - rotation) / turningScale);
}

void Drive::Tank(double magnitude, double rotation)
{
    Mecanum(0, magnitude, rotation);
}

void Drive::Stop()
{
    SetMotor(MOTORS[0], 0);
    SetMotor(MOTORS[1], 0);
    SetMotor(MOTORS[2], 0);
    SetMotor(MOTORS[3], 0);
}

void Drive::SetMotor(Motor motor, double power)
{
    int pwmValue = (int)(std::abs(power) * 255 * 255);

    if (power == 0)
    {
        pwm_set_gpio_level(motor.pin1, 0);
        pwm_set_gpio_level(motor.pin2, 0);
    }
    else if (power > 0)
    {
        pwm_set_gpio_level(motor.pin1, pwmValue);
        pwm_set_gpio_level(motor.pin2, 0);
    }
    else
    {
        pwm_set_gpio_level(motor.pin1, 0);
        pwm_set_gpio_level(motor.pin2, pwmValue);
    }
}
