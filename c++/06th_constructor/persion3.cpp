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
	person(char *name,int age,char *work = "none")
	{
		cout << "person(char *name,int age)" << endl;
		this->name = name;
		this->age = age;
		this->work = work;
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
	person per3();

	person *per4 = new person;
	person *per5 = new person();

	person *per6 = new person[2];

	person *per7 = new person("lisi",18,"student");
	person *per8 = new person("wangwu",18);

	per1.printInfo();
	per7->printInfo();
	per8->printInfo();

	delete per8;
	delete per7;
	delete []per6;
	delete per5;
	delete per4;

	
	return 0;
}
