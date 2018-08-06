objs := head.o init.o interrupt.o  main.o

timer.bin : $(objs)
	arm-linux-ld -Ttimer.lds -o timer_elf $^
	arm-linux-objcopy -O binary -S timer_elf $@
	arm-linux-objdump -D -m arm timer_elf > timer.dis

%.o:%.c
	arm-linux-gcc -Wall -O2 -c -o $@ $<

%.o:%.S
	arm-linux-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f timer.bin *.o timer_elf timer.dis 
