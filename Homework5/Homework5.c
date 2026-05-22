#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17

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

// mpu6050 write reg
static void mpu6050_write_reg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
}

// mpu6050 init
void mpu6050_init(void) {
    // turn on chip
    mpu6050_write_reg(PWR_MGMT_1, 0x00); 
    // enable accelerometer; set to +-2g 
    mpu6050_write_reg(ACCEL_CONFIG, 0x00);
    // enable gyroscope; set to +- 2000 dps  
    mpu6050_write_reg(GYRO_CONFIG,  0x18); 
}

typedef struct {
    int16_t accel_x, accel_y, accel_z;
    int16_t temp;
    int16_t gyro_x,  gyro_y,  gyro_z;
} mpu6050_data_t;

void mpu6050_read(mpu6050_data_t *out) {
    uint8_t buf[14];
    uint8_t reg = ACCEL_XOUT_H;

    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);  // true = repeated start
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 14, false);

    // recombine into signed 16-bit integers
    out->accel_x = (int16_t)(buf[0]  << 8 | buf[1]);
    out->accel_y = (int16_t)(buf[2]  << 8 | buf[3]);
    out->accel_z = (int16_t)(buf[4]  << 8 | buf[5]);
    out->temp    = (int16_t)(buf[6]  << 8 | buf[7]);
    out->gyro_x  = (int16_t)(buf[8]  << 8 | buf[9]);
    out->gyro_y  = (int16_t)(buf[10] << 8 | buf[11]);
    out->gyro_z  = (int16_t)(buf[12] << 8 | buf[13]);

    // convert units
    //  out->accel_x = raw_accel_x * 0.000061f;   // g
    // out->accel_y = raw_accel_y * 0.000061f;
    // out->accel_z = raw_accel_z * 0.000061f;

    // out->gyro_x  = raw_gyro_x  * 0.007630f;   // degrees/s
    // out->gyro_y  = raw_gyro_y  * 0.007630f;
    // out->gyro_z  = raw_gyro_z  * 0.007630f;

    // out->temp    = raw_temp / 340.00f + 36.53f; // degrees C
}

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
    
    while (true) {
        mpu6050_data_t data;
        mpu6050_read(&data);
        printf("AX: %d  AY: %d  AZ: %d\n", data.accel_x, data.accel_y, data.accel_z);
        sleep_ms(1000);
    }
}