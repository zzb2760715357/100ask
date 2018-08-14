### 最简单bootloader的编写步骤
### 1.初始化硬件:关闭看门狗、设置时钟、设置SDRAM、初始化Nand flash
### 2.把bootloader从定位到sdram
### 3.把内核nand flash读到sdram
### 4.设置要传递给内核的参数
### 5.跳转到内核执行

![看门狗的寄存器](https://i.imgur.com/zQAbVtb.png)

### 根据上图的看门狗的寄存器配置，要关闭看门狗只需要往0x53000000写入0即可
###	相关代码如下:
	ldr r0,=0x53000000    @ set watch dog register
	mov r1,#0x0
	str r1,[r0]

### 设置时钟：FCLK:HCLK:PCLK=1:2:4
![设置分频系数](https://i.imgur.com/0IcDhA5.png)
### 相关代码
	/*设置时钟*/
	ldr r0,=0x4C000014
	mov r1,#0x3
	str r1,[r0]

	/* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
    mrc    p15, 0, r1, c1, c0, 0       /* 读出控制寄存器 */ 
    orr    r1, r1, #0xc0000000          /* 设置为“asynchronous bus mode” */
    mcr    p15, 0, r1, c1, c0, 0        /* 写入控制寄存器 */

### 设置MPLL的时钟
![](https://i.imgur.com/uiYCrsL.png)
### 相关代码
	/*
	 * 对于MPLLCON寄存器，[19:12]为MDIV，[9:4]为PDIV，[1:0]为SDIV
	 * 有如下计算公式：
	 *  S3C2410: MPLL(FCLK) = (m * Fin)/(p * 2^s)
	 *  S3C2440: MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
	 *  其中: m = MDIV + 8, p = PDIV + 2, s = SDIV
	 * 对于本开发板，Fin = 12MHz
	 * 设置CLKDIVN，令分频比为：FCLK:HCLK:PCLK=1:2:4，
	 * FCLK=200MHz,HCLK=100MHz,PCLK=50MHz
	 */
	ldr r0,=0x4C000004
	ldr r1,=S3C2440_MPLL_200MHZ        /* 设置时钟为200MHZ */  
	str r1,[r0]     
	
### 初始化sdram主要是配置寄存器即可
	ldr r0,=MEM_CTL_BASE
	adr r1,mem_cfg_val  /*mem_cfg_val的当前地址*/
	add r3,r0,#(13*4)
	1:
	ldr r2,[r1],#4
	str r2,[r0],#4
	cmp r0,r3
	bne 1b
	

### 重定位 把bootloader的代码复制到链接地址去
### 2440的启动流程nor flash或nandflash 如何判断是nor或者nand启动，往0地址写入一个值如果ok是nandflash启动否则为norflash启动，nandflash启动的0地址在片内的ram中
![](https://i.imgur.com/6l18Efd.png)

	void copy_code_to_sdram(unsigned char *src,unsigned char *dest,unsigned int len)
	{
		int i = 0;
	
		//判断是nor启动还是nandflash启动
		if(isBootFromNorFlash){
			while (i < len){
				dest[i] = src[i];
				i ++;
			}
		}else {
			nand_init();
			nand_read(src,dest,len);
		}
	}


	

### nandflash的设置
	twp >=15ns
	100MHZ=10ns
	10*(x+1)>=15 
![](https://i.imgur.com/FtVVAd0.png)
![](https://i.imgur.com/pXnJ7MJ.png)
![](https://i.imgur.com/7yPMeDw.png)
![](https://i.imgur.com/nOyjlCP.png)
	

### nandflash存在位反转oob存在的作用
![](https://i.imgur.com/i5FEEwU.png)
### nandflash的读操作
![](https://i.imgur.com/FJ2w8yx.png)

### 启动内核步骤
##### 0. 初始化串口
##### 1. 从NAND FLASH把内核读入内存
##### 2. 设置参数
##### 3. 跳转执行	
### 内核存放在nandflash的地址、加载地址0x30008000
	device nand0 <nandflash0>, # parts = 4
	 #: name                        size            offset          mask_flags
	 0: bootloader          0x00040000      0x00000000      0
	 1: params              0x00020000      0x00040000      0
	 2: kernel              0x00200000      0x00060000      0
	 3: root                0x0fda0000      0x00260000      0

	
	Reading data from 0x25f800 -- 100% complete.
	 2097152 bytes read: OK
	## Booting image at 30007fc0 ...
	   Image Name:   Linux-2.6.22.6-gd3dce933
	   Created:      2018-08-10   8:16:48 UTC
	   Image Type:   ARM Linux Kernel Image (uncompressed)
	   Data Size:    1848728 Bytes =  1.8 MB
	   Load Address: 30008000
	   Entry Point:  30008000
	   Verifying Checksum ... OK
	   XIP Kernel Image ... OK


### 仿照Armlinux.c的do_bootm_linux函数 u-boot-1.1.6\lib_arm)	写内核的启动

### 传递命令行参数设置一些tag
	字符串的大小要向4取整,根据前面的 tag分析出来

### 编译错误
	Makefile添加-fno-builtin
	init.c:192:6: warning: conflicting types for built-in function 'putc'
	init.c:201:6: warning: conflicting types for built-in function 'puts'

### 调试过程
	寄存器名字的设置以及传递参数的地址

### 优化提高速度
	提高时钟频率
	启动Cache

	
		


	