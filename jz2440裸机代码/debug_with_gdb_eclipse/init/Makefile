objs := init.o

all: $(objs)
	arm-linux-ld -Ttext 0x0000000 -o init_elf $^
	arm-linux-objcopy -O binary -S init_elf init.bin
	arm-linux-objdump -D -m arm init_elf > init.dis
	
%.o:%.c
	arm-linux-gcc -Wall -g -c -o $@ $<

%.o:%.S
	arm-linux-gcc -Wall -g -c -o $@ $<

clean:
	rm -f init.bin init_elf init.dis *.o		
	
