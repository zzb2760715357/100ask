#include "Chinese.h"
#include "Englishman.h"
#include "Human.h"


int main(int argc,char **argv)
{
	Englishman e;
	Chinese c;

	e.setName("Bill");
	c.setName("zhangsan");

	e.eating();
	c.eating();

	cout << e.getName()<< endl;
	cout << c.getName()<< endl;

	return 0;
}


