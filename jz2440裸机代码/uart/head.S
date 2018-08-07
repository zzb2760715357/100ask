.extern main
.text 
.global _start 
_start:
	ldr sp,=4096
	bl disable_watch_dog
	bl clock_init
	bl memsetup
	bl copy_steppingstone_to_sdram 
	ldr pc,=on_sdram
on_sdram:
	ldr sp,=0x34000000
	ldr lr,=halt_loop
	ldr pc,=main

halt_loop:
	b halt_loop
