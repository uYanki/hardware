#pragma once

#include "driver/i2s.h"

class MEMS {
private:
    i2s_port_t m_i2s_port;

public:
    MEMS(i2s_port_t port, int pin_sdin, int pin_sck, int pin_ws, uint32_t freq = 38400 /* sample_rate */);
    MEMS(i2s_port_t port, const i2s_pin_config_t& pins, const i2s_config_t& cfg);
    ~MEMS() { i2s_driver_uninstall(m_i2s_port); }

public:
    uint32_t Read(int16_t* samples, size_t len);
};

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

MEMS::MEMS(i2s_port_t port, const i2s_pin_config_t& pins, const i2s_config_t& cfg) {
    m_i2s_port = port;
    ESP_ERROR_CHECK(i2s_driver_install(port, &cfg, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_pin(port, &pins));
    ESP_ERROR_CHECK(i2s_set_clk(port, cfg.sample_rate, cfg.bits_per_sample, I2S_CHANNEL_MONO));
}

MEMS::MEMS(i2s_port_t port, int pin_sdin, int pin_sck, int pin_ws, uint32_t freq) {
    m_i2s_port = port;

    // i2s config for reading from left channel of I2S
    i2s_config_t cfg = {
        .mode                 = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate          = freq,
        .bits_per_sample      = I2S_BITS_PER_SAMPLE_32BIT,  // 32bit
        .channel_format       = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count        = 4,
        .dma_buf_len          = 1024,
        .use_apll             = false,
        .tx_desc_auto_clear   = false,
        .fixed_mclk           = 0,
    };

    // i2s pins
    i2s_pin_config_t pins = {
        .mck_io_num   = I2S_PIN_NO_CHANGE,
        .bck_io_num   = pin_sck,
        .ws_io_num    = pin_ws,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num  = pin_sdin,
    };

    // init
    ESP_ERROR_CHECK(i2s_driver_install(port, &cfg, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_pin(port, &pins));
    ESP_ERROR_CHECK(i2s_set_clk(port, freq, cfg.bits_per_sample, I2S_CHANNEL_MONO));
}

uint32_t MEMS::Read(int16_t* samples /* buffer */, size_t len) {
    int32_t raw_samples[256];
    size_t  sample_index = 0;
    size_t  bytes_read   = 0;
    while (len) {
        i2s_read(m_i2s_port, (void*)raw_samples, sizeof(int32_t) * MIN(len, 256), &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        for (size_t i = 0; i < samples_read; ++i)
            samples[sample_index++] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
        if (len < samples_read) break;
        len -= samples_read;
    }
    return sample_index - 1;
}
