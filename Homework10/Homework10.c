#include <stdio.h>
#include "pico/stdlib.h"

#define BTN_PIN 16

int main()
{
    stdio_init_all();

    // init GPIO
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);

    printf("READY\n");
 
    while (true) {
        int btn_state = gpio_get(BTN_PIN);

        printf("(%i)\n", btn_state);
        sleep_ms(10);
    }
}