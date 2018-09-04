.text 
.global _start 
_start:
	ldr sp,=4096
	bl disable_watch_dog
	bl uart0_init
	mov r0,#0x6d  @输出m
	bl putc
	mov r0,#0x6e  @输出n
	bl putc
	bl memsetup
	mov r0,#0x6e  @输出n
	bl putc
	bl nand_init
	mov r0,#0x72  @输出r
	bl putc

	ldr r0,=0x30000000
	mov r1,#0
	mov r2,#4096
	bl nand_read

	mov r0,#0x6f   @输出o
	bl putc

	ldr sp,=0x34000000
	ldr lr,=halt_loop
	ldr pc,=main

halt_loop:
	b halt_loop
