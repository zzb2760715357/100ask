#include <stdio.h>
#include "person.h"

namespace A{

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

void printversion(void)
{
	printf("person version v1 ,by zhuangzebin\r\n");
}

}

