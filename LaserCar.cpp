#include "pico/stdlib.h"
#include <stdio.h>

int main()
{
    uint led_pin = 25;

    stdio_init_all();

    // Initialize the LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    printf("Hello, world!\n\n");

    while (true)
    {
        // Turn on the LED
        printf("Blinking...\n");
        gpio_put(led_pin, true);
        sleep_ms(500);

        // Turn off the LED
        gpio_put(led_pin, false);
        sleep_ms(500);
    }
}