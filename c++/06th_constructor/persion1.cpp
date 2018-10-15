#include <iostream>

using namespace std;

class person {
private:
	char *name;
	int age;
	char *work;
	
public:

	person()
	{
		cout << "person()" << endl;
	}
	person(char *name)
	{
		cout << "person(char *name)" << endl;
		this->name = name;
	}
	person(char *name,int age)
	{
		cout << "person(char *name,int age)" << endl;
		this->name = name;
		this->age = age;
	}
	
	void setName(char *name)
	{
		this->name = name;
	}

	int setAge(int age)
	{
		if (age < 0 || age > 150){
			this->age = 0;
			return -1;
		}
		this->age = age;
		return 0;
	}
	void setWork(char *work)
	{
		this->work = work;
	}
	void printInfo(void)
	{
		//printf("name = %s, age = %d, work = %s\n", name, age, work); 
		cout << "name = " << name << ",age = " << age <<",work = " << work << endl;
	}
};

int main(int argc,char **argv)
{
	person per1("zhuang",25);
	person per2;

	per1.printInfo();
	
	return 0;
}
