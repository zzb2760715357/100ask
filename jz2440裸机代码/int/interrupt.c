#include "s3c24xx.h"

void EINT_Handle()
{
	unsigned long oft = INTOFFSET;

	switch(oft){
	
		case 0:
		{
			GPFDAT |= (0x07<<4);
			GPFDAT &= ~(1<<4);
			break;
		}
		
		case 2:
		{
			GPFDAT |= (0x07<<4);
			GPFDAT &= ~(1<<5);
			break;
		}

		case 5:
		{
			GPFDAT |= (0x07<<4);
			GPFDAT &= ~(1<<6);
			break;
		}	
		default:
			break;
	}

	if ( oft == 5){
		EINTPEND = 1<<11;
	}
	SRCPND = 1<<oft;
	INTPND = 1<<oft;

}
