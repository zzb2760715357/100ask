#include <iostream>


using namespace std;

int add(int a,int b)
{
	cout <<"add int + int" <<endl;
	return a + b;
}
int add(int a,int b,int c)
{
	cout << "add int + int + int" << endl;
	return a + b + c;
}
double add(double a,double b)
{
	cout << "add double + double " << endl;
	return a + b;
}
double add(int a,double b)
{
	cout << "add int + double " << endl;
	return a + b;
}
double add(double a,int b)
{
	cout << "add double + int " << endl;
	return a + b;
}


int main(int argc,char **argv)
{
	add(1,2);
	add(1,2,3);
	add(1.0,2.0);
	add(1,2.0);
	add(2.0,1.0);

	return 0;
}


