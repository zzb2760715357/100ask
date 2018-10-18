#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class OS{
public:
	virtual void Install() = 0 ;
};

class LinuxOS:public OS{
public:
	virtual void Install() {cout << "Install Linux OS" << endl;};
};

class WindowOS:public OS{
public:
	virtual void Install() {cout << "Install Windows OS" << endl;};
};


class Computer{
public:
	virtual void printInfo() = 0;
};

class MAC :public Computer{
public:
	virtual void printInfo(){cout << "This is Mac, "; }
};

class MacWithLinux :public MAC,public LinuxOS{
public:
	void InstallOS(){printInfo();Install();}
};

class MacWithWindows :public MAC,public WindowOS{
public:
	void InstallOS(){printInfo();Install();}
};

class Lenovo :public Computer{
public:
	virtual void printInfo(){cout << "This is Lenovo, "; };
};

class LenovoWithLinux :public Lenovo,public LinuxOS{
public:
	void InstallOS(){printInfo();Install();}
};

class LenovoWithWindows :public Lenovo,public WindowOS{
public:
	void InstallOS(){printInfo();Install();}
};




int main(int argc,char **argv)
{
	MacWithLinux a;
	a.InstallOS();

	LenovoWithWindows b;
	b.InstallOS();
	

	return 0;
}


