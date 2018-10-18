
person9.bin: person9.o RefBase.o
	g++ -o $@ $^

%.o : %.cpp
	g++ -c -o $@ $< -I include

clean:
	rm -f *.o *.bin	
