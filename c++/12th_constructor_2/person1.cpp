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

};

class LeftRightCom{
public:
	LeftRightCom()
	{
		cout << "LeftRightCom()" << endl;
	}
};

class Date{
public:
	Date()
	{
		cout << "Date()" << endl;
	}
};

class Type{
public:
	Type()
	{
		cout << "Type()" << endl;
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
};

int main(int argc,char **argv)
{

	LeftRightComBed s;

	return 0;
}





