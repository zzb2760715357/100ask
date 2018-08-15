#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>



int main(int argc,char **argv)
{
	int fd = -1;
	int cnt = -1;
	unsigned char key_vals[4];

	fd = open("/dev/buttons",O_RDWR);
	if (fd < 0){
		printf("open /dev/buttons error \r\n");
		return -1;
	}

	while (1){
		read(fd,key_vals,sizeof(key_vals));
		if (!key_vals[0] || !key_vals[1]||  !key_vals[2]||!key_vals[3] ){
			printf("%04d key pressed: %d %d %d %d\r\n",cnt++,key_vals[0],key_vals[1],key_vals[2],key_vals[3]);
		}
	}

	return 0;	
}
