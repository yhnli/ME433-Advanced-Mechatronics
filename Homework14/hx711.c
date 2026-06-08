#include "hx711.h"

// function to init hx711
void init_hx711(void) {
    // initialize 2 GPIO pins for SCK (output) and DT (input)
    gpio_init(DT);
    gpio_set_dir(DT, GPIO_IN);

    gpio_init(SCK);
    gpio_set_dir(SCK, GPIO_OUT);
    gpio_put(SCK, 0);
}

// function to read hx711
int read_hx711(void) { 
    while (gpio_get(DT)){
        tight_loop_contents();
    }

    unsigned int raw = 0;
    for (int i = 0; i < 24; i++) {
        // blink SCK 
        gpio_put(SCK, 1);
        sleep_us(clock_time_us);

        // read DT first 24 times and shift data
        raw = (raw << 1) | (gpio_get(DT) ? 1 : 0);

        gpio_put(SCK, 0);
        sleep_us(clock_time_us);
    }

    // 25th SCK pulse
    gpio_put(SCK, 1);
    sleep_us(clock_time_us);
    gpio_put(SCK, 0);
    sleep_us(clock_time_us);

    // convert to signed int
    if (raw & 0x800000) {
        raw |= 0xFF000000;
    }
    return (int)raw;
}