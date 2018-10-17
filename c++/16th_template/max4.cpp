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

template<typename T>
const T* mymax2(const T* a,const T* b)
{
	cout << __PRETTY_FUNCTION__ <<endl;

	return (a < b)?b:a;
}




int main(int argc,char **argv)
{
	char a[] = "ab";
	char b[] = "cd";

	mymax(a,b);
	mymax2(a,b);

	char a2[] = "abc";
	char b2[] = "cd";

	//mymax(a2,b2);
	mymax2(a2,b2);


	return 0;
}



