CC = arm-linux-gcc

KERN_DIR  = /home/zhuang/work/project/6-jz2440/systems/linux-2.6.22.6
DRIVER_NAME = s3c_ts
#APP_NAME = buttons_fasync_test

all:
	make -C $(KERN_DIR) M=`pwd` modules
#	$(CC) -o $(APP_NAME).app $(APP_NAME).c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order Module.symvers
#	rm $(APP_NAME).app

install:
	cp -raf *.ko /work/nfs_root/first_fs/mydriver
#	cp $(APP_NAME).app /work/nfs_root/first_fs/mydriver

obj-m += $(DRIVER_NAME).o
