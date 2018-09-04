.text 
.global _start 
_start:
	ldr sp,=4096
	bl disable_watch_dog
	bl memsetup	
	
	bl nand_init
	

	ldr r0,=0x30000000
	mov r1,#0
	mov r2,#4096
	bl nand_read
//	bl led_flicker

	ldr sp,=0x34000000
	ldr lr,=halt_loop
	ldr pc,=main

halt_loop:
	b halt_loop


led_flicker:
	//ÅäÖÃGPIO¿Ú
	ldr r0,=0x56000050
	ldr r1,=(1 << (4*2))
	str r1,[r0]

	ldr r0,=0x56000054
	ldr r1,=0
	ldr r2,=(1<<4)

loop:
	str r1,[r0]
	bl delay
	str r2,[r0]
	bl delay
	b loop

delay:
	ldr r3,=30000
1:
	sub r3,r3,#1
	cmp r3,#0
	bne 1b
	mov pc,lr

