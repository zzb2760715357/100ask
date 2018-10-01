#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>




int main(int argc,char **argv)
{
	int fd = -1;
	int ret = -1;
	unsigned char key_vals;
	struct pollfd fds[1];

	fd = open("/dev/buttons_poll",O_RDWR);
	if (fd < 0){
		printf("open /dev/buttons_irq error \r\n");
		return -1;
	}

	fds[0].fd = fd;
	fds[0].events = POLLIN;

	while (1){
		ret = poll(fds,1,5000);
		if (ret == 0){
			printf("Poll Time Out \r\n");
		}else {
			read(fd,&key_vals,1);
			printf("key_val = 0x%x\r\n",key_vals);
		}
	}

	close(fd);

	return 0;	
}
