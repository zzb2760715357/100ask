#include <stdio.h>

class person {
private:
	char *name;
	int age;
	char *work;
	
public:
	void setName(char *name);	
	int setAge(int age);
	void setWork(char *work);	
	void printInfo(void);
};
