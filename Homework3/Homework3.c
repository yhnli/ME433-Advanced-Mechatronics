#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define I2C_FREQ 100*1000 // using at 100 kHz

// IO expander defines
#define MCP23008_ADDR 0x20
#define MCP_BTN_PIN  0   
#define MCP_LED_PIN  7   

// pico led defines
#define HEARTBEAT_FREQ 250

// general purpose read/write functions
// send data (write one byte to a register)
void setPin(unsigned char add, unsigned char reg, unsigned char val) {
    // buf = 8 bit array of data that are sending
        // first byte is the value of the register you are trying to change
        // second byte is the value to want to write to that register
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(I2C_PORT, add, buf, 2, false);

}
// read data (read one byte from a register)
unsigned char readPin(unsigned char add, unsigned char reg) {
    // reg is the 8bit register you want to read from
    // buf is the 8bit number you are getting from the chip
    uint8_t buf[1] = {reg};
    i2c_write_blocking(I2C_PORT, add, buf, 1, true);   
    i2c_read_blocking (I2C_PORT, add, buf, 1, false);
    return buf[0];

}

int main()
{
    stdio_init_all();

    // pico led init
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // i2c init
    i2c_init(I2C_PORT, I2C_FREQ);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // chip init
    // set GP7 to output
    // set GP0 and other pins to input

    while (true) {
        // blink pico led
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(HEARTBEAT_FREQ);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(HEARTBEAT_FREQ);


    }
}
