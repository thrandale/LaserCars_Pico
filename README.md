# Architecture Information

## Constants for later

PI - Drive, Animation
NUM_WEAPONS - FireController, WeaponController

## Pins

- 0, 1 - Debug UART (and LEDS)
- 2-5 - Motors 1
- 22, 26-28 - Motors 2
- 6-10 -IR LEDs
- 18-21 - IR Receivers
- 11-13 - Multiplexer Address
- 14-16 - Multiplexer Data
- 17 - Lone Weapon Data Pin

## Weapon Data Format

8 bits

- 0-5 - Car
- 6-8 - Side

## BLE

- Game Service - 00000000-6a5c-4ebb-8da6-a4471e0965ef
  - Game Start - 00000001-6a5c-4ebb-8da6-a4471e0965ef
  - Game End - 00000002-6a5c-4ebb-8da6-a4471e0965ef
  - You Died - 00000003-6a5c-4ebb-8da6-a4471e0965ef
- Driving Service - 10000000-6a5c-4ebb-8da6-a4471e0965ef
  - Driving - 10000001-6a5c-4ebb-8da6-a4471e0965ef
- Weapons Service - 20000000-6a5c-4ebb-8da6-a4471e0965ef
  - Weapon Info - 20000001-6a5c-4ebb-8da6-a4471e0965ef
  - Fire Weapon(s) - 20000002-6a5c-4ebb-8da6-a4471e0965ef
  - Hit - 20000003-6a5c-4ebb-8da6-a4471e0965ef
- Lights Service - 30000000-6a5c-4ebb-8da6-a4471e0965ef
  - Set Zone Color - 30000001-6a5c-4ebb-8da6-a4471e0965ef
  - Play Animation - 30000002-6a5c-4ebb-8da6-a4471e0965ef

# Game Modes

## Free for all

- Last Man standing wins

## Team Mode

- Last team standing wins
- Settings
  - Number of teams

## Capture the team

- Team with most players at the end of the game wins
- When a car is killed, it becomes the team of the killer
- Settings
  - Number of teams
  - Time limit
  - Disabled after team switch period

## Cars vs Zombies

- Starts with X zombies, If a Zombie kills another car, it infects it.
- Settings
  - Starting Zombies
  - Cars Win condition
    - Time limit
    - Kill all zombies
  - Starting Zombies Invulnerability period
  - New Zombie healable period

## Freeze Tag

- Taggers vs others
- Taggers can freeze others
- Taggers win if all others are frozen
- Others win if not all are frozen after time limit
- Settings
  - Time limit
  - Number of freezers

## Rougelike Mode

- You start with limited weapons
- After each round you get options for new weapons and/or upgrades
- Each Team/Car gets points based on criteria
- Settings
  - Free for all or team mode
  - Number of rounds
  - Round Lengths
    - Time limit
    - Last man/team standing

## Capture the Flag

- Requires beacons
- When tagged, you are disabled until you return to your base (maybe also frozen for a time)
- Settings
  - Number of teams
  - Disabled time

# Weapons

## Rifle

- Medium Damage
- Medium spread
- Medium rate of fire

## Shotgun

- Low Damage
- High spread
- Medium rate of fire

## Sniper

- Very High Damage
- Low spread
- Low rate of fire

## Machine gun

- Low Damage
- Medium spread
- High rate of fire

## Rocket Launcher

- High Damage
- High spread
- Very low rate of fire
- Ammo limited?

## Laser (Energy)

- Very Low Damage
- Low spread
- Very high rate of fire

## Plasma Cannon (Energy)

- Damage equal to charge time (with max)
- Medium spread

## EMP (Energy)

- Does no damage
- Medium spread
- Low rate of fire
- Invert controls for x seconds
- Disables all energy modules for x + y seconds

## Ice cannon

- Low damage
- Medium spread
- Low rate of fire
- Reduce speed by x% for x seconds
- Reduce the effectiveness of projectile modules

## Acid Cannon

- Applies x damage over time
- Medium spread
- Low rate of fire
- Does not get reflected
- Penetrates energy shields

# Shields

## Reflector (Energy)

- Reflect x% of damage back to the shooter

## Force Field (Energy)

- Absorbs all damage below x, until exhausted.
- If not hit for x seconds, it starts to recharge

## Shield

- Absorbs x% of damage

## Armor

- Increases max health by x

## TODO for model

- Fix/Add weapon offset
- Add connections between layers
- Support top weapon connection
- Test prints
  - bottom plate tolerance
    - clip
  - weapon connection tolerance
    - including top weapon
