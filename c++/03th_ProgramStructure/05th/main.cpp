#include <stdio.h>
#include "person.h"
#include "dog.h"


using namespace A;
using namespace C;

//using A::person;
//using C::dog;


int main(int argc,char **argv)
{
	//A::person per;
	
	person per;
	per.setName("zhuangsan");
	per.setAge(24);
	per.setWork("teacher");
	per.printInfo();

	//C::dog dogs;
	dog dogs;
	dogs.setName("wangcai");
	dogs.setAge(1);
	dogs.printInfo();

	A::printversion();
	C::printversion();
	
	return 0;
}
