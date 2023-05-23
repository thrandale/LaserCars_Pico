#include "IRSender.h"

int IRSender::burstSM = -1;
int IRSender::controlSM = -1;

/// @brief Initializes the IR sender on pins 10-14
void IRSender::Init()
{
    uint controlProgramOffset = -1;
    uint burstProgramOffset = -1;

    // install the carrier_burst program in the PIO shared instruction space
    if (pio_can_add_program(PIO_IR_SEND, &IRSender_burst_program))
    {
        burstProgramOffset = pio_add_program(PIO_IR_SEND, &IRSender_burst_program);
    }
    else
    {
        printf("Could not add burst program");
    }

    // claim an unused state machine on this PIO
    burstSM = pio_claim_unused_sm(PIO_IR_SEND, true);
    if (burstSM == -1)
    {
        printf("Could not claim unused SM for burst");
    }

    // configure and enable the state machine
    IRSender_burst_program_init(PIO_IR_SEND,
                                burstSM,
                                burstProgramOffset,
                                PIN_IR_SEND_START,
                                NUM_WEAPONS,
                                38.222e3, // 38.222 kHz carrier
                                16);

    // install the carrier_control program in the PIO shared instruction space
    if (pio_can_add_program(PIO_IR_SEND, &IRSender_control_program))
    {
        controlProgramOffset = pio_add_program(PIO_IR_SEND, &IRSender_control_program);
    }
    else
    {
        printf("Could not add control program");
    }

    // claim an unused state machine on this PIO
    controlSM = pio_claim_unused_sm(PIO_IR_SEND, true);
    if (controlSM == -1)
    {
        printf("Could not claim unused SM for control");
    }

    // configure and enable the state machine
    IRSender_control_program_init(PIO_IR_SEND,
                                  controlSM,
                                  controlProgramOffset,
                                  2 * (1 / 562.5e-6f), // 2 ticks per 562.5us carrier burst
                                  32);                 // 32 bits per frame
}

/// @brief Sends a frame
/// @param data The data to send (8 bits)
/// @param side The sides to send on (bitmask)
void IRSender::Send(uint8_t data, uint8_t sides)
{
    printf("Sending %d on side %d\n", data, sides);

    // set the pin directions
    uint16_t command = pio_encode_set(pio_pindirs, sides);
    pio_sm_put_blocking(PIO_IR_SEND, burstSM, command);

    //
    uint32_t frame = Encode(data);
    pio_sm_put_blocking(PIO_IR_SEND, controlSM, frame);
}

/// @brief Encodes a frame
/// @param data The data to encode (8 bits)
/// @return the encoded frame (data, data, inverted data, inverted data)
uint32_t IRSender::Encode(uint8_t data)
{
    return (data) | (data << 8) | ((data ^ 0xff) << 16) | ((data ^ 0xff) << 24);
}
