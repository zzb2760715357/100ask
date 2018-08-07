objs = head.o init.o serial.o main.o

serial.bin : $(objs)
	arm-linux-ld -Tserial.lds -o serial_elf $^
	arm-linux-objcopy -O binary -S serial_elf $@
	arm-linux-objdump -D -m arm serial_elf > serial.dis 

%.o:%.c
	arm-linux-gcc -Wall -c -O2 -o $@ $<
%.o:%.S
	arm-linux-gcc -Wall -c -O2 -o $@ $<

clean:
	rm *.o *.bin serial_elf serial.dis
