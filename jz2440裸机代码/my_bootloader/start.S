#define MEM_CTL_BASE            0x48000000
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))

.text
.global __start
__start:

	/*关闭看门狗*/
	ldr r0,=0x53000000    @ set watch dog register
	mov r1,#0x0
	str r1,[r0]
	
	/*设置时钟*/
	ldr r0,=0x4C000014
	mov r1,#0x3
	str r1,[r0]

	/* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
    mrc    p15, 0, r1, c1, c0, 0       /* 读出控制寄存器 */ 
    orr    r1, r1, #0xc0000000         /* 设置为“asynchronous bus mode” */
    mcr    p15, 0, r1, c1, c0, 0       /* 写入控制寄存器 */

	ldr r0,=0x4C000004
	ldr r1,=S3C2440_MPLL_200MHZ        /* 设置时钟为200MHZ */  
	str r1,[r0]     

	/*初始化sdram*/
	ldr r0,=MEM_CTL_BASE
	adr r1,mem_cfg_val  /*mem_cfg_val的当前地址*/
	add r3,r0,#(13*4)
1:
	ldr r2,[r1],#4
	str r2,[r0],#4
	cmp r0,r3
	bne 1b
	
	/*重定位 把bootloader的代码复制到链接地址去*/
	ldr sp,=0x34000000

	bl nand_init
	
	mov r0,#0
	ldr r1,=__start
	ldr r2,=__bss_start
	sub r2,r2,r1
	
	bl copy_code_to_sdram
	bl clean_bss

	/*执行main*/
	ldr lr,=halt
	ldr pc,=main

halt:
	b halt

.align 4
mem_cfg_val:
	@ 存储控制器13个寄存器的设置值
	.long	0x22011110		@ BWSCON
	.long	0x00000700		@ BANKCON0
	.long	0x00000700		@ BANKCON1
	.long	0x00000700		@ BANKCON2
	.long	0x00000700		@ BANKCON3
	.long	0x00000700		@ BANKCON4
	.long	0x00000700		@ BANKCON5
	.long	0x00018005		@ BANKCON6
	.long	0x00018005		@ BANKCON7
	.long	0x008C07A3		@ REFRESH
	.long	0x000000B1		@ BANKSIZE
	.long	0x00000030		@ MRSRB6
	.long	0x00000030		@ MRSRB7

