#include "s3c24xx.h"

/* 用GPIO模拟SPI */

static void SPI_GPIO_Init(void)
{
    /* GPF1 OLED_CSn output */
    GPFCON &= ~(3<<(1*2));
    GPFCON |= (1<<(1*2));
    GPFDAT |= (1<<1);

    /* GPG2 FLASH_CSn output
    * GPG4 OLED_DC   output
    * GPG5 SPIMISO   input
    * GPG6 SPIMOSI   output
    * GPG7 SPICLK    output
    */
    GPGCON &= ~((3<<(2*2)) | (3<<(4*2)) | (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
    GPGCON |= ((1<<(2*2)) | (1<<(4*2)) | (1<<(6*2)) | (1<<(7*2)));
    GPGDAT |= (1<<2);
}

static void SPI_Set_CLK(char val)
{
    if (val)
        GPGDAT |= (1<<7);
    else
        GPGDAT &= ~(1<<7);
}

static void SPI_Set_DO(char val)
{
    if (val)
        GPGDAT |= (1<<6);
    else
        GPGDAT &= ~(1<<6);
}

static char SPI_Get_DI(void)
{
    if (GPGDAT & (1<<5))
        return 1;
    else 
        return 0;
}

void SPISendByte(unsigned char val)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        SPI_Set_CLK(0);
        SPI_Set_DO(val & 0x80);
        SPI_Set_CLK(1);
        val <<= 1;
    }
    
}

unsigned char SPIRecvByte(void)
{
    int i;
    unsigned char val = 0;
    for (i = 0; i < 8; i++)
    {
        val <<= 1;
        SPI_Set_CLK(0);
        if (SPI_Get_DI())
            val |= 1;
        SPI_Set_CLK(1);
    }
    return val;    
}


void SPIInit(void)
{
    /* 初始化引脚 */
    SPI_GPIO_Init();
}

