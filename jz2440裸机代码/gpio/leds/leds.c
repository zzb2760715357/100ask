#define GPFCON  (*(volatile unsigned long *)0x56000050)
#define GPFDAT  (*(volatile unsigned long *)0x56000054)

#define GPF4_OUT    (1<<(4*2))
#define GPF5_OUT    (1<<(5*2))
#define GPF6_OUT    (1<<(6*2))

void delay(volatile unsigned long delay)
{
	for (;delay > 0;delay --);
}


int main()
{
	unsigned long i = 0;

	GPFCON = GPF4_OUT|GPF5_OUT|GPF6_OUT;  //配置为输出端口

	while(1){
		delay(30000);
		GPFDAT = (~(i<<4));
		if (++i == 8){
			i = 0;
		}
	}

	return 0;
}
