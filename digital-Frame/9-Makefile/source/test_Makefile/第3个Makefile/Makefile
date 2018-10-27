test:a.o b.o
	gcc -o test a.o b.o

a.o:a.c a.h

%.o : %.c 
	gcc -c -o $@ $<
