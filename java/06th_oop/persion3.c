#include <stdio.h>

struct persion{
	char *name;
	void (*printname)(struct persion *per);
};

void printname(struct persion *per)
{
	printf("guangdong %s\n",per->name);
}


int main(void)
{
	struct persion p1 = {"zhangsan",printname};
	struct persion p2 = {"lisi",printname};

	p1.printname(&p1);
	p2.printname(&p2);

	return 0;
}

