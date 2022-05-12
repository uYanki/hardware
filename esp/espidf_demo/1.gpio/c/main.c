#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED1 GPIO_NUM_12
#define LED2 GPIO_NUM_13
#define KEY GPIO_NUM_9

void delay_ms(uint32_t n) { vTaskDelay(n / portTICK_PERIOD_MS); }

/* 现象:灯1闪烁,按键后灯2状态切换 */

uint8_t led_state = 0;

static void IRAM_ATTR gpio_isr_handle(void* arg) {
    /* 中断函数里不能用 printf */
    led_state = (led_state + 1) % 2;
    gpio_set_level(LED2, led_state);
}

void app_main(void) {
    printf("hello world");

    /* led config */
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);

    /* key config */
    gpio_config_t key_iocfg = {
        .pin_bit_mask = 1 << KEY,
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .intr_type    = GPIO_INTR_NEGEDGE,  // 下降沿中断
    };
    gpio_config(&key_iocfg);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(KEY, gpio_isr_handle, 0);

    /* main loop */
    while (1) {
        gpio_set_level(LED1, 0);
        delay_ms(1000);
        gpio_set_level(LED1, 1);
        delay_ms(1000);
    }
}
