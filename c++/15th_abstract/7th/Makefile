human.bin:main.o libHuman.so
	g++ -o $@ $< -L./ -lHuman
%.o:%.cpp
	g++ -fPIC -c -o $@ $<

libHuman.so:Chinese.o Englishman.o  Human.o
	g++ -shared -o $@ $^
	
clean:
	rm *.o *.bin
