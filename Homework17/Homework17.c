#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

#define AS5600_ADDR 0x36
#define AS5600_ANGLE  0x0F

uint16_t read_AS5600(void) {
    uint8_t reg = AS5600_ANGLE;
    uint8_t buf[2];

    i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true);
    int read_bytes = i2c_read_blocking(I2C_PORT, AS5600_ADDR, buf, 2, false);

    uint16_t raw =  ((uint16_t)buf[0] << 8) | buf[1];
    float deg = (raw / 4096.0f) * 360.0f;

    return (int)deg;
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    while (true) {
        uint16_t deg = read_AS5600();
        printf("%lu,%u\n", to_ms_since_boot(get_absolute_time()), deg);

        sleep_ms(100);   

    }
}
