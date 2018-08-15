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

static int first_drv_open (struct inode *inode, struct file *file)
{
	printk("--- %s --- \r\n",__func__);

	return 0;
}


static ssize_t first_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{
	printk("--- %s --- \r\n",__func__);

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

	return 0;
}

static void __exit first_drv_exit(void)
{
	printk("--- %s --- \r\n",__func__);
	unregister_chrdev(111,"first_drv");
	class_device_unregister(firstdrv_class_dev);
	class_destroy(firstdrv_class);

}

module_init(first_drv_init);
module_exit(first_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin@hello.com");
MODULE_DESCRIPTION("My first driver test");
