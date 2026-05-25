
#include <stdio.h>
#include "pico/stdlib.h"
#include "mpu6050.h"
#include "hardware/i2c.h"

// mpu6050 init function
void mpu6050_init(void) {
    uint8_t buf0[2]; 
    buf0[0] = PWR_MGMT_1;
    buf0[1] = 0x00;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf0, 2, false);

    uint8_t buf1[2]; 
    buf1[0] = GYRO_CONFIG;
    buf1[1] = 0x00;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf1, 2, false);

    uint8_t buf2[2]; 
    buf2[0] = ACCEL_CONFIG;
    buf2[1] = 0x00;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf2, 2, false);
}

// read mpu6050 data and convert to physical units
void mpu6050_read(float *accel, float *gyro, float *temp) {
    uint8_t buf[14]; 
    uint8_t reg = ACCEL_XOUT_H; 

    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);  
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buf, 14, false);

    // accel
    accel[0] = (int16_t)(buf[0]<<8|buf[1])*0.000061f;  // X
    accel[1] = (int16_t)(buf[2]<<8|buf[3])*0.000061f;  // Y
    accel[2] = (int16_t)(buf[4]  << 8 |buf[5])*0.000061f;  // Z

    temp[0] = (int16_t)(buf[6]<<8 |buf[7])/340+36.53f;

    gyro[0] = (int16_t)(buf[8]  << 8 | buf[9])*0.007630f;  // X
    gyro[1] = (int16_t)(buf[10] << 8 | buf[11])*0.007630f; // Y
    gyro[2] = (int16_t)(buf[12] << 8 | buf[13])*0.007630f; // Z
}