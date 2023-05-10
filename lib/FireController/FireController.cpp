#include "FireController.h"

uint8_t *FireController::weaponData;
bool FireController::isInGame;
uint8_t FireController::carId;

/// @brief Initializes the FireController
void FireController::Init()
{
    // initialize the IR sender
    IRSender::Init();

    // initialize the weapon data
    weaponData = new uint8_t[NUM_WEAPONS];
    for (int i = 0; i < NUM_WEAPONS; i++)
    {
        weaponData[i] = 0;
    }

    carId = 0;
    isInGame = false;
}

/// @brief Joins the game
/// @param carId The ID of the car
void FireController::JoinGame(uint8_t carId)
{
    FireController::carId = carId;
    isInGame = true;
}

/// @brief Leaves the game
void FireController::LeaveGame()
{
    isInGame = false;
    carId = 0;
}

/// @brief Updates the weapon data for the specified weapon
/// @param weapon The weapon to update (0-4)
/// @param weaponData The new weapon data
void FireController::UpdateWeapon(int weapon, uint8_t weaponData)
{
    FireController::weaponData[weapon] = weaponData;
}

/// @brief Fires the weapon(s) on the specified side(s)
/// @param sides An array of sides to fire on (0-4)
/// @param numSides The number of sides to fire on
void FireController::Fire(int *sides, int numSides)
{
    // keeps track of which sides have been sent
    bool *hasBeenSent = new bool[NUM_WEAPONS];

    // iterate through the sides to shoot
    for (int i = 0; i < numSides; i++)
    {
        if (hasBeenSent[i])
        {
            continue;
        }

        uint8_t sidesToSend = 1 << sides[i];

        // check for sides with the same weapon, and send them together
        for (int j = i + 1; j < numSides; j++)
        {
            if (!hasBeenSent[j] && weaponData[sides[i]] == weaponData[sides[j]])
            {
                sidesToSend |= 1 << sides[j];
                hasBeenSent[j] = true;
            }
        }

        // send the data
        IRSender::Send(EncodeData(sides[i]), sidesToSend);
        hasBeenSent[i] = true;
    }
}

/// @brief Encodes the data to be sent by the weapon(s)
/// @param side The side to send the data to (0-4)
/// @return The encoded data
uint8_t FireController::EncodeData(int side)
{
    if (isInGame)
    {
        // send the 5 bit car id and the 3 bit side
        return (carId << 3) | (uint8_t)side;
    }

    // send the 5 bit weapon data and the null side
    return (weaponData[side] << 3) | NULL_SIDE;
}
