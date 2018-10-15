#include <iostream>
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
	std::cout<< "name = " << name << ",age = " << age << ",work = " << work << std::endl;
}

void printversion(void)
{
	std::cout << "person version v1 ,by zhuangzebin" << std::endl;
}

}

