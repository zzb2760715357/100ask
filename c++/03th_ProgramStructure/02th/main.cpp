#include <stdio.h>
#include "person.h"

int main(int argc,char **argv)
{
	person per;

	per.setName("zhuangsan");
	per.setAge(24);
	per.setWork("teacher");
	per.printInfo();
	
	return 0;
}
