#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_FREQ 100*1000 // using at 100 kHz

// IO expander defines
#define MCP23008_ADDR 0b0100000
#define IODIR 0b00000000  
#define GPIO  0b00001001  
#define OLAT  0b00001010
#define MCP_BTN_PIN  0   
#define MCP_LED_PIN  7   

// pico led defines
#define HEARTBEAT_FREQ 250

// general purpose read/write functions
// send data (write one byte to a register)
void setPin(unsigned char add, unsigned char reg, unsigned char val) {
    // add = 7 bit address of chip
    // buf = 8 bit array of data that are sending
        // first byte is the value of the register you are trying to change
        // second byte is the value to want to write to that register
    // reg = register
    // val = value to write to register
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(I2C_PORT, add, buf, 2, false);

}
// read data (read one byte from a register)
uint8_t readPin(unsigned char add, unsigned char reg) {
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

    // chip init
    // set GP7 to output
    // set GP0 and other pins to input
    setPin(MCP23008_ADDR, IODIR, 0b01111111);

    // LED starts off
    setPin(MCP23008_ADDR, OLAT, 0x00);

    while (true) {
        // // blink pico led
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(HEARTBEAT_FREQ);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(HEARTBEAT_FREQ);

        // blink LED on GP7 (test)
        // setPin(MCP23008_ADDR, OLAT,  (1 << MCP_LED_PIN));
        // sleep_ms(HEARTBEAT_FREQ);
        // setPin(MCP23008_ADDR, OLAT, 0x00);               
        // sleep_ms(HEARTBEAT_FREQ);

        // incorporate led button press
        uint8_t btn_state = readPin(MCP23008_ADDR, GPIO);
        if (!(btn_state & (1 << MCP_BTN_PIN))) { 
            // button pressed (pin LOW)
            setPin(MCP23008_ADDR, OLAT, (1 << MCP_LED_PIN));
        } 
        else {
            // button not pressed (pin HIGH)
            setPin(MCP23008_ADDR, OLAT, 0x00);
        }   
    }   
}

