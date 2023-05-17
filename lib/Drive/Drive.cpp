#include "Drive.h"

Motor const Drive::MOTORS[] = {
    {2, 3},
    {4, 5},
    {6, 7},
    {8, 9}};

/// @brief Initializes the drive system
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

/// @brief Moves the car
/// @param data The data to move the car with formatted as "angle:magnitude;rotation"
void Drive::Move(std::string data)
{
    double ADPower = 0;
    double BCPower = 0;
    double angle = 0;
    double magnitude = 0;
    double rotation = 0;

    try
    {
        if (CheckStop(data))
        {
            Drive::Stop();
            return;
        }

        angle = ExtractAngle(data);
        magnitude = ExtractMagnitude(data);
        rotation = ExtractRotation(data);
    }
    catch (const std::exception &e)
    {
        printf("Error: %s\n", e.what());
        return;
    }

    // scale the magnitude to be between MIN_SPEED and 1
    magnitude = magnitude > 0
                    ? (magnitude) * (1 - MIN_SPEED) + MIN_SPEED
                    : 0;
    rotation = rotation != 0
                   ? (rotation) * (1 - MIN_SPEED) + MIN_SPEED * rotation / std::abs(rotation)
                   : 0;

    int closestAngle = std::round(angle / (PI / 4));

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

    // make sure the turning scale doesn't go over 1
    double turningScale = std::max(
        std::max(
            std::abs(ADPower + rotation),
            std::abs(ADPower - rotation)),
        std::max(
            std::abs(BCPower + rotation),
            std::abs(BCPower - rotation)));

    // adjust turn power scale correctly
    turningScale = std::abs(turningScale) < 1 ? 1 : turningScale;

    // set the motors, and divide them by turningScale to make sure none of them go over the top,
    // which would alter the translation angle
    SetMotor(MOTORS[0], (ADPower + rotation) / turningScale);
    SetMotor(MOTORS[1], (BCPower + rotation) / turningScale);
    SetMotor(MOTORS[2], (BCPower - rotation) / turningScale);
    SetMotor(MOTORS[3], (ADPower - rotation) / turningScale);
}

/// @brief Stops the car
void Drive::Stop()
{
    SetMotor(MOTORS[0], 0);
    SetMotor(MOTORS[1], 0);
    SetMotor(MOTORS[2], 0);
    SetMotor(MOTORS[3], 0);
}

/// @brief Sets the motor to the given power
/// @param motor The motor to set
/// @param power The power to set the motor to (between -1 and 1)
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

/// @brief  Extracts the angle from the data
/// @param data The data to extract the angle from formatted as "angle:magnitude;rotation"
/// @return The angle
double Drive::ExtractAngle(std::string data)
{
    return (double)std::stoi(data.substr(0, data.find(':'))) / 100;
}

/// @brief  Extracts the magnitude from the data
/// @param data The data to extract the magnitude from formatted as "angle:magnitude;rotation"
double Drive::ExtractMagnitude(std::string data)
{
    return (double)std::stoi(data.substr(data.find(':') + 1, data.find(';'))) / 100;
}

/// @brief  Extracts the rotation from the data
/// @param data The data to extract the rotation from formatted as "angle:magnitude;rotation"
double Drive::ExtractRotation(std::string data)
{
    std::string rotationStr = data.substr(data.find(';') + 1, data.length());
    int isRotNegative = rotationStr.find("-");
    double rotation = isRotNegative != -1
                          ? std::stoi(rotationStr.substr(isRotNegative, rotationStr.length()))
                          : std::stoi(rotationStr.substr(0, rotationStr.length()));
    return rotation / 100;
}

/// @brief Checks if the data is a stop command
/// @param data The data to check if it starts with "stop"
bool Drive::CheckStop(std::string data)
{
    return data.find("stop") != -1;
}
