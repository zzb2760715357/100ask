#include <stdio.h>

int main(int argc, char **argv)
{
	int i = 0;
	unsigned char *str="abcÖĞ";

	while (str[i])
	{
		printf("%02x ", str[i]);
		i++;
	}
	printf("\n");
	return 0;
}