#include <stdio.h>
#include "pico/stdlib.h"

#define BTN_PIN 16
#define DEBOUNCE_MS 50

int main()
{
    stdio_init_all();

    // init GPIO
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);

    printf("READY\n");
    fflush(stdout);

    bool last_state = true;  // unpressed = HIGH
    absolute_time_t last_event = get_absolute_time();
 
    while (true) {
        bool current_state = gpio_get(BTN_PIN);
 
        // Heartbeat every 2s so we know the loop is alive
        static uint32_t heartbeat = 0;
        if (heartbeat++ % 400 == 0) {
            printf("ALIVE btn=%d\n", current_state);
            fflush(stdout);
        }
 
        if (last_state && !current_state) {   // falling edge = press
            absolute_time_t now = get_absolute_time();
            int64_t elapsed_ms = absolute_time_diff_us(last_event, now) / 1000;
 
            if (elapsed_ms > DEBOUNCE_MS) {
                printf("FLAP\n");
                fflush(stdout);
                last_event = now;
            }
        }
 
        last_state = current_state;
        sleep_ms(10);
    }
}