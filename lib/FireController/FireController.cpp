#include "FireController.h"

std::array<uint8_t, NUM_WEAPONS> FireController::weaponData;
bool FireController::isInGame;
uint8_t FireController::carId;

/// @brief Initializes the FireController
void FireController::Init()
{
    // initialize the IR sender
    IRSender::Init();

    // initialize the weapon data
    weaponData.fill(0);

    carId = 1;
    isInGame = true;
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
    std::array<bool, NUM_WEAPONS> hasBeenSent;
    hasBeenSent.fill(false);

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

/// @brief Handles the data received from the BT module and fires the weapon(s)
/// @param data The data received (as a 5 bit bit mask)
void FireController::HandleBTData(std::string data)
{
    printf("Fire data: %s\n", data.c_str());
    // decode the data
    uint8_t sides = std::stoi(data);
    printf("Sides: %d\n", sides);

    // fire on the sides
    int sidesToFire[NUM_WEAPONS];
    int numSides = 0;
    for (int i = 0; i < NUM_WEAPONS; i++)
    {
        if (sides & (1 << i))
        {
            sidesToFire[numSides++] = i;
        }
    }
    printf("Sides to fire: ");
    for (int i = 0; i < numSides; i++)
    {
        printf("%d ", sidesToFire[i]);
    }

    Fire(sidesToFire, numSides);
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
