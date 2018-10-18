
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "RefBase.h"

using namespace std;
using namespace android::RSC;


class Person:public LightRefBase<Person> {
private:
	sp<Person> father;
	sp<Person> son;

public:
	

	Person() {
		cout <<"Pserson()"<<endl;
	}
	

	~Person()
	{
		cout << "~Person()"<<endl;
	}

	void setFather(sp<Person> &father)
	{
		this->father = father;
	}

	void setSon(sp<Person> &son)
	{
		this->son = son;
	}
	
	void printInfo(void)
	{
		cout << "Just a test function " << endl;
	}
};


void test_func()
{
	sp<Person> father = new Person();
	sp<Person> son = new Person();

	father->setSon(son);
	son->setFather(father);
}


int main(int argc, char **argv)
{
	
	test_func();
	cout << "end test func" << endl;
	
	
	return 0;
}

