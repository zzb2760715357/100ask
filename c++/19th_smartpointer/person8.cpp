
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class RefBase{
private :
	int count;
	
public:
	RefBase():count(0){}
	void incStrong(){count++;}
	void decStrong(){count--;}
	int getStrongCount(){return count;}

};

class Person:public RefBase {
public:

	Person() {
		cout <<"Pserson()"<<endl;
	}
	

	~Person()
	{
		cout << "~Person()"<<endl;
	}

	
	void printInfo(void)
	{
		cout << "Just a test function " << endl;
	}
};

class sp{
private:
	Person *p;
public:
	sp():p(0){}

	sp(Person *other)
	{
		cout << "sp(Person *other)" << endl;
		p = other;
		p->incStrong();
	}
	sp(const sp &other)
	{
		cout<<"sp(const sp &other)"<<endl;
		p = other.p;
		p->incStrong();
	}
	~sp()
	{
		cout << "~sp()" << endl;
		if(p){
			p->decStrong();
			if (p->getStrongCount() == 0){
				delete p;
				p = NULL;
			}
		}
			
	}

	Person *operator->()
	{
		return p;
	}
	Person& operator*()
	{
		return *p;
	}

};

void test_func(sp &other)
{
	sp s = other;

	cout << "In test_func: " << s->getStrongCount()<< endl;

	s->printInfo();
}


int main(int argc, char **argv)
{
	int i;

	sp other = new Person();

	(*other).printInfo();
	cout << "Before test_func: " << other->getStrongCount()<< endl;

	for (i = 0;i < 2;i ++){
		test_func(other);		
		cout << "After call test_func: " << other->getStrongCount()<< endl;
	}
	
	return 0;
}

