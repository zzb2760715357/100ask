#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

#if 0
int& max(int& a,int& b)
{
	return (a < b)?b:a;
}

double& max(double& a,double& b)
{
	return (a < b)?b:a;
}

float& max(float& a,float& b)
{
	return (a < b)?b:a;
}
#endif
template<typename T>
const T& mymax(const T&a,const T&b)
{
	cout << __PRETTY_FUNCTION__ <<endl;

	return (a < b)?b:a;
}



int main(int argc,char **argv)
{
	int ia = 1;
	int ib = 2;

	mymax(ia,ib);


	return 0;
}



