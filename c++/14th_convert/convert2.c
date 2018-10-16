#include <stdio.h>

int main(int argc,char **argv)
{
	double d = 100.1;
	int i = d;
	char *str = "zhuangzebin";
	int *p = (int *)str;

	printf("i = %d,str = 0x%x,p = 0x%x\r\n",i,(unsigned int)str,(unsigned int)p);


	return 0;
}


