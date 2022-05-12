#include <math.h>
#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/task.h"

#define I2C_MASTER_TIMEOUT_MS 1000
#define PIN_I2C_SDA 4
#define PIN_I2C_SCL 5
void i2c_master_init(i2c_port_t i2c, uint32_t freq);

typedef struct mpu6050 {
    i2c_port_t i2c;
    uint8_t    dev;  // 设备地址
} mpu6050;

#define MPU6050_ADDRESS_AD0_LOW 0x68
#define MPU6050_ADDRESS_AD0_HIGH 0x69
#define MPU6050_WHO_AM_I 0x75

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_PWR_MGMT_2 0x6C
#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_ACCEL_FULLSCALE 2
#define MPU6050_GYRO_FULLSCALE 250

#if 1
// ACCEL_SENSITIVE {16384, 8192, 4096, 2048} -- +/- 2,4,8,16 g
#if MPU6050_ACCEL_FULLSCALE == 2
#define MPU6050_ACCEL_SENSITIVE (float)(16384)
#elif MPU6050_ACCEL_FULLSCALE == 4
#define MPU6050_ACCEL_SENSITIVE (float)(8192)
#elif MPU6050_ACCEL_FULLSCALE == 8
#define MPU6050_ACCEL_SENSITIVE (float)(4096)
#elif MPU6050_ACCEL_FULLSCALE == 16
#define MPU6050_ACCEL_SENSITIVE (float)(2048)
#endif
// GYRO_SENSITIVE {131.0, 65.5, 32.8, 16.4} -- +/- 250,500,1000,2000 degrees/sec
#if MPU6050_GYRO_FULLSCALE == 250
#define MPU6050_GYRO_SENSITIVE (float)(131.0)
#elif MPU6050_GYRO_FULLSCALE == 500
#define MPU6050_GYRO_SENSITIVE (float)(65.5)
#elif MPU6050_GYRO_FULLSCALE == 1000
#define MPU6050_GYRO_SENSITIVE (float)(32.8)
#elif MPU6050_GYRO_FULLSCALE == 2000
#define MPU6050_GYRO_SENSITIVE (float)(16.4)
#endif
#else
#define MPU6050_ACCEL_SENSITIVE 1
#define MPU6050_GYRO_SENSITIVE 1
#endif

#define MPU6050_TEMP_SENSITIVE (float)340
#define MPU6050_TEMP_OFFSET (float)36.5

#define MPU6050_ACCEL_XOUT 0x3B
#define MPU6050_ACCEL_YOUT 0x3D
#define MPU6050_ACCEL_ZOUT 0x3F
#define MPU6050_TEMP_OUT 0x41
#define MPU6050_GYRO_XOUT 0x43
#define MPU6050_GYRO_YOUT 0x45
#define MPU6050_GYRO_ZOUT 0x47

#define MPU6050_FIFO_EN 0x23
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_USER_CTRL 0x6A

void mpu6050_init(mpu6050* mpu);

short mpu6050_read(mpu6050* mpu, uint8_t reg);
void  mpu6050_write(mpu6050* mpu, uint8_t reg, uint8_t data);

float mpu6050_get_accel_x(mpu6050* mpu);
float mpu6050_get_accel_y(mpu6050* mpu);
float mpu6050_get_accel_z(mpu6050* mpu);
float mpu6050_get_gyro_x(mpu6050* mpu);
float mpu6050_get_gyro_y(mpu6050* mpu);
float mpu6050_get_gyro_z(mpu6050* mpu);
float mpu6050_get_temp(mpu6050* mpu);

#define RAD_TO_DEG 57.2958

// relative tp self
float mpu6050_get_self_pitch_angle(mpu6050* mpu);
float mpu6050_get_self_roll_angle(mpu6050* mpu);
// relative tp world
float mpu6050_get_wrold_pitch_angle(mpu6050* mpu);
float mpu6050_get_wrold_roll_angle(mpu6050* mpu);

void mpu6050_log(mpu6050* mpu, uint8_t flag /* accel=1 gyro=2 temp=4 angle(self)=8 angle(wrold)=16 all=31 */);
