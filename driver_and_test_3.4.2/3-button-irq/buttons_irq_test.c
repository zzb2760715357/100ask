#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>



int main(int argc,char **argv)
{
	int fd = -1;
	int cnt = -1;
	unsigned char key_vals;

	fd = open("/dev/buttons_irq",O_RDWR);
	if (fd < 0){
		printf("open /dev/buttons_irq error \r\n");
		return -1;
	}

	while (1){
		read(fd,&key_vals,1);
		printf("key_val = %d\r\n",key_vals);
	}

	return 0;	
}
