#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	int fd = -1;
	int val = 1;

	fd = open("/dev/xxx",O_RDWR);
	if (fd < 0){
		printf("open /dev/xxx error \r\n");
		return -1;
	}
	write(fd,&val,4);

	return 0;	
}
