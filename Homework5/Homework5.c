#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"

int main()
{
    stdio_init_all();

    // I2C Initialisation at 400 kHz
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // mpu6050 initialization
    mpu6050_init();
    
    mpu6050_data_t data;
    while (true) {
        mpu6050_read(&data);
        printf("Accel (g):  X=%.3f  Y=%.3f  Z=%.3f\n", data.accel_x, data.accel_y, data.accel_z);
        printf("Gyro (d/s): X=%.3f  Y=%.3f  Z=%.3f\n", data.gyro_x,  data.gyro_y,  data.gyro_z);
        printf("Temp (C):   %.2f\n\n",                  data.temp);
        sleep_ms(1000);
    }
}