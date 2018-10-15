#include <iostream>
#include "dog.h"

namespace C{
using namespace std;

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
	cout << "name = " << name <<",age = " << age << endl;
}

void printversion(void)
{
	cout << "dog version v1 ,by zhuangzebin"<<endl;
}
}


