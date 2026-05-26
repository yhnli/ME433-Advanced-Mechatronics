#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
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

    // I2C Initialisation
    i2c_init(I2C_PORT, 1700*1000);
    
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
        ssd1306_drawPixel(1, 1, 1);
        ssd1306_update();
        sleep_ms(HEARTBEAT_FREQ);
        ssd1306_drawPixel(1, 1, 0);
        ssd1306_update();
        sleep_ms(HEARTBEAT_FREQ);

        // drawChar (test)
        // ssd1306_clear();
        // drawChar(10, 10, 'A');
        // ssd1306_update();

        // drawString (test)
        // int i = 15;
        // char message[50]; 
        // sprintf(message, "my var = %d", i); 

        // ssd1306_clear();
        // drawString(20,10,message); 
        // ssd1306_update();

        // read ADC voltage + display voltage and FPS
        // variables init
        // char message[50];
        // uint64_t t_start, t_end;

        // // get start time for fps calc
        // t_start = to_us_since_boot(get_absolute_time());  

        // ssd1306_clear();

        // // read adc and convert to voltage
        // uint16_t adc_value = adc_read();
        // float voltage = adc_value * (3.3f / 4095.0f);
        // // print voltage to first line of display
        // sprintf(message, "ADC: %d", adc_value);
        // drawString(0,0,message);

        // ssd1306_update();


        // // get end time for FPS calc
        // t_end = to_us_since_boot(get_absolute_time());

        // // calc fps and reset
        // float fps = 1000000.0f / (t_end - t_start);
        
        // // print fps to second line of display
        // sprintf(message, "FPS: %.2f", fps);
        // drawString(0,24,message);
        // ssd1306_update();
    }
}
