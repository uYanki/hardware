
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_i2s_mems.hpp"

MEMS sampler(I2S_NUM_0, 19, 1, 0);

#define SAMPLE_SIZE 64
int16_t* samples;

void delay_ms(uint32_t ms) { vTaskDelay(ms / portTICK_PERIOD_MS); }

void setup() {
    samples = (int16_t*)malloc(sizeof(uint16_t) * SAMPLE_SIZE);
}

void loop() {
    int samples_read = sampler.Read(samples, SAMPLE_SIZE);
    for (int i = 0; i < samples_read; ++i)
        printf("%d\r\n", samples[i]);
    delay_ms(200);
}

extern "C" void app_main(void) {
    setup();
    while (1) loop();
}
