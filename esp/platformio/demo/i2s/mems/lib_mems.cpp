#include "lib_mems.h"
MEMS::MEMS(i2s_port_t port, const i2s_pin_config_t& pins, const i2s_config_t& cfg) {
    m_i2s_port = port;
    i2s_driver_install(port, &cfg, 0, NULL);
    i2s_set_pin(port, &pins);
}

MEMS::~MEMS() {
    i2s_driver_uninstall(m_i2s_port);
}

#define MIN(a, b) a < b ? a : b

int MEMS::read(int16_t* samples, int count) {
    int32_t raw_samples[256];
    int     sample_index = 0;
    while (count > 0) {
        size_t bytes_read = 0;
        i2s_read(m_i2s_port, (void**)raw_samples, sizeof(int32_t) * MIN(count, 256), &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        for (int i = 0; i < samples_read; i++) {
            samples[sample_index] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
            sample_index++;
            count--;
        }
    }
    return sample_index;
}
