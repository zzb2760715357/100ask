#include <stdio.h>

class person {
private:
	char *name;
	int age;
	char *work;
	
public:
	void setName(char *n)
	{
		name = n;
	}

	int setAge(int a)
	{
		if (a < 0 || a > 150){
			age = 0;
			return -1;
		}
		age = a;
		return 0;
	}
	void setWork(char *w)
	{
		work = w;
	}
	void printInfo(void)
	{
		printf("name = %s, age = %d, work = %s\n", name, age, work); 
	}
};

int main(int argc,char **argv)
{
	person per;

	per.setName("zhuangsan");
	per.setAge(24);
	per.setWork("teacher");
	per.printInfo();
	
	return 0;
}
