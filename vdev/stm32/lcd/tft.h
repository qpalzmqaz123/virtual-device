#ifndef __TFT_H
#define __TFT_H

#include "stm32f4xx.h"

/**
 * config
 */
#define __LCD_DEV_SSD1963 0
#define __LCD_DEV_ILI9486 1
#define __LCD_DEVICE __LCD_DEV_ILI9486 /* support SSD1963, ILI9486 */
#define __USE_IO_SIMULATE 1
#define __LCD_X_MAX 320
#define __LCD_Y_MAX 480


#if __USE_IO_SIMULATE == 0

#define LCD_REG              (*((volatile unsigned short *) 0x60000000))
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000))

#define LCD_LED_H() {GPIOA->ODR |= 0x0001;}
#define LCD_LED_L() {GPIOA->ODR &= ~0x0001;}

#else

#define LCD_CS_H()  {GPIOD->BSRRL = 0x0001;}
#define LCD_CS_L()  {GPIOD->BSRRH = 0x0001;}
#define LCD_RS_H()  {GPIOD->BSRRL = 0x0002;}
#define LCD_RS_L()  {GPIOD->BSRRH = 0x0002;}
#define LCD_WR_H()  {GPIOD->BSRRL = 0x0004;}
//#define LCD_WR_L()  {GPIOD->BSRRH = 0x0004;}
#define LCD_WR_L()  {GPIOD->BSRRH = 0x0004; __NOP();}
#define LCD_RD_H()  {GPIOD->BSRRL = 0x0008;}
#define LCD_RD_L()  {GPIOD->BSRRH = 0x0008; __NOP(); __NOP();}
#define LCD_RST_H() {GPIOD->BSRRL = 0x0010;}
#define LCD_RST_L() {GPIOD->BSRRH = 0x0010;}

#define LCD_LED_H() {GPIOD->ODR |= 0x0020;}
#define LCD_LED_L() {GPIOD->ODR &= ~0x0020;}

#define LCD_DATA(x) {GPIOE->ODR = (x);}

#endif

/* LCD */
void Lcd_Init(void);
void Lcd_SetWindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void Lcd_Clear(uint16_t color);
void Lcd_WriteReg(uint16_t data);
void Lcd_WriteData(uint16_t data);
void Lcd_WriteRegData(uint16_t reg, uint16_t data);
void Lcd_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
uint16_t Lcd_ReadPoint(uint16_t x, uint16_t y);
void Lcd_SetBackLight(uint8_t); /* set lcd backlight [0 - 100]*/

/* Touch */
#define TOUCH_DOWN (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))

void Touch_Init(void);
uint8_t Touch_GetData(uint16_t *x, uint16_t *y);
uint8_t Touch_GetXY(uint16_t *x, uint16_t *y);
#endif
