
#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H

void SPIFlashReadID(int *pMID, int *pDID);
void SPIFlashInit(void);
void SPIFlashEraseSector(unsigned int addr);
void SPIFlashProgram(unsigned int addr, unsigned char *buf, int len);
void SPIFlashRead(unsigned int addr, unsigned char *buf, int len);

#endif

