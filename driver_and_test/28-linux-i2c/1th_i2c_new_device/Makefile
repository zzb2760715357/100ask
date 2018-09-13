CC = arm-linux-gcc

#KERN_DIR  	 = /home/zhuang/work/project/6-jz2440/systems/linux-2.6.22.6
KERN_DIR     = /home/zhuang/work/project/6-jz2440/systems/linux-3.4.2

INSTALL_DIR  = /work/nfs_root/fileroot_3.4.2/mydriver

DRIVER_NAME  = at24cxx_dev
DRIVER_NAME2 = at24cxx_drv
#APP_NAME = input_replay

all:
	make -C $(KERN_DIR) M=`pwd` modules
#	$(CC) -o $(APP_NAME).app $(APP_NAME).c

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order Module.symvers
#	rm $(APP_NAME).app

install:
#	cp -raf *.ko /work/nfs_root/first_fs/mydriver
	cp -raf *.ko $(INSTALL_DIR)
#	cp $(APP_NAME).app /work/nfs_root/first_fs/mydriver

obj-m += $(DRIVER_NAME).o
obj-m += $(DRIVER_NAME2).o
