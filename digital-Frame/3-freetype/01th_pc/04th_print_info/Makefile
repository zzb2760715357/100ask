CC = gcc

# gcc -o example1.bin example1.c -I /usr/local/include/freetype2/ -lfreetype -lm

example1.bin:example1.o
	$(CC) -finput-charset=GBK -fexec-charset=UTF-8 -o $@ $<  -I /usr/local/include/freetype2/ -lfreetype -lm 
%.o:%.c
	$(CC) -finput-charset=GBK -fexec-charset=UTF-8 -c -o $@ $<  -I /usr/local/include/freetype2/ -lfreetype -lm 

clean:
	rm *.o *.bin

install:
	cp *.bin  /work/nfs_root/fs_mini_mdev_new/mydriver/

