#ifndef __CHINESE_H_
#define __CHINESE_H_

#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;
class Chinese{
private:
	char *name;

public:
	void setName(char *name);
	char *getName(void);
	void eating(void);
	void wearing(void);
	void driving(void);	
	~Chinese();
};


#endif


