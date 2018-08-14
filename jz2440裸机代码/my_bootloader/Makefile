
CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

CFLAGS 		:= -Wall -O2
CPPFLAGS   	:= -nostdinc -fno-builtin

objs := start.o init.o boot.o

boot.bin: $(objs)
	${LD} -Tboot.lds -o boot_elf $^
	${OBJCOPY} -O binary -S boot_elf $@
	${OBJDUMP} -D -m arm boot_elf > boot.dis
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f boot.bin boot_elf boot.dis *.o
	
