#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cnt = 0;

void C(void)
{
	int i = 0 ;
	while (1){
		printf("Hello ,cnt = %d,i = %d\n",cnt,i);
		cnt ++;
		i = i + 2;
		sleep(2);
	}
}


void B(void)
{
	C();
}

void A(void)
{
	B();
}

int main()
{

	A();
	return 0;
}
