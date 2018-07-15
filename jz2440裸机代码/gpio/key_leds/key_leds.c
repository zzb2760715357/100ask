#define GPFCON    (*(volatile unsigned long *)0x56000050)
#define GPFDAT    (*(volatile unsigned long *)0x56000054)

#define GPGCON    (*(volatile unsigned long *)0x56000060)
#define GPGDAT    (*(volatile unsigned long *)0x56000064)

//LED1,LED2,LED3
#define GPF4_OUT  (1<<(4*2))
#define GPF5_OUT  (1<<(5*2))
#define GPF6_OUT  (1<<(6*2))

#define GPF4_MASK (3<<(4*2))
#define GPF5_MASK (3<<(5*2))
#define GPF6_MASK (3<<(6*2))

//KEY1,KEY2,KEY3
#define GPF0_IN   (0<<(0*4))
#define GPF2_IN   (0<<(2*4))
#define GPG3_IN   (0<<(3*4))

#define GPF0_MAKS (3<<(0*2))
#define GPF2_MAKS (3<<(2*2))
#define GPG3_MAKS (3<<(3*2))

int main()
{
	unsigned long key_status;

	//LED设置为输出引脚
	GPFCON = GPFCON & ~(GPF4_MASK|GPF5_MASK|GPF6_MASK);	
	GPFCON = GPFCON | (GPF4_OUT|GPF5_OUT|GPF6_OUT);

	//KEY设置为输入引脚
	GPFCON = GPFCON & ~(GPF0_MAKS|GPF2_MAKS);
	GPFCON = GPFCON | (GPF0_IN|GPF2_IN);

	GPGCON = GPGCON & ~(GPG3_MAKS);
	GPGCON = GPGCON | GPG3_IN;

	while(1){
		key_status = GPFDAT;
		
		if (key_status & (1<<0))
			GPFDAT |= (1<<4);
		else 
			GPFDAT &= ~(1<<4);

		if (key_status & (1<<2))
			GPFDAT |= (1<<5);
		else
			GPFDAT &= ~(1<<5);
		
		key_status = GPGDAT;
		
		if (key_status & (1<<3))
			GPFDAT |= (1<<6);
		else
			GPFDAT &= ~(1<<6);
	}


	return 0;
}

