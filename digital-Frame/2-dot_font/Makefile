CC = arm-linux-gcc

show_font.bin:show_font.o
	$(CC) -o $@ $< 
%.o:%.c
	$(CC) -c -o $@ $<

clean:
	rm *.o *.bin

install:
	cp *.bin  /work/nfs_root/fs_mini_mdev_new/mydriver/

