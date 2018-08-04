objs := head.o init.o leds.o

mmu.bin : $(objs)
	arm-linux-ld -Tmmu.lds -o mmu_linux $^
	arm-linux-objcopy -O binary -S mmu_linux $@
	arm-linux-objdump -D -m arm mmu_linux > mmu.dis
	
%.o:%.c
	arm-linux-gcc -Wall -O2 -c -o $@ $<

%.o:%.S
	arm-linux-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f mmu.bin mmu_linux mmu.dis *.o		
	
