#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>



int main(int argc,char **argv)
{
	int fd = -1;
	int val = -1;

	if (argc != 2){
		printf("usage:\r\n");
		printf("%s <on|off>\r\n",argv[0]);
		return 0;
	}

	fd = open("/dev/xxx",O_RDWR);
	if (fd < 0){
		printf("open /dev/xxx error \r\n");
		return -1;
	}

	if (strcmp(argv[1],"on") == 0){
		val = 1;
	}else  {
		val = 0;
	}
	
	fprintf(stderr,"APP:val = %d\r\n",val);
	
	write(fd,&val,4);

	return 0;	
}
