#ifndef FIRECONTROLLER_H
#define FIRECONTROLLER_H

#include "pico/stdlib.h"
#include <array>
#include <string>

#include "IRSender.h"

#define NULL_SIDE 0b111
#define FIRE_NUM_WEAPONS IR_SEND_NUM_PINS

class FireController
{
public:
    static void Init();
    static void JoinGame(uint8_t carId);
    static void LeaveGame();
    static void UpdateWeapon(int weapon, uint8_t weaponData);

    static void Fire(int *sides, int numSides);

    static void HandleBTData(std::string data);

private:
    static std::array<uint8_t, FIRE_NUM_WEAPONS> weaponData;
    static bool isInGame;
    static uint8_t carId;

    static uint8_t EncodeData(int side);
};

#endif