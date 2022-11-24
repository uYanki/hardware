#include <stdio.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_mpu6050.hpp"

void delay_ms(uint32_t n) { vTaskDelay(n / portTICK_PERIOD_MS); }

// #define PIN_I2C_SDA 4
// #define PIN_I2C_SCL 5

void i2c_master_init(i2c_port_t i2c, int pin_sda, int pin_scl, uint32_t freq = 400000 /*400000*/) {
    i2c_config_t cfg;
    cfg.mode             = I2C_MODE_MASTER;
    cfg.sda_io_num       = pin_sda;
    cfg.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    cfg.scl_io_num       = pin_scl;
    cfg.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    cfg.master.clk_speed = freq;
    cfg.clk_flags        = 0;
    i2c_param_config(i2c, &cfg);
    i2c_driver_install(i2c, cfg.mode, 0, 0, 0);
}

void mpu6050_task(void* arg) {
    uint32_t lasttime      = 0;
    kalman*  kfilter_roll  = new kalman();
    kalman*  kfilter_pitch = new kalman();
    i2c_master_init(0, 4, 5);
    mpu6050 imu(0);
    delay_ms(100);
    while (1) {
        float pitch = kfilter_pitch->filter(imu.get_self_pitch_angle(), imu.get_gyro_y());
        float roll  = kfilter_roll->filter(imu.get_self_roll_angle(), -imu.get_gyro_x());
        if (esp_log_timestamp() / 100 != lasttime) {
            lasttime = esp_log_timestamp() / 100;
            printf("(kalman filter - self) pitch:%f, roll:%f\n", pitch, roll);
        }
    }
}

extern "C" void app_main(void) {
    mpu6050_task(nullptr);
    // xTaskCreatePinnedToCore(&mpu6050_task, "mpu6050_task", 2048 , NULL, 0, NULL, 0);
}
