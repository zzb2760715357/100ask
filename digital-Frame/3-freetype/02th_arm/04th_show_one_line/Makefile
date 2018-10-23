CC = arm-linux-gcc


show_lines.bin:show_lines.o
	$(CC) -finput-charset=GBK -fexec-charset=GBK -o $@ $<  -lfreetype -lm 
%.o:%.c
	$(CC) -finput-charset=GBK -fexec-charset=GBK -c -o $@ $<  -lfreetype -lm 

clean:
	rm *.o *.bin

install:
	cp *.bin  *.ttc  /work/nfs_root/fs_mini_mdev_new/mydriver/

