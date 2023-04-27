# Architecture Information

## Pins

- 0, 1 - Debug UART (and LEDS)
- 2-9 - Motors
- 10-14 -IR LEDs
- 18-21 - IR Receivers
- 15-17 - Multiplexer Address
- 26-28 - Multiplexer Data
- 22 - Lone Weapon Data Pin

## Weapon Data Format

8 bits

- 0-5 - Car
- 6-8 - Side

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
  - Round Length
    - Time limit
    - Last man/team standing

## Capture the Flag

- Requires beacons
- When tagged, you are disabled until you return to your base (maybe also frozen for a time)
- Settings
  - Number of teams
  - Disabled time
