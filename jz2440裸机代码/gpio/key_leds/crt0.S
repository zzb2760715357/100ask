.text
.global _start
_start:

	ldr R0,=0x53000000     @ 设置为看门狗寄存器
	
	mov R1,#0x0
	str R1,[R0]

	ldr SP,=1024*4         @ 设置栈指针
	
	bl main
halt_loop:
	b halt_loop
