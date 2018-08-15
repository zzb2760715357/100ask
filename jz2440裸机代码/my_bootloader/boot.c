#include "setup.h"

extern void uart0_init(void);
extern void nand_read(unsigned int addr, unsigned char *buf, unsigned int len);
extern void puts(char *str);


static struct tag *params;

void setup_start_tag()
{
	params = (struct tag *)0x30000100;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}
void setup_memory_tags()
{
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size (tag_mem32);

	params->u.mem.start = 0x30000000;
	params->u.mem.size  = 0x4000000;

	params = tag_next (params);
}

int strlen(char *str)
{
	int i = 0;
	while(str[i]){
		i ++;
	}

	return i;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
void setup_commandline_tag(char *cmdline)
{
	int len = strlen(cmdline) + 1;
	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + len  + 3) >> 2;

	strcpy (params->u.cmdline.cmdline, cmdline);

	params = tag_next (params);
}
void setup_end_tag()
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

int main(void)
{
	void (*theKernel)(int zero, int arch, unsigned int params);
	
	/*0.串口初始化*/
	uart0_init();

	/* 1. 从NAND FLASH把内核读入内存 */
	puts("Copy kernel from nand\r\n");
	nand_read(0x60000+64,(unsigned char *)0x30008000,0x200000);
	
	/* 2. 设置参数 */
	puts("Set boot param\r\n");
	setup_start_tag ();
	setup_memory_tags ();
//	setup_commandline_tag ("noinitrd root=/dev/mtdblock3 rootfstype=jffs2 init=/linuxrc console=ttySAC0");
//	setup_commandline_tag ("noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0");

	setup_commandline_tag ("noinitrd root=/dev/nfs nfsroot=192.168.1.100:/work/nfs_root/first_fs ip=192.168.1.101:192.168.1.100:192.168.1.1:255.255.255.0::eth0:off  init=/linuxrc console=ttySAC0");
	setup_end_tag ();
	
	/* 3. 跳转执行 */
	puts("Boot kernel\r\n");
	theKernel = (void (*)(int, int, unsigned int))0x30008000;
	theKernel(0,362,0x30001000);

	puts("Error\r\n");
	return -1;
}


