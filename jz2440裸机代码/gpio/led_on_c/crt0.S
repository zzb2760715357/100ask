.text
.global _start
_start:

	ldr R0,=0x53000000   @ 设置为看门狗寄存器地址
	
	mov R1,#0x0
	str R1,[R0] 

	ldr sp,=1024*4       @ 设置的sp范围不要超过4K

	bl main

halt_loop:
	b halt_loop

