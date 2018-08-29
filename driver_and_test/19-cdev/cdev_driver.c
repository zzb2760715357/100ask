#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>



#define HELLO_CNT 2


static int major;
static struct cdev hello_cdev;
static struct cdev hello2_cdev;
static struct class *cls;

static int hello_open (struct inode *inode, struct file *filep)
{
	printk("--- %s --- \r\n",__func__);

	return 0;
}


const struct file_operations hello_fops = {
	.open = hello_open,
};

static int hello2_open (struct inode *inode, struct file *filep)
{
	printk("--- %s --- \r\n",__func__);

	return 0;
}

const struct file_operations hello2_fops = {
	.open = hello2_open,
};


static int __init hello_init(void)
{
	dev_t devid;
	printk("--- %s ---\r\n",__func__);

	if (major){
		devid = MKDEV(major,0);
		register_chrdev_region(devid,HELLO_CNT,"hello");
	}else {
		alloc_chrdev_region(&devid,0,HELLO_CNT,"hello");
		major = MAJOR(devid);
	}

	cdev_init(&hello_cdev,&hello_fops);
	cdev_add(&hello_cdev,devid,HELLO_CNT);

	devid = MKDEV(major,2);
	register_chrdev_region(devid,1,"hello2");
	cdev_init(&hello2_cdev,&hello2_fops);
	cdev_add(&hello2_cdev,devid,1);



	cls = class_create(THIS_MODULE,"hello");
	class_device_create(cls,NULL,MKDEV(major,0),NULL,"hello0");
	class_device_create(cls,NULL,MKDEV(major,1),NULL,"hello1");
	class_device_create(cls,NULL,MKDEV(major,2),NULL,"hello2");
	class_device_create(cls,NULL,MKDEV(major,3),NULL,"hello3");

	return 0;
}

static void __exit hello_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	class_device_destroy(cls,MKDEV(major,0));
	class_device_destroy(cls,MKDEV(major,1));
	class_device_destroy(cls,MKDEV(major,2));
	class_device_destroy(cls,MKDEV(major,3));
	class_destroy(cls);

	cdev_del(&hello2_cdev);	
	unregister_chrdev_region(MKDEV(major,2),1);
	
	cdev_del(&hello_cdev);
	unregister_chrdev_region(MKDEV(major,0),HELLO_CNT);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("cdev driver test");



