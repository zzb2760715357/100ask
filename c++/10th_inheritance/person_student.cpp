
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Person {
private:
	char *name;
	int age;

public: 

	Person() {//cout <<"Pserson()"<<endl;
		name = NULL;
	}
	Person(char *name) 
	{
		//cout <<"Pserson(char *)"<<endl;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}

	Person(char *name, int age) 
	{
		cout <<"Pserson(char*, int), name = "<<name<<", age= "<<age<<endl;
		this->age = age;

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

	}

	Person(Person &per) 
	{
		cout <<"Pserson(Person &)"<<endl;
		this->age = per.age;

		this->name = new char[strlen(per.name) + 1];
		strcpy(this->name, per.name);
	}

	~Person()
	{
		cout << "~Person()"<<endl;
		if (this->name) {
			cout << "name = "<<name<<endl;
			delete this->name;
		}
	}

	void setName(char *name)
	{
		if (this->name){
			delete this->name;
		}

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		
	}
	int setAge(int a)
	{
		if (a < 0 || a > 150)
		{
			age = 0;
			return -1;
		}
		age = a;
		return 0;
	}
	void printInfo(void)
	{
		//printf("name = %s, age = %d, work = %s\n", name, age, work); 
		cout<<"name = "<<name<<", age = "<<age<<endl;
	}
};

class Student :public Person{
private :
	int grade;
	void setGrade(int grade){this->grade = grade;};
	int getGrade(void){return grade;}

public:
	void printInfo(void)
	{
		cout << "Student" ;
		Person::printInfo();
	}

};


void test_fun(Person &p)
{
	p.printInfo();
}

int main(int argc, char **argv)
{
	Person p("lisi",16);

	Student s;

	s.setName("zhangsan");
	s.setAge(16);

	test_fun(p);
	test_fun(s);
	
	s.printInfo();

	
	
	
	return 0;
}

