CC = arm-linux-gcc

# gcc -o example1.bin example1.c -I /usr/local/include/freetype2/ -lfreetype -lm

show_font.bin:show_font.o
	$(CC) -finput-charset=GBK -fexec-charset=GBK -o $@ $<  -lfreetype -lm 
%.o:%.c
	$(CC) -finput-charset=GBK -fexec-charset=GBK -c -o $@ $<  -lfreetype -lm 

clean:
	rm *.o *.bin

install:
	cp *.bin  *.ttc  /work/nfs_root/fs_mini_mdev_new/mydriver/

