#ifndef CONSTANTS_H
#define CONSTANTS_H

// Pins
// clang-format off
#define PIN_NEOPIXELS 1
#define PIN_UNUSED_1 1
#define PIN_MOTOR_0_0 2
#define PIN_MOTOR_0_1 3
#define PIN_MOTOR_1_0 4
#define PIN_MOTOR_1_1 5
#define PIN_MOTOR_2_0 22
#define PIN_MOTOR_2_1 26
#define PIN_MOTOR_3_0 27
#define PIN_MOTOR_3_1 28
#define PIN_IR_SEND_START 6
#define PIN_MPLEX_ADDRESSES {11, 12, 13}
#define PIN_IR_REC_START 18
#define PIN_LONE_WEAPON_DATA 17
#define PIN_MPLEX_DATA {14, 15, 16}

// PIO
#define PIO_IR_REC pio0
#define PIO_IR_SEND pio1

// others
#define NUM_WEAPONS 5
#define NUM_RECEIVERS 4


#define MAX_PLAYERS 32
#define NULL_SIDE 0b111


#endif // CONSTANTS_H