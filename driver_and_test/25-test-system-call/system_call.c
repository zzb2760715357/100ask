#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __NR_SYSCALL_BASE	0x900000


void hello(char *buf,int count)
{
	asm("mov r0,%0\n"
		"mov r1,%1\n"
		"swi %2\n"
		:
		:"r"(buf),"r"(count),"i"(__NR_SYSCALL_BASE + 352)
		:"r0","r1");
}

int main(int argc,char **argv)
{

	printf("in app, call hello\n");
	hello("www.100ask.net", 15);

	return 0;
}

