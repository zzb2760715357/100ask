#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Human{
private:
	int a;

public:
	virtual void eating(void) = 0;
	virtual void wearing(void) = 0;
	virtual void driving(void) = 0;
	virtual ~Human()
	{
		cout << "~Human()" << endl;
	}
	virtual Human* test(void)
	{
		cout << "Human test(void)" << endl;
		return this;
	}
};

class Englishman:public Human{
public:
	void eating(void)
	{
		cout << "use knife to eat" << endl;
	}
	void wearing(void)
	{
		cout << "Englishman wearing" << endl;
	}
	void driving(void)
	{
		cout << "Englishman driving" << endl;
	}

	virtual ~Englishman()
	{
		cout << "~Englishman()" << endl;
	}

	virtual Englishman* test(void)
	{
		cout << "Englishman test(void)" << endl;

		return this;
	}
};

class Chinese:public Human{
public:
	void eating(void)
	{
		cout << "use chopsticks to eat" << endl;
	}
	void wearing(void)
	{
		cout << "Chinese wearing" << endl;
	}
	#if 0
	void driving(void)
	{
		cout << "Chinese driving" << endl;
	}
	#endif
	virtual ~Chinese()
	{
		cout << "~Chinese()" << endl;
	}

	virtual Chinese* test(void)
	{
		cout << "Chinese test(void)" << endl;

		return this;
	}
};

class Guangximan :public Chinese{
	void driving(void)
	{
		cout << "Chinese driving" << endl;
	}
};

int main(int argc,char **argv)
{
	
	
	//Human h;
	Englishman e;
	Guangximan g;

	

	return 0;
	
}



