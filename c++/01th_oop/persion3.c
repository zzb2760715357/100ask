#include <stdio.h>

struct person {
	char *name;
	int age;
	char *work;

	void (*printInfo)(struct person *per);
};

void printInfo(struct person *per)
{
	printf("name = %s,age = %d,work = %s\r\n",per->name,per->age,per->work);		
}

int main(int argc,char **argv)
{
	struct person persons[] = {
			{"zhuangsan",10,"teacher",printInfo},
			{"lisi",16,"doctor",printInfo},
	};

	int i;

	persons[0].printInfo(&persons[0]);
	persons[1].printInfo(&persons[1]);

	return 0;
}
