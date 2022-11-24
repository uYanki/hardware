#ifndef _LCD_INIT_H_
#define _LCD_INIT_H_

#define LCD_DIRECTION 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if LCD_DIRECTION == 0 || LCD_DIRECTION == 1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif

#include <stdint.h>

#include "main.h"
#include "spi.h"

// LCD PORT

#define LCD_Delayms(ms) HAL_Delay(ms)

#define LCD_RES_Clr() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)  // RES
#define LCD_RES_Set() HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)

#define LCD_DC_Clr() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)  // DC
#define LCD_DC_Set() HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

#define LCD_CS_Clr() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)  // CS
#define LCD_CS_Set() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_BLK_Clr() HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET)  // BLK
#define LCD_BLK_Set() HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET)

void LCD_Init(void);

void LCD_WR_DATA8(uint8_t data);
void LCD_WR_DATA16(uint16_t data);
void LCD_WR_CMD(uint8_t data);

void LCD_SetAddrWnd(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif
