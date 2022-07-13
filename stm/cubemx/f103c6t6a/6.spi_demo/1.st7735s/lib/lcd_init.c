
#include "lcd_init.h"

void LCD_WR_DATA8(uint8_t data) {
    HAL_SPI_Transmit(&hspi1, &data, 1, 0xFF);
}

void LCD_WR_DATA16(uint16_t data) {
    LCD_WR_DATA8(data >> 8);
    LCD_WR_DATA8(data);
}

void LCD_WR_CMD(uint8_t data) {
    LCD_DC_Clr();  // write command mode
    HAL_SPI_Transmit(&hspi1, &data, 1, 0xFF);
    LCD_DC_Set();  // write data mode
}

void LCD_SetAddrWnd(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    (LCD_DIRECTION == 0 | LCD_DIRECTION == 1) ? (x += 24) : (y += 24);

    LCD_WR_CMD(0x2a);  // col addr set
    LCD_WR_DATA16(x);
    LCD_WR_DATA16(x + w - 1);

    LCD_WR_CMD(0x2b);  // row addr set
    LCD_WR_DATA16(y);
    LCD_WR_DATA16(y + h - 1);

    LCD_WR_CMD(0x2c);  // write to RAM
}

void LCD_Init(void) {
    LCD_RES_Clr();  // reset
    LCD_Delayms(100);
    LCD_RES_Set();
    LCD_Delayms(100);

    LCD_BLK_Clr();  // 开背光
    LCD_Delayms(100);

    LCD_WR_CMD(0x11);  // Sleep exit
    LCD_Delayms(120);  // Delay 120ms
    LCD_WR_CMD(0xB1);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_CMD(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_CMD(0xB3);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);

    LCD_WR_CMD(0xB4);  // Dot inversion
    LCD_WR_DATA8(0x03);

    LCD_WR_CMD(0xC0);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x04);

    LCD_WR_CMD(0xC1);
    LCD_WR_DATA8(0xC5);

    LCD_WR_CMD(0xC2);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x00);

    LCD_WR_CMD(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);

    LCD_WR_CMD(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);

    LCD_WR_CMD(0xC5);    // VCOM
    LCD_WR_DATA8(0x06);  // 1D  .06

    LCD_WR_CMD(0x36);  // MX, MY, RGB mode
    switch (LCD_DIRECTION) {
        case 0: LCD_WR_DATA8(0x08); break;
        case 1: LCD_WR_DATA8(0xC8); break;
        case 2: LCD_WR_DATA8(0x78); break;
        default: LCD_WR_DATA8(0xA8); break;
    }

    LCD_WR_CMD(0x3A);
    LCD_WR_DATA8(0x55);

    LCD_WR_CMD(0xE0);
    LCD_WR_DATA8(0x0b);
    LCD_WR_DATA8(0x17);
    LCD_WR_DATA8(0x0a);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1a);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x3c);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);

    LCD_WR_CMD(0xE1);
    LCD_WR_DATA8(0x0c);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1b);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x3E);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);

    LCD_Delayms(120);
    LCD_WR_CMD(0x29);  // Display on
}
