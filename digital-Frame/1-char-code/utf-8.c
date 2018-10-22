#include <stdio.h>

int main(int argc, char **argv)
{
	int i = 0;
	unsigned char *str="abc中";

	while (str[i])
	{
		printf("%02x ", str[i]);
		i++;
	}
	printf("\n");
	return 0;
}