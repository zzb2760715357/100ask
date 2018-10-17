human.bin:main.o Chinese.o Englishman.o
	g++ -o $@ $^
%.o:%.cpp
	g++ -c -o $@ $<
clean:
	rm *.o *.bin
