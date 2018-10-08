
#ifndef _OLED_H
#define  _OLED_H

void OLEDInit(void);
void OLEDPrint(int page, int col, char *str);
void OLEDPutChar(int page, int col, char c);
void OLEDClearPage(int page);

#endif

