.extern main
.text 
.global _start
_start:

b Reset

@ 未定义指令中止模式的向量地址
HandleUnder:
	b HandleUnder

@ 管理模式的向量地址
HandleSWI:
	b HandleSWI

@ 指令预取异常的向量地址
HanledPrefetchAbort:	
	b HanledPrefetchAbort

@ 数据访问异常的向量地址
HandleDataAbort:
	b HandleDataAbort

@ 保留
HandleNotUsed:
	b HandleNotUsed

@ 中断模式向量地址
	b HandleIRQ

@ 快速中断模式的向量地址
HandleFIQ:
	b HandleFIQ

Reset:
	ldr sp,=4096
	bl disable_watch_dog
    bl clock_init
	bl memsetup
	bl copy_steppingstone_to_sdram
	ldr pc,=on_sdram
on_sdram:
	msr cpsr_c,#0xd2 @ 进入中断模式，设置中断模式栈指针
	ldr sp,=4096
	
	msr cpsr_c,#0xdf @ 进入管理模式，设置管理模式栈指针
	ldr sp,=0x34000000

	bl init_led      @ 初始化LED的GPIO口
	bl timer0_init
    bl init_irq      @ 初始化INT
	msr cpsr_c,#0x5f
	
	ldr lr,=halt_loop
	ldr pc,=main

halt_loop:
	b halt_loop
	
HandleIRQ:
	sub lr,lr,#4
	stmdb sp!,{r0-r12,lr} @ 保存寄存器的值

	ldr lr,=int_return 
	ldr pc,=Timer0_Handle
	
int_return: 
	ldmia sp!,{r0-r12,pc}^ @ 中断返回, ^表示将spsr的值复制到cpsr
    






