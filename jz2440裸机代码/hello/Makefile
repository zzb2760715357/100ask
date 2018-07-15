main:main.o extern.o
	gcc -o main main.o extern.o

main.o:main.c
	gcc -o main.o -c main.c

extern.o:extern.c
	gcc -o extern.o -c extern.c

clean:
	rm *.o main
