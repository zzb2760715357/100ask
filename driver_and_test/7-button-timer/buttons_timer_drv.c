
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

static unsigned int major;
static struct class *buttons_class;
static struct class_device *buttons_class_devices;
static struct fasync_struct *buttons_async;
static struct timer_list button_timer;


volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

static struct pin_desc *irq_pd;

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
static volatile int ev_press = 0;
//static atomic_t openflag = ATOMIC_INIT(1);
static DECLARE_MUTEX(button_lock); 


static unsigned char key_val;

/* 键值: 按下时, 0x01, 0x02, 0x03, 0x04 */
/* 键值: 松开时, 0x81, 0x82, 0x83, 0x84 */
static struct pin_desc pins_desc[4] = {
	{S3C2410_GPF0, 0x01},
	{S3C2410_GPF2, 0x02},
	{S3C2410_GPG3, 0x03},
	{S3C2410_GPG11,0x04},
};

static ssize_t buttons_fasync_drv_read (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	if (count != 1)
		return -EINVAL;

	if (file->f_flags & O_NONBLOCK){
		if(!ev_press)
			return -EAGAIN;
	}else {
		wait_event_interruptible(button_waitq,ev_press);
	}

	if (copy_to_user(buf,&key_val,1))
		return -EFAULT;

	ev_press = 0;

	return 1;	
}

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&button_timer,jiffies + HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static int buttons_fasync_drv_open (struct inode *inode, struct file *file)
{
	printk("---  %s ---\r\n",__func__);

	#if 0
	// 使用原子操作限制只能打开一次设备
	if (!atomic_dec_and_test(&openflag)){
		atomic_inc(&openflag);
		return -EBUSY;
	}
	#endif

	if (file->f_flags & O_NONBLOCK){
		if(down_trylock(&button_lock))
			return -EBUSY;
	}else {
		down(&button_lock);
	}

	
	
	request_irq(IRQ_EINT0,  buttons_irq, IRQT_BOTHEDGE, "S2", &pins_desc[0]);
	request_irq(IRQ_EINT2,  buttons_irq, IRQT_BOTHEDGE, "S3", &pins_desc[1]);
	request_irq(IRQ_EINT11, buttons_irq, IRQT_BOTHEDGE, "S4", &pins_desc[2]);
	request_irq(IRQ_EINT19, buttons_irq, IRQT_BOTHEDGE, "S5", &pins_desc[3]);	

	return 0;	
}

int buttons_fasync_drv_close (struct inode *inode, struct file *file)
{
	printk("---  %s ---\r\n",__func__);

	//atomic_inc(&openflag);

	free_irq(IRQ_EINT0, &pins_desc[0]);
	free_irq(IRQ_EINT2, &pins_desc[1]);
	free_irq(IRQ_EINT11, &pins_desc[2]);
	free_irq(IRQ_EINT19, &pins_desc[3]);

	up(&button_lock);
	
	return 0;
}


static unsigned int buttons_fasync_drv_poll (struct file *file, struct poll_table_struct *wait)
//static unsigned buttons_fasync_drv_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	
	//printk("---  %s ---\r\n",__func__);

	poll_wait(file,&button_waitq,wait);

	if(ev_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static int buttons_fasync_drv_fasync (int fd, struct file * filp, int on)
{
	printk("--- %s ---\r\n",__func__);

	return fasync_helper(fd,filp,on,&buttons_async);
}

static struct file_operations buttons_fasync_drv_fops = {
	.owner = THIS_MODULE,
	.open = buttons_fasync_drv_open,
	.read = buttons_fasync_drv_read,
	.release = buttons_fasync_drv_close,
	.poll = buttons_fasync_drv_poll,
	.fasync = buttons_fasync_drv_fasync,
};

static void button_timer_handler(unsigned long data)
{
	struct pin_desc * pindesc = irq_pd;
	unsigned int pinval;

	if (!pindesc)
		return ;

	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if (pinval){
		//松开
		key_val = 0x80 | pindesc->key_val;	
	}else{
		//按下
		key_val = pindesc->key_val;
	}

	ev_press = 1;
	wake_up_interruptible(&button_waitq);
	kill_fasync(&buttons_async,SIGIO,POLLIN);

}

static int __init buttons_fasync_drv_init(void)
{
	printk("---  %s ---\r\n",__func__);
	init_timer(&button_timer);
	button_timer.function = button_timer_handler;
	add_timer(&button_timer);
	
	major = register_chrdev(0,"buttons_fasync_drv",&buttons_fasync_drv_fops);

	buttons_class = class_create(THIS_MODULE,"buttons_fasync_drv");
	buttons_class_devices = class_device_create(buttons_class,NULL,MKDEV(major,0),NULL,"buttons_fasync");

	gpfcon = (volatile unsigned long *)ioremap(0x56000050,16);
	gpfdat = gpfcon + 1;

	gpgcon = (volatile unsigned long *)ioremap(0x56000060,16);
	gpgdat = gpgcon + 1;

	return 0;
}

static void __exit buttons_fasync_drv_exit(void)
{
	printk("---  %s ---\r\n",__func__);
	
	unregister_chrdev(major,"buttons_fasync_drv");
	class_device_unregister(buttons_class_devices);
	class_destroy(buttons_class);
	iounmap(gpfcon);
	iounmap(gpgcon);

	del_timer(&button_timer);

}

module_init(buttons_fasync_drv_init);
module_exit(buttons_fasync_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("Buttons test");
