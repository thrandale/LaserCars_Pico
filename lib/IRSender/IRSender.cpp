#include "IRSender.h"

IRSender::IRSender(PIO pio, uint pin, uint numPins)
{
    this->pio = pio;
    this->controlProgramOffset = -1;
    this->burstProgramOffset = -1;
    this->burstSM = -1;
    this->controlSM = -1;

    // install the carrier_burst program in the PIO shared instruction space
    if (pio_can_add_program(pio, &IRSender_burst_program))
    {
        burstProgramOffset = pio_add_program(pio, &IRSender_burst_program);
    }
    else
    {
        printf("Could not add burst program");
    }

    // claim an unused state machine on this PIO
    burstSM = pio_claim_unused_sm(pio, true);
    if (burstSM == -1)
    {
        printf("Could not claim unused SM for burst");
    }

    // configure and enable the state machine
    IRSender_burst_program_init(pio,
                                burstSM,
                                burstProgramOffset,
                                pin,
                                numPins,
                                38.222e3, // 38.222 kHz carrier
                                16);

    // install the carrier_control program in the PIO shared instruction space
    if (pio_can_add_program(pio, &IRSender_control_program))
    {
        controlProgramOffset = pio_add_program(pio, &IRSender_control_program);
    }
    else
    {
        printf("Could not add control program");
    }

    // claim an unused state machine on this PIO
    controlSM = pio_claim_unused_sm(pio, true);
    if (controlSM == -1)
    {
        printf("Could not claim unused SM for control");
    }

    // configure and enable the state machine
    IRSender_control_program_init(pio,
                                  controlSM,
                                  controlProgramOffset,
                                  2 * (1 / 562.5e-6f), // 2 ticks per 562.5us carrier burst
                                  32);                 // 32 bits per frame
}

// Send a frame
// Data: the 8 bit message
// Pin: the binary mask of the pins to send on
void IRSender::Send(uint8_t data, uint pin)
{
    uint16_t command = 0xE080 | pin;
    pio_sm_put_blocking(pio, burstSM, command);
    uint32_t frame = Encode(data);
    pio_sm_put_blocking(pio, controlSM, frame);
}

// Encode a frame
// Data: the 8 bit message
// Returns: the 32 bit frame (data, data, inverted data, inverted data)
uint32_t IRSender::Encode(uint8_t data)
{
    return (data) | (data << 8) | ((data ^ 0xff) << 16) | ((data ^ 0xff) << 24);
}
