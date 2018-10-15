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
		cout << "person(char *name,int age),name = " << name << ",age = "<< age << endl;
		this->age = age;
		
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);
		
		this->work = new char[strlen(work)+1];
		strcpy(this->work,work);
	}

	person(person &per)
	{
		cout << "person(person &per)" << endl;
		this->age = per.age;

		this->name = new char[strlen(per.name)+1];
		strcpy(this->name,per.name);
		
		this->work = new char[strlen(per.work)+1];
		strcpy(this->work,per.work);
	}
	
	~person()
	{
		cout << "~person()" << endl;
	
		if (this->name){
			cout << "name = " << name << endl;
			delete this->name;
		}
		if (this->work){
			cout << "work = " << work << endl;
			delete this->work;
		}
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

person per_g("per_g",10);

void fun()
{
	person per_fun("per_fun",11);
	static person per_fun_s("per_fun_s",11);
}

int main(int argc,char **argv)
{
	person per_main("per_main",11);
	static person per_main_s("per_main_s",11);

	for(int i = 0;i < 2;i ++){
		fun();
		person per_for("per_for",i);
	}


	return 0;
}



