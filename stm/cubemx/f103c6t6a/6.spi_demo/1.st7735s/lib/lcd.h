#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>

// functions
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);                         // 画点
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);  // 填充矩形

// colors
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0

#endif
