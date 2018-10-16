#include <iostream>
#include <string.h>
#include <unistd.h>


using namespace std;

class person {
private:
	char *name;
	int age;
	char *work;
	static int cnt;
	
public:

	static int getCount(void)
	{
		return cnt;
	}
	

	person()
	{
		//cout << "person()" << endl;
		name = NULL;
		work = NULL;
		cnt ++;
	}
	person(char *name)
	{
		//cout << "person(char *name)" << endl;
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);

		this->work = NULL;
		cnt ++;
	}
	person(char *name,int age,char *work = "none")
	{
		cout << "person(char *name,int age),name = " << name << ",age = "<< age << endl;
		this->age = age;
		
		this->name = new char[strlen(name)+1];
		strcpy(this->name,name);
		
		this->work = new char[strlen(work)+1];
		strcpy(this->work,work);
		cnt ++;
	}

	person(person &per)
	{
		cout << "person(person &per)" << endl;
		this->age = per.age;

		this->name = new char[strlen(per.name)+1];
		strcpy(this->name,per.name);
		
		this->work = new char[strlen(per.work)+1];
		strcpy(this->work,per.work);
		cnt ++;
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

int person::cnt = 0;

int main(int argc,char **argv)
{
	
	cout << "person number = " << person::getCount()<<endl;

	return 0;
}



