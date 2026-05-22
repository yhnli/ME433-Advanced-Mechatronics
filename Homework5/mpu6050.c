#include "mpu6050.h"
#include "hardware/i2c.h"

// write to register in mpu6050
void mpu6050_write_reg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(IMU_PORT, MPU6050_ADDR, buf, 2, false);
}

// mpu6050 init function
void mpu6050_init(void) {
    // turn on chip
    mpu6050_write_reg(PWR_MGMT_1, 0x00); 
    // enable accelerometer; set to +-2g 
    mpu6050_write_reg(ACCEL_CONFIG, 0x00);
    // enable gyroscope; set to +- 2000 dps  
    mpu6050_write_reg(GYRO_CONFIG,  0x18); 
}

// read mpu6050 data and convert to physical units
void mpu6050_read(mpu6050_data_t *out) {
    uint8_t buffer[6];
    int16_t raw[3];

    // Read accelerometer (6 bytes from 0x3B)
    uint8_t val = ACCEL_XOUT_H;
    i2c_write_blocking(IMU_PORT, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(IMU_PORT, MPU6050_ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++)
        raw[i] = (int16_t)(buffer[i * 2] << 8 | buffer[i * 2 + 1]);
    // units g
    out->accel_x = raw[0] * 0.000061f;
    out->accel_y = raw[1] * 0.000061f;
    out->accel_z = raw[2] * 0.000061f;

    // Read gyroscope (6 bytes from 0x43)
    val = GYRO_XOUT_H;
    i2c_write_blocking(IMU_PORT, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(IMU_PORT, MPU6050_ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++)
        raw[i] = (int16_t)(buffer[i * 2] << 8 | buffer[i * 2 + 1]);
    // degrees per second
    out->gyro_x = raw[0] * 0.007630f;
    out->gyro_y = raw[1] * 0.007630f;
    out->gyro_z = raw[2] * 0.007630f;

    // Read temperature (2 bytes from 0x41)
    val = TEMP_OUT_H;
    i2c_write_blocking(IMU_PORT, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(IMU_PORT, MPU6050_ADDR, buffer, 2, false);
    int16_t raw_temp = (int16_t)(buffer[0] << 8 | buffer[1]);
    // degrees C
    out->temp = raw_temp / 340.0f + 36.53f;
}