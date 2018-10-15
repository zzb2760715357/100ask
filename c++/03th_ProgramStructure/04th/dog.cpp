#include <stdio.h>
#include "dog.h"

namespace C{

void dog::setName(char *name)
{
	this->name = name;
}

int dog::setAge(int age)
{
	if (age < 0 || age > 20){
		this->age = 0;
		return -1;
	}
	this->age = age;
	return 0;
}
void dog::printInfo(void)
{
	printf("name = %s, age = %d\n", name, age); 
}

void printversion(void)
{
	printf("dog version v1 ,by zhuangzebin\r\n");
}
}


