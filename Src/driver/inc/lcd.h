#ifndef INC_LCD_H
#define INC_LCD_H

#include "stm32f1xx_hal.h"

#define LCD_RS GPIO_PIN_0
#define LCD_E GPIO_PIN_1
#define LCD_GPIO GPIOA
#define LCD_DATA4 GPIO_PIN_4
#define LCD_DATA5 GPIO_PIN_5
#define LCD_DATA6 GPIO_PIN_6
#define LCD_DATA7 GPIO_PIN_7
#define LCD_DATA_OFFSET 4

void LCD_Init(void);
void LCD_Command(uint8_t cmnd);
void LCD_Data(uint8_t data);
void LCD_String(char *str);
void LCD_String_xy(uint8_t row, uint8_t pos, char *str);
void LCD_Clear();
void LCD_sendbyte(uint8_t data);
void LCD_setCustomChar(uint8_t cgrampos, uint8_t *pixmap);
void LCD_Clear_xy(uint8_t row, uint8_t from, uint8_t len);

#endif