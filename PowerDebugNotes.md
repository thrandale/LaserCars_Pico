## Probably not related to interference from the battery

- Sending "Light Change" commands works over a long distance on battery power
- Sending "Move" commands without the motors connected works over a long distance
- The Pico does not appear to be losing power
  - Tested with a battery pack on the Pico, and the voltage with a multimeter

## Suspicions

- The Motors or the Motor wires are causing interference
- Too many bt commands
  - memory overflow?
- Unhandled exception causing a reset
- Handled exception not working as expected with the asynchronous bt causing a reset?

## Things to test

- Add Logs to pico
  - Startup Log
  - See if we can get a log where the disconnection event happens
- Test with motors at a distance
- Running motors at lower power
  - From a 6V battery pack
  - From the Pico

## Other notes

- Small pegs in the middle broke. Just remove them.
