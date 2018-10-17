#include "Chinese.h"
#include "Englishman.h"
#include "Human.h"

void test_eatting(Human *h)
{
	h->eating();
}

int main(int argc,char **argv)
{
	Human& e = CreateEnglishman("Bill",10,"sfdsffas");
	Human& c  = CreateChinese("zhangsan",10,"sfdsffas");

	Human *h[2] = {&e,&c};
	int i;

	for(i = 0;i < 2;i ++)
		test_eatting(h[i]);
	

	return 0;
}


