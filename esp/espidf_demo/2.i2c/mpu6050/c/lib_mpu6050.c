#include "lib_mpu6050.h"

void i2c_master_init(i2c_port_t i2c, uint32_t freq) {
    i2c_config_t cfg = {
        .mode             = I2C_MODE_MASTER,
        .sda_io_num       = PIN_I2C_SDA,
        .scl_io_num       = PIN_I2C_SCL,
        .sda_pullup_en    = GPIO_PULLUP_ENABLE,
        .scl_pullup_en    = GPIO_PULLUP_ENABLE,
        .master.clk_speed = freq,
    };
    cfg.clk_flags = 0;
    i2c_param_config(i2c, &cfg);
    i2c_driver_install(i2c, cfg.mode, 0, 0, 0);
}

short mpu6050_read(mpu6050* mpu, uint8_t reg) {
    i2c_cmd_handle_t cmd;

    uint8_t buf[2];

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (mpu->dev << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(mpu->i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (mpu->dev << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &buf[0], I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &buf[1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(mpu->i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return (buf[0] << 8) | buf[1];
}

void mpu6050_write(mpu6050* mpu, uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd;
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (mpu->dev << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(mpu->i2c, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void mpu6050_init(mpu6050* mpu) {
    // set basic params
    mpu6050_write(mpu, MPU6050_PWR_MGMT_1, 0x80);    // reset
    vTaskDelay(100 / portTICK_PERIOD_MS);            // delay 100ms
    mpu6050_write(mpu, MPU6050_PWR_MGMT_1, 0x00);    // wake up
    mpu6050_write(mpu, MPU6050_SMPLRT_DIV, 0x07);    // sampling frequency, fs = 125Hz
    mpu6050_write(mpu, MPU6050_CONFIG, 0x07);        // LPF = 5Hz
    mpu6050_write(mpu, MPU6050_GYRO_CONFIG, 0x18);   // gyro 16.4 LSB/(deg/s) -> 2000deg/s
    mpu6050_write(mpu, MPU6050_ACCEL_CONFIG, 0x01);  // accel 16384 LSB/g -> 2g
    // set other params
    mpu6050_write(mpu, MPU6050_FIFO_EN, 0x00);     // disable fifo
    mpu6050_write(mpu, MPU6050_INT_ENABLE, 0x00);  // disable interrupt
    mpu6050_write(mpu, MPU6050_USER_CTRL, 0x00);   // disable i2c master
    mpu6050_write(mpu, MPU6050_PWR_MGMT_2, 0x00);  // enable accel & gyro
}

float mpu6050_get_accel_x(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_ACCEL_XOUT) / MPU6050_ACCEL_SENSITIVE; }
float mpu6050_get_accel_y(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_ACCEL_YOUT) / MPU6050_ACCEL_SENSITIVE; }
float mpu6050_get_accel_z(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_ACCEL_ZOUT) / MPU6050_ACCEL_SENSITIVE; }

float mpu6050_get_gyro_x(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_GYRO_XOUT) / MPU6050_GYRO_SENSITIVE; }
float mpu6050_get_gyro_y(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_GYRO_YOUT) / MPU6050_GYRO_SENSITIVE; }
float mpu6050_get_gyro_z(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_GYRO_ZOUT) / MPU6050_GYRO_SENSITIVE; }

float mpu6050_get_temp(mpu6050* mpu) { return mpu6050_read(mpu, MPU6050_TEMP_OUT) / MPU6050_TEMP_SENSITIVE + MPU6050_TEMP_OFFSET; }

// 加速度计 Accelerometer、陀螺仪 Gyroscope、磁力计 Magnetometer
// 俯仰 pitch(绕y)、滚转 roll(绕x)、偏航 yaw(绕y)

float mpu6050_get_self_pitch_angle(mpu6050* mpu) { return atan2(mpu6050_get_accel_x(mpu), mpu6050_get_accel_z(mpu)) * RAD_TO_DEG; }
float mpu6050_get_self_roll_angle(mpu6050* mpu) { return atan2(mpu6050_get_accel_y(mpu), mpu6050_get_accel_z(mpu)) * RAD_TO_DEG; }

float mpu6050_get_wrold_roll_angle(mpu6050* mpu) {
    float ax = mpu6050_get_accel_x(mpu);
    float ay = mpu6050_get_accel_y(mpu);
    float az = mpu6050_get_accel_z(mpu);
    return atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
}
float mpu6050_get_wrold_pitch_angle(mpu6050* mpu) { return atan2(mpu6050_get_accel_x(mpu), mpu6050_get_accel_z(mpu)) * RAD_TO_DEG; }

void mpu6050_log(mpu6050* mpu, uint8_t flag) {
    if (flag == 0) return;
    if (flag & 1) printf("ax:%f,ay:%f,az:%f\n", mpu6050_get_accel_x(mpu), mpu6050_get_accel_y(mpu), mpu6050_get_accel_z(mpu));
    if (flag & 2) printf("gx:%f,gy:%f,gz:%f\n", mpu6050_get_gyro_x(mpu), mpu6050_get_gyro_y(mpu), mpu6050_get_gyro_z(mpu));
    if (flag & 4) printf("temp:%f\n", mpu6050_get_temp(mpu));
    if (flag & 8) printf("(self) pitch:%f,roll:%f\n", mpu6050_get_self_pitch_angle(mpu), mpu6050_get_self_roll_angle(mpu));
    if (flag & 16) printf("(world) pitch:%f,roll:%f\n", mpu6050_get_wrold_pitch_angle(mpu), mpu6050_get_wrold_roll_angle(mpu));
}
