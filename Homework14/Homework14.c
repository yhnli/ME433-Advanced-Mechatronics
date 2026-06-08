#include <stdio.h>
#include "pico/stdlib.h"
#include "hx711.h"

// Write a function to initialize 2 GPIO pins for SCK (output) and DT (input).
// Write a function to read out the 24 bits from the sensor, waiting until the DT pin is low, then blinking SCK 25 times, reading DT the first 24 times, and shifting the data and converting to a signed int.
int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
