## 12课字符设备驱动基本概念
#### 应用程序open设备的时候在C库中通过swi指令陷入内核，从而找到对应的调用接口
![](https://i.imgur.com/wJ6MHn5.png)
![](https://i.imgur.com/xP4NdrO.png)
![](https://i.imgur.com/LwM1B3v.png)


---

## 第12课第2.1节 字符设备驱动程序之LED驱动程序_编写编译
### 应用程序在open的时候如何找到对应的驱动程序的open，在注册的时候调用register_chrdev把驱动程序的first_drv_fops 注册到内核之中，内核会定义一个数组用来管理这些字符设备驱动，open的时候VFS根据主设备号找到对应的驱动程序的open接口
![设备驱动框架](https://i.imgur.com/Ro35tge.png)

### 编写一个简单的驱动程序
###	cat /proc/devices 列出支持的设备,主设备号写0，系统会自动分配主设备号,register_chrdev只是把驱动程序注册进入内核，但是设备节点还需要自己手动的创建,我们可以利用mdev的机制来自动创建设备节点,mdev机制是根据在/sys目录下注册的设备信息来生成相关的设备节点和信息,class_create、class_device_create

### 驱动程序要操作LED的不能直接操作物理地址，要ioremap成虚拟地址
![](https://i.imgur.com/cgW5Jz4.png)
![](https://i.imgur.com/9YrLJnn.png) 改成/sbin/mdev 

### 相关代码
	#include <linux/module.h>
	#include <linux/kernel.h>
	#include <linux/fs.h>
	#include <linux/init.h>
	#include <linux/delay.h>
	#include <asm/uaccess.h>
	#include <asm/irq.h>
	#include <asm/io.h>
	#include <asm/arch/regs-gpio.h>
	#include <asm/hardware.h>
	
	#include <linux/proc_fs.h>
	#include <linux/poll.h>
	#include <linux/device.h>
	
	static int major;
	static struct class *firstdrv_class;
	static struct class_device *firstdrv_class_dev;
	
	volatile unsigned long *gpfcon = NULL;
	volatile unsigned long *gpfdat = NULL;
	
	static int first_drv_open (struct inode *inode, struct file *file)
	{
		/*配置gpio为输出状态*/
		printk("--- %s --- \r\n",__func__);
		*gpfcon &= ~((0x3<<(4*2))|(0x3<<(5*2))|(0x3<<(6*2))); 
		*gpfcon |= ((0x1<<(4*2))|(0x1<<(5*2))|(0x1<<(6*2)));
	
		return 0;
	}
	
	
	static ssize_t first_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *ppos)
	{
		int val;
		
		printk("--- %s --- \r\n",__func__);
		if (copy_from_user(&val,buf,size))
			return -EFAULT;
	
		printk("kernel:val = %d\r\n",val);
	
		if (val == 1){
			//点灯
			printk("--- %s:%d ---\r\n",__func__,__LINE__);
			*gpfdat &= ~((1<<4)|(1<<5)|(1<<6));
		}else{
			//灭灯
			printk("--- %s:%d ---\r\n",__func__,__LINE__);
			*gpfdat |= ((1<<4)|(1<<5)|(1<<6));
		}
	
		return 0;
	}
	
	
	static struct file_operations first_drv_fops = {
		.owner = THIS_MODULE,
		.open  = first_drv_open,
		.write = first_drv_write, 
	};
	
	
	static int __init first_drv_init(void)
	{
		printk("--- %s --- \r\n",__func__);
		//0自动分配设备号
		major = register_chrdev(0,"first_drv",&first_drv_fops);
	
		firstdrv_class = class_create(THIS_MODULE,"firstdrv");
	
		firstdrv_class_dev = class_device_create(firstdrv_class,NULL,MKDEV(major,0),NULL,"xxx");
	
		gpfcon =(volatile unsigned long *) ioremap(0x56000050,16);
		gpfdat = gpfcon + 1;
	
		return 0;
	}
	
	static void __exit first_drv_exit(void)
	{
		printk("--- %s --- \r\n",__func__);
		unregister_chrdev(111,"first_drv");
		class_device_unregister(firstdrv_class_dev);
		class_destroy(firstdrv_class);
		iounmap(gpfcon);
	
	}
	
	module_init(first_drv_init);
	module_exit(first_drv_exit);
	
	MODULE_LICENSE("GPL");
	MODULE_AUTHOR("zhuangzebin@hello.com");
	MODULE_DESCRIPTION("My first driver test");



---
### Makefile
	CC = arm-linux-gcc
  
	KERN_DIR  = /home/zhuang/work/project/6-jz2440/systems/linux-2.6.22.6
	DRIVER_NAME = first_drv
	APP_NAME = first_test
	
	all:
	    make -C $(KERN_DIR) M=`pwd` modules
	    $(CC) -o $(APP_NAME).app $(APP_NAME).c
	
	clean:
	    make -C $(KERN_DIR) M=`pwd` modules clean
	    rm -rf modules.order Module.symvers
	    rm $(APP_NAME).app
	
	install:
	    cp -raf *.ko /work/nfs_root/first_fs/mydriver
	    cp $(APP_NAME).app /work/nfs_root/first_fs/mydriver
	
	obj-m += $(DRIVER_NAME).o


----
### TestAPP
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <stdio.h>
	
	
	
	int main(int argc,char **argv)
	{
		int fd = -1;
		int val = -1;
	
		if (argc != 2){
			printf("usage:\r\n");
			printf("%s <on|off>\r\n",argv[0]);
			return 0;
		}
	
		fd = open("/dev/xxx",O_RDWR);
		if (fd < 0){
			printf("open /dev/xxx error \r\n");
			return -1;
		}
	
		if (strcmp(argv[1],"on") == 0){
			val = 1;
		}else  {
			val = 0;
		}
		
		fprintf(stderr,"APP:val = %d\r\n",val);
		
		write(fd,&val,4);
	
		return 0;	
	}


![](https://i.imgur.com/wm6kxFL.png)



	
	


