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
#include <linux/proc_fs.h>

#define KER_RW_R8		0
#define KER_RW_R16      1
#define KER_RW_R32      2

#define KER_RW_W8       3
#define KER_RW_W16      4
#define KER_RW_W32      5

static int major ;
static struct class *clss;
static struct class_device *register_dev;

static int register_ioctl (struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	volatile unsigned char *p8;
	volatile unsigned short *p16;
	volatile unsigned int  *p32;

	unsigned int addr;
	unsigned int val;

	unsigned int buf[2];

	if(copy_from_user(buf,(const void __user *)arg,8))
		return -EFAULT;

	addr = buf[0];
	val  = buf[1];

	p8 = (volatile unsigned char *)ioremap(addr,4);
	p16 = p8;
	p32 = p16;

	switch (cmd)
	{
		case KER_RW_R8:
		{
			val = *p8;
			if (copy_to_user((void __user *)(arg+4),&val,4))
				return -EFAULT;

			break;
		}
		case KER_RW_R16:
		{
			val = *p16;
			if (copy_to_user((void __user *)(arg+4),&val,4))
				return -EFAULT;
			
			break;
		}
		case KER_RW_R32:
		{
			val = *p32;
			if (copy_to_user((void __user *)(arg+4),&val,4))
				return -EFAULT;
			
			break;
		}
		

		case KER_RW_W8:
		{	
			*p8 = val;
			
			break;
		}

		case KER_RW_W16:
		{
			*p16 = val;
			break;
		}
		case KER_RW_W32:
		{
			*p32 = val;
			break;
		}

	}
	iounmap(p8);

	return 0;
}


const struct file_operations register_fops = {
	.owner = THIS_MODULE,
	.ioctl = register_ioctl,
};


static int __init register_rw_init(void)
{
	printk("--- %s ---\r\n",__func__);

	major = register_chrdev(0,"register_rw",&register_fops);

	clss = class_create(THIS_MODULE,"register_rw");
	register_dev = class_device_create(clss,NULL,MKDEV(major,0),NULL,"register_rw");

	return 0;
}

static void register_rw_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	class_device_unregister(register_dev);
	class_destroy(clss);

	unregister_chrdev(major,"register_rw");
		
}

module_init(register_rw_init);
module_exit(register_rw_exit);

MODULE_LICENSE("GPL");


