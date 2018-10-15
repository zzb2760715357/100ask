#include <iostream>
#include <string.h>
#include <unistd.h>


using namespace std;

class person {
private:
	char *name;
	int age;
	char *work;
	
public:

	person()
	{
		//cout << "person()" << endl;
		name = NULL;
		work = NULL;
	}
	person(char *name)
	{
		//cout << "person(char *name)" << endl;
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);

		this->work = NULL;
	}
	person(char *name,int age,char *work = "none")
	{
		//cout << "person(char *name,int age)" << endl;
		this->age = age;
		
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);
		
		this->work = new char[strlen(work)+1];
		strcpy(this->work,work);
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
		//cout << "name = " << name << ",age = " << age <<",work = " << work << endl;
	}
};

void test_fun()
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

}

int main(int argc,char **argv)
{
	for(int i = 0;i < 1000000;i++)
		test_fun();
	cout << "run test_fun end" << endl;
	sleep(10);

	return 0;
}



