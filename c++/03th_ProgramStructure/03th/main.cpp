#include <stdio.h>
#include "person.h"
#include "dog.h"

int main(int argc,char **argv)
{
	A::person per;

	per.setName("zhuangsan");
	per.setAge(24);
	per.setWork("teacher");
	per.printInfo();

	C::dog dogs;
	dogs.setName("wangcai");
	dogs.setAge(1);
	dogs.printInfo();

	A::printversion();
	C::printversion();
	
	return 0;
}
