main.bin:main.o person.o
	g++ -o $@ $^
%.o:%.c
	g++ -c -o $@ $<
clean:
	rm *.o *.bin
