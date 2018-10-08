
#ifndef _GPIO_SPI_H
#define _GPIO_SPI_H

void SPIInit(void);
void SPISendByte(unsigned char val);
unsigned char SPIRecvByte(void);

#endif

