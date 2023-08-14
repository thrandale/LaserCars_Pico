#ifndef FIRECONTROLLER_H
#define FIRECONTROLLER_H

#include "Constants.h"
#include "pico/stdlib.h"
#include <array>
#include <string>

#include "Debug.h"
#include "IRSender.h"

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
    static std::array<uint8_t, NUM_WEAPONS> weaponData;
    static bool isInGame;
    static uint8_t carId;

    static uint8_t EncodeData(int side);
};

#endif