#include "tft.h"

#define Touch_PEN_Pin  GPIO_Pin_5
#define Touch_CS_Pin   GPIO_Pin_4

#define Touch_PEN_H  GPIO_ReadInputDataBit(GPIOC, Touch_PEN_Pin)

#define Touch_CS_H   GPIOC->ODR |=  Touch_CS_Pin
#define Touch_CS_L   GPIOC->ODR &= ~Touch_CS_Pin

#define AD_TOP      110
#define AD_LEFT     1950
#define AD_BOTTOM   1920
#define AD_RIGHT    120
#define LCD_WIDTH   480
#define LCD_HEIGHT  320

#define TOUCH_ABS(x) ( ((x) < 0) ? (-(x)) : (x) )

static void
Touch_DelayUs(volatile int x)
{
    volatile int i;

    while(x--) {
        for(i = 0; i < 10; i++) {
        } 
    }
}

/**
 * @brief 采用SPI2，PB13-SCK, PB14-MISO, PB15-MOSI, APB1默认为84MHZ
 */
void
SpiTouchPin_Init(void)
{
    GPIO_InitTypeDef gpio;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_Init(GPIOB, &gpio);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
}

/**
 * @brief 初始化spi
 * @param baduratePres: SPI时钟分频系数,如SPI_BaudRatePrescaler_16,参照st库头文件
 */
void
SpiTouch_Init(uint16_t baduratePres)
{
    SPI_InitTypeDef spi;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = baduratePres;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7; 
 
    SpiTouchPin_Init();
    SPI_Init(SPI2, &spi);
    SPI_Cmd(SPI2, ENABLE);
}


/**
 * @brief 读写SPI
 * @param data: 需要发送的数据
 * @return 接收到的数据
 */
uint8_t
SpiTouch_WriteAndRead(uint8_t data)
{
    while(!(SPI2->SR & SPI_I2S_FLAG_TXE));
    SPI2->DR = data;
    while(!(SPI2->SR & SPI_I2S_FLAG_RXNE));
    return SPI2->DR;
}


/**
 * @brief 触摸屏初始化
 */
void
Touch_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    SpiTouch_Init(SPI_BaudRatePrescaler_32); /* 初始化SPI2 */
}

/**
 * @brief 获取AD值
 */
static void
Touch_Get(uint16_t *x, uint16_t *y)
{
    Touch_CS_L;
    
    SpiTouch_WriteAndRead(0x90);
    Touch_DelayUs(10);
    *y = (uint16_t)SpiTouch_WriteAndRead(0x00) << 8;
    *y |= SpiTouch_WriteAndRead(0x00);
    
    SpiTouch_WriteAndRead(0xD0);
    Touch_DelayUs(10);
    *x = (uint16_t)SpiTouch_WriteAndRead(0x00) << 8;
    *x |= SpiTouch_WriteAndRead(0x00);
    
    *y >>= 4;
    *x >>= 4;
    
    Touch_CS_H;
}


/**
 * @brief 获取滤波后的AD值
 * @param *x: 存储xAD的指针
 * @param *y: 存储yAD的指针
 * @return 1: 滤波成功 0: 滤波失败
 */
uint8_t
Touch_GetData(uint16_t *x, uint16_t *y)
{
    uint16_t retry = 2;
    uint16_t cnt = 20;
    uint16_t x1, y1, x2, y2;

    Touch_Get(&x1, &y1);  
    while(retry) {
        Touch_Get(&x2, &y2);
        if(TOUCH_ABS(x2 - x1) < 10 && TOUCH_ABS(y2 - y1) < 10) {
            retry--; 
        }
        else {
            retry = 2;
            x1 = x2; y1 = y2;
        }
        cnt--;
        if(cnt == 0) break;
    }

    *x = y1; *y = x1;
    return (cnt == 0) ? 0 : 1;
}


/**
 * @brief 获取滤波后的坐标
 * @param *x: 存储x坐标的指针
 * @param *y: 存储y坐标的指针
 * @return 1: 滤波成功 0: 滤波失败
 */
uint8_t
Touch_GetXY(uint16_t *x, uint16_t *y) 
{
    uint8_t res;
    int32_t tx, ty;
    
    res = Touch_GetData((uint16_t *)&tx, (uint16_t *)&ty);
    tx = (tx - AD_LEFT) * LCD_WIDTH / (AD_RIGHT - AD_LEFT);
    ty = (ty - AD_TOP) * LCD_HEIGHT / (AD_BOTTOM - AD_TOP);

    /* 限制边缘 */
    tx = (tx < 0) ? 0 : tx;
    tx = (tx >= LCD_WIDTH) ? (LCD_WIDTH - 1) : tx;
    ty = (ty < 0) ? 0 : ty;
    ty = (ty >= LCD_HEIGHT) ? (LCD_HEIGHT - 1) : ty;

    *x = (uint16_t)tx;
    *y = (uint16_t)ty;
    
    return res;
}


