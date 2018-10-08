
#ifndef _S3C2440_SPI_H
#define _S3C2440_SPI_H

void SPIInit(void);
void SPISendByte(unsigned char val);
unsigned char SPIRecvByte(void);

#endif

