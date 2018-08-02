
.equ MEM_CTL_BASE,       0x48000000
.equ SDRAM_BASE,         0x30000000

.text 
.global _start
_start:
	bl disable_watch_dog		@关闭看门狗
    bl memsetup                 @初始化存储控制器
    bl copy_steppingstone_to_sdram	@拷贝到sdram中执行
    ldr pc,=on_sdram
on_sdram:
    ldr sp,=0x34000000 
    bl main

halt_loop:
    b halt_loop

disable_watch_dog:
    mov r1,#0x53000000
    mov r2,#0x00
    str r2,[r1]
    mov pc,lr
 
copy_steppingstone_to_sdram:
    mov r1,#0
    ldr r2,=SDRAM_BASE
    mov r3,#4*1024
1:
    ldr r4,[r1],#4	@从0地址读取数据并且读取完之后加4
    str r4,[r2],#4	@将0地址的内容写入sdram当中，目的地址加4
    cmp r1,r3
    bne 1b
    mov pc,lr
    
    

memsetup:
    mov r1,#MEM_CTL_BASE	@13个存储控制器的起始地址
    adrl r2,mem_cfg_val     @配置这13个存储控制器的值
    add r3,r1,#52           @13*4=52这是13个寄存器的长度

1:
    ldr r4,[r2],#4
    str r4,[r1],#4
    cmp r1,r3
    bne 1b
    mov pc,lr

.align 4
mem_cfg_val:
    @ 存储控制器13个寄存器的设置值
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7    
