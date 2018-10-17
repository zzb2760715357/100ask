#ifndef __ENGLISHMAN_H_
#define __ENGLISHMAN_H_

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "Human.h"


using namespace std;
class Englishman:public Human{
private:
	char address[100];
	int age;

public:
	
	void eating(void);
	void wearing(void);
	void driving(void);
	Englishman();
	virtual ~Englishman();
	Englishman(char *name,int age,char *address);
};

#endif
