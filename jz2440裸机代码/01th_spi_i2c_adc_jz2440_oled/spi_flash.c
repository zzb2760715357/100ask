
#include "s3c24xx.h"
#include "gpio_spi.h"

static void SPIFlash_Set_CS(char val)
{
    if (val)
        GPGDAT |= (1<<2);
    else
        GPGDAT &= ~(1<<2);
}

static void SPIFlashSendAddr(unsigned int addr)
{
    SPISendByte(addr >> 16);
    SPISendByte(addr >> 8);
    SPISendByte(addr & 0xff);
}

/* 
 * 
 */
void SPIFlashReadID(int *pMID, int *pDID)
{
    SPIFlash_Set_CS(0); /* Ñ¡ÖÐSPI FLASH */

    SPISendByte(0x90);

    SPIFlashSendAddr(0);

    *pMID = SPIRecvByte();
    *pDID = SPIRecvByte();

    SPIFlash_Set_CS(1);
}

static void SPIFlashWriteEnable(int enable)
{
    if (enable)
    {
        SPIFlash_Set_CS(0);
        SPISendByte(0x06);
        SPIFlash_Set_CS(1);
    }
    else
    {
        SPIFlash_Set_CS(0);
        SPISendByte(0x04);
        SPIFlash_Set_CS(1);
    }
}

static unsigned char SPIFlashReadStatusReg1(void)
{
    unsigned char val;
    SPIFlash_Set_CS(0);
    SPISendByte(0x05);
    val = SPIRecvByte();
    SPIFlash_Set_CS(1);
    return val;
}

static unsigned char SPIFlashReadStatusReg2(void)
{
    unsigned char val;
    SPIFlash_Set_CS(0);
    SPISendByte(0x35);
    val = SPIRecvByte();
    SPIFlash_Set_CS(1);
    return val;
}

static void SPIFlashWaitWhenBusy(void)
{
    while (SPIFlashReadStatusReg1() & 1);
}

static void SPIFlashWriteStatusReg(unsigned char reg1, unsigned char reg2)
{    
    SPIFlashWriteEnable(1);  
    
    SPIFlash_Set_CS(0);
    SPISendByte(0x01);
    SPISendByte(reg1);
    SPISendByte(reg2);
    SPIFlash_Set_CS(1);

    SPIFlashWaitWhenBusy();
}

static void SPIFlashClearProtectForStatusReg(void)
{
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(1<<7);
    reg2 &= ~(1<<0);

    SPIFlashWriteStatusReg(reg1, reg2);
}


static void SPIFlashClearProtectForData(void)
{
    /* cmp=0,bp2,1,0=0b000 */
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(7<<2);
    reg2 &= ~(1<<6);

    SPIFlashWriteStatusReg(reg1, reg2);
}

/* erase 4K */
void SPIFlashEraseSector(unsigned int addr)
{
    SPIFlashWriteEnable(1);  

    SPIFlash_Set_CS(0);
    SPISendByte(0x20);
    SPIFlashSendAddr(addr);
    SPIFlash_Set_CS(1);

    SPIFlashWaitWhenBusy();
}

/* program */
void SPIFlashProgram(unsigned int addr, unsigned char *buf, int len)
{
    int i;
    
    SPIFlashWriteEnable(1);  

    SPIFlash_Set_CS(0);
    SPISendByte(0x02);
    SPIFlashSendAddr(addr);

    for (i = 0; i < len; i++)
        SPISendByte(buf[i]);
    
    SPIFlash_Set_CS(1);

    SPIFlashWaitWhenBusy();
    
}

void SPIFlashRead(unsigned int addr, unsigned char *buf, int len)
{
    int i;
    
    SPIFlash_Set_CS(0);
    SPISendByte(0x03);
    SPIFlashSendAddr(addr);
    for (i = 0; i < len; i++)
        buf[i] = SPIRecvByte();

    SPIFlash_Set_CS(1);    
}


void SPIFlashInit(void)
{
    SPIFlashClearProtectForStatusReg();
    SPIFlashClearProtectForData();
}

