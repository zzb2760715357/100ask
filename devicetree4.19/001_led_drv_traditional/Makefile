#CC = arm-linux-gcc
CC = arm-linux-gnueabi-gcc 
APPCC = /usr/local/arm/4.3.2/bin/arm-linux-gcc 

KERN_DIR  = /home/zhuang/project/3-jz2440/systems/source_and_images/kernel/linux-4.19-rc3
DRIVER_NAME = led_drv
APP_NAME = led_test

all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(APPCC) -o $(APP_NAME).app $(APP_NAME).c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order Module.symvers
	rm $(APP_NAME).app

install:
	cp -raf *.ko /work/nfs_root/fs_mini_mdev_new/mydriver/
	cp $(APP_NAME).app /work/nfs_root/fs_mini_mdev_new/mydriver/

obj-m += $(DRIVER_NAME).o
