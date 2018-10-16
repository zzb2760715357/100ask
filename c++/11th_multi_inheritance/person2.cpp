#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Sofa{
private:
	int weight;
public:
	void watchTV(void)
	{
		cout << "watchTV" << endl;
	}
	void setWeight(int weight)
	{
		this->weight = weight;
	}
	int getWeight(void) const
	{
		return weight;
	}

};

class Bed{
private:
	int weight;

public:
	void sleep(void)
	{
		cout << "sleep" << endl;
	}
	void setWeight(int weight)
	{
		this->weight = weight;
	}
	int getWeight(void) const
	{
		return weight;
	}
};

class Sofabed:public Sofa,public Bed{


};

int main(int argc,char **argv)
{

	Sofabed s;

	s.watchTV();
	s.sleep();

	s.Sofa::setWeight(100);
	//s.setWeight(100);

	cout << s.Sofa::getWeight() << endl;

	return 0;
}





