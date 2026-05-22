#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
#define I2C_PORT     i2c0
#define I2C_SDA      16
#define I2C_SCL      17

// MPU6050 I2C address (AD0 pin low)
#define MPU6050_ADDR 0x68

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

// data structure
typedef struct {
    float accel_x, accel_y, accel_z;   // g
    float gyro_x,  gyro_y,  gyro_z;    // degrees/s
    float temp;                          // degrees C
} mpu6050_data_t;

// function prototypes
void mpu6050_write_reg(uint8_t reg, uint8_t value);
void mpu6050_init(void);
void mpu6050_read(mpu6050_data_t *out);

#endif // MPU6050_H