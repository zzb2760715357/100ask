#include <stdio.h>

int main(int argc,char **argv)
{
	char *name[] = {"zhangsan","lisi"};
	char ages[] = {10,16};
	char *work[] = {"teacher","doctor"};

	int i;

	for (i = 0;i < 2;i ++){
		printf("name = %s,age = %d\r\n",name[i],ages[i]);
	}

	return 0;
}
