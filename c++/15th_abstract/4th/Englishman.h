#ifndef __ENGLISHMAN_H_
#define __ENGLISHMAN_H_

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "Human.h"


using namespace std;
class Englishman:public Human{
public:
	
	void eating(void);
	void wearing(void);
	void driving(void);

	~Englishman();
};

#endif
