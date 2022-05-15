
#include <Arduino.h>
#include "driver/i2s.h"

class MEMS {
private:
    i2s_port_t m_i2s_port;

public:
    MEMS(i2s_port_t port, const i2s_pin_config_t& pins, const i2s_config_t& cfg);
    ~MEMS();

public:
    int read(int16_t* samples, int count);
};
