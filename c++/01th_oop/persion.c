#include <stdio.h>

int main(int argc,char **argv)
{
	char *zs_name = "zhangsan";
	int zs_age = 10;

	char *ls_name = "lisi";
	int ls_age = 16;

	printf("name = %s,age = %d \r\n",zs_name,zs_age);
	printf("name = %s,age = %d \r\n",ls_name,ls_age);

	return 0;
}
