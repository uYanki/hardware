
#include <stdio.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_i2s_mems.hpp"

MEMS sampler(I2S_NUM_0, 5, 6, 7);

#define SAMPLE_SIZE 65
int16_t* samples;

void delay_ms(uint32_t ms) { vTaskDelay(ms / portTICK_PERIOD_MS); }

void setup() {
    samples = (int16_t*)malloc(sizeof(uint16_t) * SAMPLE_SIZE);
}

void loop() {
    std::string output = "";

    int samples_read = sampler.Read(samples, SAMPLE_SIZE);
    for (int i = 0; i < samples_read; ++i) {
        output += std::to_string(samples[i]);
        output += ',';
    }
    printf("%s\r\n", output.c_str());
    delay_ms(50);
}

extern "C" void app_main(void) {
    setup();
    while (1) loop();
}
