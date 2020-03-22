KERN_DIR = /usr/src/linux-headers-2.6.31-14-generic

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m   += vivi.o
obj-m   += videobuf-core.o
obj-m   += videobuf-vmalloc.o
obj-m   += v4l2-common.o
