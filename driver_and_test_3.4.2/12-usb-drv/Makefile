CC = arm-linux-gcc

KERN_DIR  = /home/zhuang/project/3-jz2440/systems/linux-3.4.2
INSTALL_DIR = /work/nfs_root/fs_mini_mdev_new/mydriver
DRIVER_NAME = usbmouse_as_key
#APP_NAME =  buttons_fasync_test

all:
	make -C $(KERN_DIR) M=`pwd` modules
#	$(CC) -o $(APP_NAME).app $(APP_NAME).c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order Module.symvers
#	rm $(APP_NAME).app

install:
	cp -raf *.ko $(INSTALL_DIR)
#	cp $(APP_NAME).app $(INSTALL_DIR)

obj-m += $(DRIVER_NAME).o
