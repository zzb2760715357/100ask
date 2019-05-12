#include <stdio.h>

struct persion{
	char *name;
};

int main(void)
{
	struct persion p1 = {"zhangsan"};
	struct persion p2 = {"lisi"};

	printf("%s\r\n",p1.name);
	printf("%s\r\n",p2.name);

	return 0;
}
