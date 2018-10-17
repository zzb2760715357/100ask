#include "Chinese.h"
#include "Human.h"


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

Human& CreateChinese(char *name,int age,char *address)
{
	return *(new Chinese());
}



