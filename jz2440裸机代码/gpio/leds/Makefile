leds.bin:crt0.S leds.c 
	arm-linux-gcc -g -c -o crt0.o crt0.S
	arm-linux-gcc -g -c -o leds.o leds.c
	arm-linux-ld -Ttext 0x00000000 -g crt0.o leds.o -o leds_elf
	arm-linux-objcopy -O binary -S leds_elf leds.bin
	arm-linux-objdump -D -m arm leds_elf > leds.dis
clean:
	rm -f leds_elf leds.dis leds.bin *.o
