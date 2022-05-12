#include "lib_xpt2046.h"

uint32_t XPT2046_Read(uint8_t cmd) {
    uint32_t ad = 0;
    SPI_CS      = 0;
    spi_send_byte(cmd);
    _nop_();
    _nop_();
    spi_recv_bits(&ad, 12);
    SPI_CS = 1;
    return ad;
}
