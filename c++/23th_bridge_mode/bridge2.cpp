#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class OS{
public:
	virtual void Install_impl() = 0 ;
};

class LinuxOS:public OS{
public:
	virtual void Install_impl() {cout << "Install Linux OS" << endl;};
};

class WindowOS:public OS{
public:
	virtual void Install_impl() {cout << "Install Windows OS" << endl;};
};

class UnixOS:public OS{
public:
	virtual void Install_impl() {cout << "Install UnixOS OS" << endl;};
};



class Computer{
public:
	virtual void printInfo() = 0;
	virtual void InstallOS() = 0;
};

class MAC :public Computer{
public:
	virtual void printInfo(){cout << "This is Mac, "; }
	MAC(OS *impl){this->impl = impl;}
	void InstallOS(){printInfo();impl->Install_impl();}
private:
	OS *impl;
};

class Lenovo :public Computer{
public:
	virtual void printInfo(){cout << "This is Lenovo, "; }
	Lenovo(OS *impl){this->impl = impl;}
	void InstallOS(){printInfo();impl->Install_impl();}
private:
	OS *impl;
};







int main(int argc,char **argv)
{
	OS *os1 = new LinuxOS();
	OS *os2 = new WindowOS();
	OS *os3 = new UnixOS();

	Computer *c1 = new MAC(os1);
	Computer *c2 = new Lenovo(os2);
	Computer *c3 = new Lenovo(os3);

	c1->InstallOS();
	c2->InstallOS();
	c3->InstallOS();
	

	return 0;
}


