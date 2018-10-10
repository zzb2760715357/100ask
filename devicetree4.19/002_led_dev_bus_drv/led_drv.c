#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/platform_device.h>

#include <linux/sched.h>



#define S3C2440_GPA(n)  (0<<16 | n)
#define S3C2440_GPB(n)  (1<<16 | n)
#define S3C2440_GPC(n)  (2<<16 | n)
#define S3C2440_GPD(n)  (3<<16 | n)
#define S3C2440_GPE(n)  (4<<16 | n)
#define S3C2440_GPF(n)  (5<<16 | n)
#define S3C2440_GPG(n)  (6<<16 | n)
#define S3C2440_GPH(n)  (7<<16 | n)
#define S3C2440_GPI(n)  (8<<16 | n)
#define S3C2440_GPJ(n)  (9<<16 | n)

static int led_pin = S3C2440_GPF(5);
static volatile unsigned int *gpio_con;
static volatile unsigned int *gpio_dat;


static int major;
static struct class *led_class;

static unsigned int gpio_base[] = {
        0x56000000, /* GPACON */
        0x56000010, /* GPBCON */
        0x56000020, /* GPCCON */
        0x56000030, /* GPDCON */
        0x56000040, /* GPECON */
        0x56000050, /* GPFCON */
        0x56000060, /* GPGCON */
        0x56000070, /* GPHCON */
        0,          /* GPICON */
        0x560000D0, /* GPJCON */
};



static int led_open(struct inode * node, struct file *filep)
{
	int bank = led_pin >> 16;
	int base = gpio_base[bank];

	int pin = led_pin & 0xffff;
	gpio_con = ioremap(base, 8);
	if (gpio_con){
		printk("ioremap(0x%x) = 0x%x\r\n",base,gpio_con);
	}else {
		return -EINVAL;
	}

	gpio_dat = gpio_con + 1;
	*gpio_con &= ~(3<<(pin * 2));
	*gpio_con |= (1<<(pin * 2));

	return 0;
}

static ssize_t led_write(struct file * filp, const char __user * buf, size_t count, loff_t * off)
{
	unsigned char val;
	int pin = led_pin & 0xffff;

	if (copy_from_user(&val, buf, 1))
		return -EFAULT;

	if (val){
		*gpio_dat &= ~(1<<pin);
	}else {
		*gpio_dat |= (1<<pin);
	}

	return 1;
	
}

static int led_release (struct inode * node, struct file *filp)
{
	iounmap(gpio_con);

	return 0;
}


static const struct file_operations myled_ops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.write = led_write,
	.release = led_release,
};

static int __init myled_init(void)
{
	major = register_chrdev(0,"myled", &myled_ops);

	led_class = class_create(THIS_MODULE, "myled");
	device_create(led_class, NULL, MKDEV(major,0), NULL, "led");

	return 0;
}

static void __exit myled_exit(void)
{
	unregister_chrdev(major, "myled");
	device_destroy(led_class,  MKDEV(major, 0));
	class_destroy(led_class);
		
}

module_init(myled_init);
module_exit(myled_exit);

MODULE_LICENSE("GPL");







