#include "s3c24xx.h"

void Timer0_Handle()
{
	if (INTOFFSET == 10){
		GPFDAT = ~(GPFDAT&(0x07<<4));
	}
	SRCPND = 1 << INTOFFSET;
	INTPND = 1 << INTOFFSET;
}
