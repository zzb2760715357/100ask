#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/device.h>

static unsigned int major;
static struct class *buttons_class;
static struct device *buttons_class_devices;

volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

static ssize_t buttons_drv_read (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	unsigned char key_vals[4];
	int regval;


	if (count != sizeof(key_vals))
		return -EINVAL;

	regval = *gpfdat;
	key_vals[0] = (regval & (1 << 0))?1:0;
	key_vals[1] = (regval & (1 << 2))?1:0;

	regval = *gpgdat;
	key_vals[2] = (regval & (1 << 3))?1:0;
	key_vals[3] = (regval & (1 << 11))?1:0;
	
	if (copy_to_user(buf,key_vals,sizeof(key_vals)))
		return -EFAULT;

	return sizeof(key_vals);	
}

static int buttons_drv_open (struct inode *inode, struct file *file)
{
	printk("---  %s ---\r\n",__func__);
	
	*gpfcon &= ~((0x3<<(0*2))|(0x3<<(2*2)));
	*gpgcon &= ~((0x3<<(3*2))|(0x3<<(11*2)));

	return 0;	
}

static struct file_operations buttons_drv_fops = {
	.owner = THIS_MODULE,
	.open = buttons_drv_open,
	.read = buttons_drv_read,
};

static int __init buttons_drv_init(void)
{
	printk("---  %s ---\r\n",__func__);
	
	major = register_chrdev(0,"buttons_drv",&buttons_drv_fops);

	buttons_class = class_create(THIS_MODULE,"buttons_drv");
	buttons_class_devices = device_create(buttons_class,NULL,MKDEV(major,0),NULL,"buttons");

	gpfcon = (volatile unsigned long *)ioremap(0x56000050,16);
	gpfdat = gpfcon + 1;

	gpgcon = (volatile unsigned long *)ioremap(0x56000060,16);
	gpgdat = gpgcon + 1;

	return 0;
}

static void __exit buttons_drv_exit(void)
{
	printk("---  %s ---\r\n",__func__);
	
	unregister_chrdev(major,"buttons_drv");
	device_destroy(buttons_class,MKDEV(major,0));
	class_destroy(buttons_class);
	iounmap(gpfcon);
	iounmap(gpgcon);
}

module_init(buttons_drv_init);
module_exit(buttons_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("Buttons test");
