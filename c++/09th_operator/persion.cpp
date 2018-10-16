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

	static int getCount(void);

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

	person(const person &per)
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
	void printInfo(void) const
	{
		//printf("name = %s, age = %d, work = %s\n", name, age, work); 
		cout << "name = " << name << ",age = " << age <<",work = " << work << endl;
	}

	person& operator=(const person &p) 
	{
		cout << "operator=(const person &p)" << endl;

		if (this == &p)
			return *this;
		this->age = p.age;

		if (this->name){
			delete this->name;
		}
		if (this->work){
			delete this->work;
		}

		this->name = new char[strlen(p.name)+1];
		strcpy(this->name,p.name);
		
		this->work = new char[strlen(p.work)+1];
		strcpy(this->work,p.work);

		return *this;
	}

	
};

int person::cnt = 0;
int person::getCount(void)
{
	return cnt;
}

int main(int argc,char **argv)
{
	const person p1("zhangsan",10);

	cout << "person p2 = p1" << endl;
	person p2 = p1;

	person p3;

	cout << "p3=p1" << endl;
	p3 = p1;
	cout << "end" << endl;

	p1.printInfo();
	p2.printInfo();
	p3.printInfo();

	
	return 0;
}



