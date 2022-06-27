#include <stdio.h>

#include "driver/gpio.h"
#include "driver/i2c.h"

extern "C" void app_main(void) {
    i2c_master_init(0, 7, 6);

    i2c_config_t cfg;
    cfg.mode             = I2C_MODE_MASTER;
    cfg.sda_io_num       = 7;
    cfg.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    cfg.scl_io_num       = 6;
    cfg.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    cfg.master.clk_speed = 400000;
    cfg.clk_flags        = 0;
    i2c_param_config(i2c, &cfg);
    i2c_driver_install(i2c, cfg.mode, 0, 0, 0);

    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16) {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            fflush(stdout);
            address              = i + j;
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, 0x1);
            i2c_master_stop(cmd);
            esp_err_t ret = i2c_master_cmd_begin(0, cmd, 25 / portTICK_RATE_MS);
            i2c_cmd_link_delete(cmd);
            if (ret == ESP_OK) {
                printf("%02x ", address);
            } else if (ret == ESP_ERR_TIMEOUT) {
                printf("UU ");
            } else {
                printf("-- ");
            }
        }
        printf("\r\n");
    }
}
