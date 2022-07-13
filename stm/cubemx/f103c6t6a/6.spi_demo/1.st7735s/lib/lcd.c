#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"

void LCD_FillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color) {
    uint16_t i, j;
    LCD_SetAddrWnd(x1, y1, w, h);
    for (i = 0; i < w; ++i)
        for (j = 0; j < h; ++j)
            LCD_WR_DATA16(color);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
    LCD_SetAddrWnd(x, y, 1, 1);
    LCD_WR_DATA16(color);
}
