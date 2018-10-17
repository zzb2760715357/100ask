#ifndef __HUMAN_H_
#define __HUMAN_H_

#include <iostream>
#include <string.h>
#include <unistd.h>

class Human{
private:
	char *name;
public:
	void setName(char *name);
	char *getName(void);
};

#endif

