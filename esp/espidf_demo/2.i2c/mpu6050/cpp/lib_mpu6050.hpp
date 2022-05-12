#pragma once

#include <math.h>
#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/task.h"

#define I2C_MASTER_TIMEOUT_MS 1000

#define MPU6050_ADDRESS_AD0_LOW 0x68
#define MPU6050_ADDRESS_AD0_HIGH 0x69
#define MPU6050_WHO_AM_I 0x75

#define MPU6050_PWr_MGMT_1 0x6B
#define MPU6050_PWr_MGMT_2 0x6C
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
#define MPU6050_USEr_CTRL 0x6A

#define RAD_TO_DEG 57.2958

class mpu6050 {
    i2c_port_t m_i2c;
    uint8_t    m_dev;  // 从机地址

    short read(uint8_t reg);
    void  write(uint8_t reg, uint8_t data);

public:
    mpu6050(uint8_t i2c, uint8_t dev = MPU6050_ADDRESS_AD0_LOW);

    float get_accel_x() { return read(MPU6050_ACCEL_XOUT) / MPU6050_ACCEL_SENSITIVE; }
    float get_accel_y() { return read(MPU6050_ACCEL_YOUT) / MPU6050_ACCEL_SENSITIVE; }
    float get_accel_z() { return read(MPU6050_ACCEL_ZOUT) / MPU6050_ACCEL_SENSITIVE; }
    float get_gyro_x() { return read(MPU6050_GYRO_XOUT) / MPU6050_GYRO_SENSITIVE; }
    float get_gyro_y() { return read(MPU6050_GYRO_YOUT) / MPU6050_GYRO_SENSITIVE; }
    float get_gyro_z() { return read(MPU6050_GYRO_ZOUT) / MPU6050_GYRO_SENSITIVE; }
    float get_temp() { return read(MPU6050_TEMP_OUT) / MPU6050_TEMP_SENSITIVE + MPU6050_TEMP_OFFSET; }

    // relative tp self
    float get_self_pitch_angle() { return atan2(get_accel_x(), get_accel_z()) * RAD_TO_DEG; }
    float get_self_roll_angle() { return atan2(get_accel_y(), get_accel_z()) * RAD_TO_DEG; }

    // relative tp world
    float get_wrold_pitch_angle() { return atan2(get_accel_x(), get_accel_z()) * RAD_TO_DEG; }
    float get_wrold_roll_angle() {
        float ax = get_accel_x();
        float ay = get_accel_y();
        float az = get_accel_z();
        return atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    }

    void log(uint8_t flag = 8 /* accel=1 gyro=2 temp=4 angle(self)=8 angle(wrold)=16 all=31 */) {
        if (flag == 0) return;
        if (flag & 1) printf("ax:%f,ay:%f,az:%f\n", get_accel_x(), get_accel_y(), get_accel_z());
        if (flag & 2) printf("gx:%f,gy:%f,gz:%f\n", get_gyro_x(), get_gyro_y(), get_gyro_z());
        if (flag & 4) printf("temp:%f\n", get_temp());
        if (flag & 8) printf("(self) pitch:%f,roll:%f\n", get_self_pitch_angle(), get_self_roll_angle());
        if (flag & 16) printf("(world) pitch:%f,roll:%f\n", get_wrold_pitch_angle(), get_wrold_roll_angle());
    }
};

mpu6050::mpu6050(uint8_t i2c, uint8_t dev) : m_i2c(i2c), m_dev(dev) {
    // set basic params
    write(MPU6050_PWr_MGMT_1, 0x80);       // reset
    vTaskDelay(100 / portTICK_PERIOD_MS);  // delay 100ms
    write(MPU6050_PWr_MGMT_1, 0x00);       // wake up
    write(MPU6050_SMPLRT_DIV, 0x07);       // sampling frequency, fs = 125Hz
    write(MPU6050_CONFIG, 0x07);           // LPF = 5Hz
    write(MPU6050_GYRO_CONFIG, 0x18);      // gyro 16.4 LSB/(deg/s) -> 2000deg/s
    write(MPU6050_ACCEL_CONFIG, 0x01);     // accel 16384 LSB/g -> 2g
    // set other params
    write(MPU6050_FIFO_EN, 0x00);     // disable fifo
    write(MPU6050_INT_ENABLE, 0x00);  // disable interrupt
    write(MPU6050_USEr_CTRL, 0x00);   // disable i2c master
    write(MPU6050_PWr_MGMT_2, 0x00);  // enable accel & gyro
}

short mpu6050::read(uint8_t reg) {
    i2c_cmd_handle_t cmd;

    uint8_t buf[2];

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_dev << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(m_i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_dev << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &buf[0], I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &buf[1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(m_i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return (buf[0] << 8) | buf[1];
}

void mpu6050::write(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd;
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (m_dev << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(m_i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

struct kalman {
public:
    float q_angle   = 0.1;    // 加速计的过程噪声协方差
    float q_bias    = 0.003;  // 陀螺仪偏差的过程噪声协方差
    float r_measure = 0.03;   // 测量噪声协方差
private:
    float angle;     // 卡尔曼滤波器计算的角度
    float bias;      // 卡尔曼滤波器计算的偏差
    float pp[2][2];  // 误差协方差矩阵
public:
    float filter(float new_angle, float gyro, float dt = 0.005) {
        angle += dt * (gyro - bias);

        pp[0][0] += dt * (q_angle - pp[0][1] - pp[1][0] + dt * pp[1][1]);
        pp[0][1] -= dt * pp[1][1];
        pp[1][0] -= dt * pp[1][1];
        pp[1][1] += q_bias * dt;

        float s = pp[0][0] + r_measure;
        float k_0, k_1;
        k_0 = pp[0][0] / s;
        k_1 = pp[1][0] / s;

        float t_0 = pp[0][0];
        float t_1 = pp[0][1];

        pp[0][0] -= k_0 * t_0;
        pp[0][1] -= k_0 * t_1;
        pp[1][0] -= k_1 * t_0;
        pp[1][1] -= k_1 * t_1;

        float angle_err = new_angle - angle;
        angle += k_0 * angle_err;
        bias += k_1 * angle_err;

        return angle;
    }
};
