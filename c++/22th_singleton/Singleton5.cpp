#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class Singleton;
Singleton *gInstance;
static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;


class Singleton{
public:
	static Singleton *getInstance()
	{
		if (NULL == gInstance){
			pthread_mutex_lock(&g_tMutex);
			if (NULL == gInstance)
				gInstance = new Singleton;
			pthread_mutex_unlock(&g_tMutex);
		}
		return gInstance;
	}
void printInfo()
{
	cout << "This is Singleton" << endl;
}

private:
	Singleton()
	{
		cout << "Singleton()" << endl;
	}

	

};


void *start_routine_thread1(void *arg)
{
	cout << "this is thread 1 ..." << endl;

	Singleton *s = Singleton::getInstance();
	s->printInfo();
}

void *start_routine_thread2(void *arg)
{
	cout << "this is thread 2 ..." << endl;

	Singleton *s = Singleton::getInstance();
	s->printInfo();
}


int main(int argc,char **argv)
{
	Singleton *s = Singleton::getInstance();
	s->printInfo();

	Singleton *s1 = Singleton::getInstance();
	s1->printInfo();

	Singleton *s2 = Singleton::getInstance();
	s2->printInfo();

	//Singleton *s4 = new Singleton();
	//Singleton s5;

	pthread_t thread1ID;
	pthread_t thread2ID;

	pthread_create(&thread1ID,NULL,start_routine_thread1,NULL);
	pthread_create(&thread2ID,NULL,start_routine_thread2,NULL);
	
	sleep(3);

	return 0;
}






