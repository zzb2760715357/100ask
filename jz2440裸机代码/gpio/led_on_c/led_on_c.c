#define GPFCON  (*(volatile unsigned long *)0x56000050)
#define GPFDAT  (*(volatile unsigned long *)0x56000054)

int main()
{
	GPFCON = 0x00000100; //配置GPF4为输出口
	GPFDAT = 0x00000000; //输出低电平

	return 0;
}
