#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int fd = -1;

void my_signal_fun (int signum)
{
	unsigned char key_val;
	read(fd,&key_val,1);
	printf("key_val:%0x\r\n",key_val);
}


int main(int argc,char **argv)
{
	
	int ret = -1;
	struct pollfd fds[1];
	int Oflags;
	
	signal(SIGIO,my_signal_fun);

	fd = open("/dev/buttons_fasync",O_RDWR);
	if (fd < 0){
		printf("open /dev/buttons_fasync error \r\n");
		return -1;
	}

	fcntl(fd,F_SETOWN,getpid());
	Oflags = fcntl(fd,F_GETFL);
	fcntl(fd,F_SETFL,Oflags|FASYNC);

	while (1){
		sleep(1000);
	}

	close(fd);

	return 0;	
}
