#include <stdio.h>

class person {
private:
	char *name;
	int age;
	char *work;
	
public:
	void setName(char *name);	
	int setAge(int age);
	void setWork(char *work);	
	void printInfo(void);
};
void person::setName(char *name)
{
	this->name = name;
}

int person::setAge(int age)
{
	if (age < 0 || age > 150){
		this->age = 0;
		return -1;
	}
	this->age = age;
	return 0;
}
void person::setWork(char *work)
{
	this->work = work;
}
void person::printInfo(void)
{
	printf("name = %s, age = %d, work = %s\n", name, age, work); 
}

int main(int argc,char **argv)
{
	person per;

	per.setName("zhuangsan");
	per.setAge(24);
	per.setWork("teacher");
	per.printInfo();
	
	return 0;
}
