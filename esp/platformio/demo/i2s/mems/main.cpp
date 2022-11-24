#include <Arduino.h>
#include "lib_mems.h"

// i2s config for reading from left channel of I2S

// 目前PDM的支持情况是：
// ESP32：TX RX
// ESP32S2：不支持
// ESP32C3：TX
// ESP32S3：TX RX

i2s_config_t i2s_cfg = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 38400,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s pins
i2s_pin_config_t i2s_pins = {
    .bck_io_num = GPIO_NUM_NC,
    .ws_io_num = GPIO_NUM_32,  // pdm_sclk
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = GPIO_NUM_33  // pdm_din
};

// init sampler
MEMS sampler(I2S_NUM_0, i2s_pins, i2s_cfg);

// how many samples to read at once
// const int SAMPLE_SIZE = 16384;
const int SAMPLE_SIZE = 64;

// Task to write samples to our server
void i2s_mems_task(void* param) {
  MEMS* sampler = (MEMS*)param;
  int16_t* samples = (int16_t*)malloc(sizeof(uint16_t) * SAMPLE_SIZE);
  if (!samples) {
    Serial.println("Failed to allocate memory for samples");
    return;
  }
  while (true) {
    int samples_read = sampler->read(samples, SAMPLE_SIZE);
    String s = "";
    for (int i = 0; i < samples_read; ++i)
      s = s + samples[i] + ((i < samples_read - 1) ? ',' : ' ');
    Serial.println(s);
    delay(100);
  }
}

void setup() {
  TaskHandle_t hTask_MEMS;
  Serial.begin(115200);
  xTaskCreatePinnedToCore(i2s_mems_task, "I2S Task", 4096, &sampler, 1,
                          &hTask_MEMS, 1);
}

void loop() {}
