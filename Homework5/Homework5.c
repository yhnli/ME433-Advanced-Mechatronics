#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "font.h"

// draw line on display
void drawLine(int x0, int y0, int x1, int y1) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        ssd1306_drawPixel((unsigned char)x0, (unsigned char)y0, 1);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// mark center of display
void drawCrosshair(int cx, int cy) {
    ssd1306_drawPixel(cx - 2, cy,     1);
    ssd1306_drawPixel(cx - 1, cy,     1);
    ssd1306_drawPixel(cx,     cy,     1);
    ssd1306_drawPixel(cx + 1, cy,     1);
    ssd1306_drawPixel(cx + 2, cy,     1);
    ssd1306_drawPixel(cx,     cy - 2, 1);
    ssd1306_drawPixel(cx,     cy - 1, 1);
    ssd1306_drawPixel(cx,     cy + 1, 1);
    ssd1306_drawPixel(cx,     cy + 2, 1);
}

int main()
{
    stdio_init_all();

    // imu i2c
    i2c_init(i2c1, 400*1000);
    gpio_set_function(IMU_SDA, GPIO_FUNC_I2C);
    gpio_set_function(IMU_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(IMU_SDA);
    gpio_pull_up(IMU_SCL);

    // display idc
    i2c_init(i2c0, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // mpu6050 initialization
    mpu6050_init();

    // display initialization
    ssd1306_setup();
    
    // display geometry constants
    const int cx = 64;
    const int cy = 16;
    const float scale = 14.0f;

    mpu6050_data_t data;

    while (true) {
        // read mpu6050
        mpu6050_read(&data);

    }
}