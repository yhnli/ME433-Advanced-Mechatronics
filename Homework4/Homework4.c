#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

// PICO defines
#define HEARTBEAT_FREQ 250

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

int main()
{
    stdio_init_all();

    // pico led init
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // adc init
    adc_init();
    adc_gpio_init(26);  
    adc_select_input(0);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // init display
    ssd1306_setup();

    while (true) {
        // blink pico led
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(HEARTBEAT_FREQ);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(HEARTBEAT_FREQ);

        // blink display pixel (test)
        // ssd1306_drawPixel(1, 1, 1);
        // ssd1306_update();
        // sleep_ms(HEARTBEAT_FREQ);
        // ssd1306_drawPixel(1, 1, 0);
        // ssd1306_update();
        // sleep_ms(HEARTBEAT_FREQ);

        // drawChar (test)
        // ssd1306_clear();
        // drawChar(10, 10, 'A');
        // ssd1306_update();

        // drawString (test)
        int i = 15;
        char message[50]; 
        sprintf(message, "my var = %d", i); 

        ssd1306_clear();
        drawString(20,10,message); 
        ssd1306_update();
    }
}
