key_leds.bin:crt0.S key_leds.c
	arm-linux-gcc -g -c -o crt0.o crt0.S
	arm-linux-gcc -g -c -o key_leds.o key_leds.c
	arm-linux-ld -Ttext 0x00000000 -g crt0.o key_leds.o -o key_leds_elf
	arm-linux-objcopy -O binary -S key_leds_elf key_leds.bin
	arm-linux-objdump -D -m arm key_leds_elf > key_leds.dis
clean:
	rm *.o *.bin *.dis key_leds_elf
