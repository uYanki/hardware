
#include "lib_i2c.h"
#include "lib_uart.h"

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void i2c_scan() {
    uint8_t i;
    for (i = 0; i < 0x80; i++) {
        if (i2c_check(i)) {
            uart_sendhex(i);
            uart_sendline();
            delay_ms(3000);
        }
        delay_ms(1);
    }
}

void main() {
    uart_init();
    i2c_scan();
    while (1) {}
}
