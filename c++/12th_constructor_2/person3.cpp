#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Furniture{
private:
	int weight;

public:
void setWeight(int weight)
	{
		this->weight = weight;
	}
	int getWeight(void) const
	{
		return weight;
	}
	Furniture()
	{
		cout << "Furniture()" << endl;
	}
};

class Vertification3C{
public:
	Vertification3C()
	{
		cout << "Vertification3C()" << endl;
	}
};

class Sofa :virtual public Furniture,virtual public Vertification3C{
private:
	int a;
public:
	void watchTV(void)
	{
		cout << "watchTV" << endl;
	}

	Sofa()
	{
		cout << "Sofa()" << endl;
	}
	

};

class Bed:virtual public Furniture,virtual public Vertification3C{
private:
	int b;

public:
	void sleep(void)
	{
		cout << "sleep" << endl;
	}

	Bed()
	{
		cout << "Bed()" << endl;
	}
};

class Sofabed:public Sofa,public Bed{
private:
	int c;
public:
	Sofabed()
	{
		cout << "Sofabed()" << endl;
	}
	Sofabed(char *str)
	{
		cout << "Sofabed(char *str)" << endl;
	}

};

class LeftRightCom{
public:
	LeftRightCom()
	{
		cout << "LeftRightCom()" << endl;
	}
	LeftRightCom(char *str)
	{
		cout << "LeftRightCom(char *str)" << endl;
	}
};

class Date{
public:
	Date()
	{
		cout << "Date()" << endl;
	}
	Date(char *str)
	{
		cout << "Date(char *str)" << endl;
	}
};

class Type{
public:
	Type()
	{
		cout << "Type()" << endl;
	}
	Type(char *str)
	{
		cout << "Type(char *str)" << endl;
	}
};


class LeftRightComBed:public Sofabed ,public LeftRightCom {
private :
	Date date;
    Type type;
public:
	LeftRightComBed()
	{
		cout << "LeftRightComBed()" << endl;
	}
	LeftRightComBed(char *str1,char *str2,char *str3):Sofabed(str1),LeftRightCom(str2),date(str3)
	{
		cout << "LeftRightComBed()" << endl;
	}
	
};

int main(int argc,char **argv)
{

	LeftRightComBed s("abc","123","yyy");
	
	return 0;
}





