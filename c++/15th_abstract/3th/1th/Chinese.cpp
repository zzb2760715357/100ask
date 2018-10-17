#include "Chinese.h"

void Chinese::setName(char *name)
{
	this->name = name;
}

char *Chinese::getName(void)
{
	return this->name;
}


void Chinese::eating(void)
{
	cout << "use chopsticks to eat" << endl;
}
void Chinese::wearing(void)
{
	cout << "Chinese wearing" << endl;
}

void Chinese::driving(void)
{
	cout << "Chinese driving" << endl;
}

Chinese::~Chinese()
{
	cout << "~Chinese()" << endl;
}



