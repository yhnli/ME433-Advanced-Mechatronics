#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "font.h"

// i2c defines 
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

// display defines
#define CX  64   // screen center x
#define CY  16   // screen center y 
#define SCALE 13 

// draw line from (x0,y0) to (x1,y1)
void drawLine(int x0, int y0, int x1, int y1, unsigned char color) {
    int dx  =  abs(x1 - x0);
    int sx  = (x0 < x1) ? 1 : -1;
    int dy  = -abs(y1 - y0);
    int sy  = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
 
    while (1) {
        if (x0 >= 0 && x0 < 128 && y0 >= 0 && y0 < 32) {
            ssd1306_drawPixel((unsigned char)x0, (unsigned char)y0, color);
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

int main()
{
    stdio_init_all();

    // init i2c
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // mpu6050 initialization
    mpu6050_init();

    // display initialization
    ssd1306_setup();
    
    mpu6050_data_t data;
    char msg1[24], msg2[24];

    while (true) {
        // read mpu6050
        mpu6050_read(&data);
        printf("Accel (g):  X=%.3f  Y=%.3f  Z=%.3f\n", data.accel_x, data.accel_y, data.accel_z);
        printf("Gyro (d/s): X=%.3f  Y=%.3f  Z=%.3f\n", data.gyro_x,  data.gyro_y,  data.gyro_z);
        printf("Temp (C):   %.2f\n\n",                  data.temp);
        sleep_ms(100);
        
        // test display
        ssd1306_clear();
        drawChar(10, 10, 'A');
        ssd1306_update();
    }
}