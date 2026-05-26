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
#define CENTER_X  64   // screen center x
#define CENTER_Y  16   // screen center y 
#define SCALE 1000 

// draw line from (x0,y0) to (x1,y1)
void draw_line(int x0, int y0, int x1, int y1) {
    int dx =  abs(x1-x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        ssd1306_drawPixel(x0, y0, 1);
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
    ssd1306_clear();
    ssd1306_update();
    
    // variables init
    float accel[3];
    float gyro[3];
    float temp;

    while (true) {
        // read mpu6050
        mpu6050_read(accel, gyro, &temp);
        float accel_x = -accel[0];
        float accel_y = accel[1];
        // // test to see if work in serial monitor
        // printf("Accel (g):  X=%.3f  Y=%.3f  Z=%.3f\n", accel[0], accel[1], accel[2]);
        // printf("Gyro (d/s): X=%.3f  Y=%.3f  Z=%.3f\n", gyro[0],  gyro[1],  gyro[2]);
        // printf("Temp (C): %.2f\n\n", temp);
        // sleep_ms(100);
        
        // // test display
        // ssd1306_clear();
        // drawChar(CENTER_X, CENTER_Y, 'A');
        // ssd1306_update();

        // scale to screen size
        int x = (int)(accel_x * SCALE) + CENTER_X;
        int y = (int)(accel_y * SCALE) + CENTER_Y;

        // lock to screen size
        if (x < 0) x = 0;
        if (x > 127) x = 127;
        if (y < 0) y = 0;
        if (y > 31) y = 31;

        // clear display and draw line
        ssd1306_clear();
        draw_line(CENTER_X, CENTER_Y, x, y);
        ssd1306_update();
    }
}