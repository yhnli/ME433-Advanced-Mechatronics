#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "font.h"

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
    
    mpu6050_data_t data;

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