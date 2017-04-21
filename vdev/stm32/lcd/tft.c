#include "tft.h"

#if __LCD_DEVICE == __LCD_DEV_SSD1963
uint16_t  HDP=799;
uint16_t  HT=928;
uint16_t  HPS=46;
uint16_t  LPS=15;
uint8_t   HPW=48;

uint16_t  VDP=479;
uint16_t  VT=525;
uint16_t  VPS=16;
uint16_t  FPS=8;
uint8_t   VPW=16;
#endif

static void
Lcd_PinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if __USE_IO_SIMULATE == 0
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);

    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 |
                                  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                 GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#else
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_Init(GPIOE, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_4 | GPIO_Pin_5;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
}

#if __USE_IO_SIMULATE == 0
static void
Lcd_FSMCInit(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 16;
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 1;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 16;
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
#endif

void
Lcd_SetBackLight(uint8_t val)
{
    if (0 == val) {
        LCD_LED_L();
    }
    else {
        LCD_LED_H();
    }
}

void
Lcd_DelayMs(volatile int ms)
{
    volatile int i;

    while(ms--) {
        for(i = 0; i < 50000; i++){
        }
    }
}

void
Lcd_WriteReg(uint16_t data)
{
#if __USE_IO_SIMULATE == 0
    LCD_REG = data;
#else
    LCD_RS_L();

    LCD_CS_L();
    LCD_DATA(data);
    LCD_WR_L();
    LCD_WR_H();
#endif
}

void
Lcd_WriteData(uint16_t data)
{
#if __LCD_DEVICE == __LCD_DEV_ILI9486
    /* 8 bit mode */
#if __USE_IO_SIMULATE == 0
    LCD_RAM = data >> 8;
    LCD_RAM = data;
#else
    LCD_RS_H();

    LCD_DATA(data >> 8);
    LCD_WR_L();
    LCD_WR_H();

    LCD_DATA(data);
    LCD_WR_L();
    LCD_WR_H();
#endif

#else
    /* 16 bit mode */
#if __USE_IO_SIMULATE == 0
    LCD_RAM = data;
#else
    LCD_RS_H();

    LCD_DATA(data);
    LCD_WR_L();
    LCD_WR_H();
#endif

#endif
}

#if __LCD_DEVICE == __LCD_DEV_ILI9486
void
Lcd_WriteData8Bit(uint8_t data)
{
#if __USE_IO_SIMULATE == 0
    LCD_RAM = data;
#else
    LCD_RS_H();

    LCD_DATA(data);
    LCD_WR_L();
    LCD_WR_H();
#endif
}
#endif

uint16_t
Lcd_ReadPoint(uint16_t x, uint16_t y)
{
    volatile uint16_t tmp = 0;

#if __LCD_DEVICE == __LCD_DEV_ILI9486
	Lcd_WriteReg(0x2A);
	Lcd_WriteData8Bit(x>>8);
	Lcd_WriteData8Bit(0x00FF&x);
	Lcd_WriteData8Bit(x>>8);
	Lcd_WriteData8Bit(0x00FF&x);

	Lcd_WriteReg(0x2B);
	Lcd_WriteData8Bit(y>>8);
	Lcd_WriteData8Bit(0x00FF&y);
	Lcd_WriteData8Bit(y>>8);
	Lcd_WriteData8Bit(0x00FF&y);

	Lcd_WriteReg(0x2E);

    GPIOE->MODER = 0x0000; /* input mode */

    /* dummy */
    LCD_RS_H();
    __NOP();

    LCD_RD_L();
    LCD_RD_H();
    tmp = GPIOE->IDR;
    __NOP();

    /* dummy */
    LCD_RD_L();
    LCD_RD_H();
    tmp = GPIOE->IDR;
    __NOP();

    /* read H */
    LCD_RD_L();
    LCD_RD_H();
    tmp = GPIOE->IDR;
    __NOP();

    /* read L */
    tmp <<= 8;
    LCD_RD_L();
    LCD_RD_H();
    tmp |= (GPIOE->IDR & 0x00FF);

    /* clear config */
    GPIOE->ODR = 0x0000;
    GPIOE->MODER = 0x5555; /* output mode */
    LCD_RD_H();

#endif
    return tmp;
}



void
Lcd_WriteRegData(uint16_t reg, uint16_t data)
{
    Lcd_WriteReg(reg);
    Lcd_WriteData(data);
}


static void
Lcd_Reset(void)
{
#if __USE_IO_SIMULATE == 0
    GPIOA->ODR &= ~0x0002;
    Lcd_DelayMs(50);
    GPIOA->ODR |=  0x0002;
    Lcd_DelayMs(50);
#else
    LCD_RST_L();
    Lcd_DelayMs(50);
    LCD_RST_H();
    Lcd_DelayMs(50);
#endif
}

void
Lcd_Init(void)
{
    Lcd_PinInit();
#if __USE_IO_SIMULATE == 0
    Lcd_FSMCInit();
#else
    LCD_RD_H();
#endif

    Lcd_SetBackLight(0);

    /* reset */
    Lcd_Reset();

#if __LCD_DEVICE == __LCD_DEV_SSD1963
    /* SSD1963 */
	Lcd_WriteReg(0x00E2);	//PLL multiplier, set PLL clock to 120M
	Lcd_WriteData(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	Lcd_WriteData(0x0002);
	Lcd_WriteData(0x0004);
	Lcd_WriteReg(0x00E0);  // PLL enable
	Lcd_WriteData(0x0001);
	Lcd_DelayMs(1);
	Lcd_WriteReg(0x00E0);
	Lcd_WriteData(0x0003);
	Lcd_DelayMs(5);
	Lcd_WriteReg(0x0001);  // software reset
	Lcd_DelayMs(5);
	Lcd_WriteReg(0x00E6);	//PLL setting for PCLK, depends on resolution
	Lcd_WriteData(0x0003);
	Lcd_WriteData(0x00ff);
	Lcd_WriteData(0x00ff);

	Lcd_WriteReg(0x00B0);	//LCD SPECIFICATION
	Lcd_WriteData(0x0027);
	Lcd_WriteData(0x0000);
	Lcd_WriteData((HDP>>8)&0X00FF);  //Set HDP
	Lcd_WriteData(HDP&0X00FF);
    Lcd_WriteData((VDP>>8)&0X00FF);  //Set VDP
	Lcd_WriteData(VDP&0X00FF);
    Lcd_WriteData(0x0000);

	Lcd_WriteReg(0x00B4);	//HSYNC
	Lcd_WriteData((HT>>8)&0X00FF);  //Set HT
	Lcd_WriteData(HT&0X00FF);
	Lcd_WriteData((HPS>>8)&0X00FF);  //Set HPS
	Lcd_WriteData(HPS&0X00FF);
	Lcd_WriteData(HPW);			   //Set HPW
	Lcd_WriteData((LPS>>8)&0X00FF);  //Set HPS
	Lcd_WriteData(LPS&0X00FF);
	Lcd_WriteData(0x0000);

	Lcd_WriteReg(0x00B6);	//VSYNC
	Lcd_WriteData((VT>>8)&0X00FF);   //Set VT
	Lcd_WriteData(VT&0X00FF);
	Lcd_WriteData((VPS>>8)&0X00FF);  //Set VPS
	Lcd_WriteData(VPS&0X00FF);
	Lcd_WriteData(VPW);			   //Set VPW
	Lcd_WriteData((FPS>>8)&0X00FF);  //Set FPS
	Lcd_WriteData(FPS&0X00FF);

	Lcd_WriteReg(0x00BA);
	Lcd_WriteData(0x000F);    //GPIO[3:0] out 1

	Lcd_WriteReg(0x00B8);
	Lcd_WriteData(0x0007);    //GPIO3=input, GPIO[2:0]=output
	Lcd_WriteData(0x0001);    //GPIO0 normal

	Lcd_WriteReg(0x0036); //rotation
	Lcd_WriteData(0x0000);

	Lcd_WriteReg(0x00F0); //pixel data interface
	Lcd_WriteData(0x0003);

	Lcd_DelayMs(5);

	Lcd_WriteReg(0x0029); //display on

	Lcd_WriteReg(0x00d0);
	Lcd_WriteData(0x000d);
#elif __LCD_DEVICE == __LCD_DEV_ILI9486
	Lcd_WriteReg(0XF9);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x08);

	Lcd_WriteReg(0xC0);
	Lcd_WriteData8Bit(0x19);//VREG1OUT POSITIVE
	Lcd_WriteData8Bit(0x1a);//VREG2OUT NEGATIVE

	Lcd_WriteReg(0xC1);
	Lcd_WriteData8Bit(0x45);//VGH,VGL    VGH>=14V.
	Lcd_WriteData8Bit(0x00);

	Lcd_WriteReg(0xC2);
	Lcd_WriteData8Bit(0x33);

	Lcd_WriteReg(0XC5);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x28);//VCM_REG[7:0]. <=0X80.

	Lcd_WriteReg(0xB1);
	Lcd_WriteData8Bit(0xB0);//0XB0 =70HZ, <=0XB0.
	Lcd_WriteData8Bit(0x11);

	Lcd_WriteReg(0xB4);
	Lcd_WriteData8Bit(0x02); //2 DOT FRAME MODE,F<=70HZ.

	Lcd_WriteReg(0xB6);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x42);//0 GS SS SM ISC[3:0];
	Lcd_WriteData8Bit(0x3B);

	Lcd_WriteReg(0xB7);
	Lcd_WriteData8Bit(0x07);

	Lcd_WriteReg(0xE0);
	Lcd_WriteData8Bit(0x1F);
	Lcd_WriteData8Bit(0x25);
	Lcd_WriteData8Bit(0x22);
	Lcd_WriteData8Bit(0x0B);
	Lcd_WriteData8Bit(0x06);
	Lcd_WriteData8Bit(0x0A);
	Lcd_WriteData8Bit(0x4E);
	Lcd_WriteData8Bit(0xC6);
	Lcd_WriteData8Bit(0x39);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x00);

	Lcd_WriteReg(0XE1);
	Lcd_WriteData8Bit(0x1F);
	Lcd_WriteData8Bit(0x3F);
	Lcd_WriteData8Bit(0x3F);
	Lcd_WriteData8Bit(0x0F);
	Lcd_WriteData8Bit(0x1F);
	Lcd_WriteData8Bit(0x0F);
	Lcd_WriteData8Bit(0x46);
	Lcd_WriteData8Bit(0x49);
	Lcd_WriteData8Bit(0x31);
	Lcd_WriteData8Bit(0x05);
	Lcd_WriteData8Bit(0x09);
	Lcd_WriteData8Bit(0x03);
	Lcd_WriteData8Bit(0x1C);
	Lcd_WriteData8Bit(0x1A);
	Lcd_WriteData8Bit(0x00);

	Lcd_WriteReg(0XF1);
	Lcd_WriteData8Bit(0x36);
	Lcd_WriteData8Bit(0x04);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x3C);
	Lcd_WriteData8Bit(0x0F);
	Lcd_WriteData8Bit(0x0F);
	Lcd_WriteData8Bit(0xA4);
	Lcd_WriteData8Bit(0x02);

	Lcd_WriteReg(0XF2);
	Lcd_WriteData8Bit(0x18);
	Lcd_WriteData8Bit(0xA3);
	Lcd_WriteData8Bit(0x12);
	Lcd_WriteData8Bit(0x02);
	Lcd_WriteData8Bit(0x32);
	Lcd_WriteData8Bit(0x12);
	Lcd_WriteData8Bit(0xFF);
	Lcd_WriteData8Bit(0x32);
	Lcd_WriteData8Bit(0x00);

	Lcd_WriteReg(0XF4);
	Lcd_WriteData8Bit(0x40);
	Lcd_WriteData8Bit(0x00);
	Lcd_WriteData8Bit(0x08);
	Lcd_WriteData8Bit(0x91);
	Lcd_WriteData8Bit(0x04);

	Lcd_WriteReg(0XF8);
	Lcd_WriteData8Bit(0x21);
	Lcd_WriteData8Bit(0x04);

	Lcd_WriteReg(0x36);
	Lcd_WriteData8Bit(0x48);

	Lcd_WriteReg(0x3A);
	Lcd_WriteData8Bit(0x55);

	Lcd_WriteReg(0x11);
	Lcd_DelayMs(120);
	Lcd_WriteReg(0x29);

	Lcd_WriteReg(0x36);//BGR==1,MY==1,MX==0,MV==1
	Lcd_WriteData8Bit(0b10101000);
#endif


    Lcd_SetBackLight(100);
}


void Lcd_SetWindow(uint16_t xs, uint16_t ys,uint16_t xe,uint16_t ye)
{
#if __LCD_DEVICE == __LCD_DEV_SSD1963
	Lcd_WriteReg(0x002A);
	Lcd_WriteData(xs>>8);
	Lcd_WriteData(xs&0x00ff);
	Lcd_WriteData(xe>>8);
	Lcd_WriteData(xe&0x00ff);
    Lcd_WriteReg(0x002b);
	Lcd_WriteData(ys>>8);
	Lcd_WriteData(ys&0x00ff);
	Lcd_WriteData(ye>>8);
	Lcd_WriteData(ye&0x00ff);

	Lcd_WriteReg(0x002c);
#elif __LCD_DEVICE == __LCD_DEV_ILI9486
	Lcd_WriteReg(0x2A);
	Lcd_WriteData8Bit(xs>>8);
	Lcd_WriteData8Bit(0x00FF&xs);
	Lcd_WriteData8Bit(xe>>8);
	Lcd_WriteData8Bit(0x00FF&xe);

	Lcd_WriteReg(0x2B);
	Lcd_WriteData8Bit(ys>>8);
	Lcd_WriteData8Bit(0x00FF&ys);
	Lcd_WriteData8Bit(ye>>8);
	Lcd_WriteData8Bit(0x00FF&ye);

	Lcd_WriteReg(0x2C);
#endif
}

void
Lcd_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    Lcd_SetWindow(x, y, x, y);
    Lcd_WriteData(color);
}

void
Lcd_Clear(uint16_t color)
{
    int x = __LCD_X_MAX, y = __LCD_Y_MAX;
    int i, j;

    Lcd_SetWindow(0, 0, x - 1, y - 1);
    for(i = 0; i < y; i++) {
        for(j = 0; j < x; j++) {
            Lcd_WriteData(color);
        }
    }
}
