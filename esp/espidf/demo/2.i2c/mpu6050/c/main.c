#include <stdio.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_mpu6050.h"

void delay_ms(uint32_t n) { vTaskDelay(n / portTICK_PERIOD_MS); }

void app_main(void) {
    mpu6050 imu = {
        .i2c = 0,
        .dev = MPU6050_ADDRESS_AD0_LOW,
    };

    i2c_master_init(imu.i2c, 200000);
    delay_ms(100);
    mpu6050_init(&imu);
    while (1) {
        mpu6050_log(&imu, 16);
        delay_ms(200);
    }
    i2c_driver_delete(imu.i2c);
}
