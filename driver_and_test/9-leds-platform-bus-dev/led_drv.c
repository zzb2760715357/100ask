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
#include <linux/input.h>
#include <linux/platform_device.h>


volatile unsigned long *gpio_con = NULL;
volatile unsigned long *gpio_dat = NULL;
static int pin;

static unsigned int major;

static struct class *led_class;
static struct class_device *led_device;

static ssize_t led_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos)
{
	int val;
	printk("--- %s ---\r\n",__func__);

	if (copy_from_user(&val,buf,count))
		return -EFAULT;

	if(val == 1){
		*gpio_dat &= ~(1<<pin);
	}else{
		*gpio_dat |= (1<<pin);
	}

	return 0;

}

static int led_open (struct inode *inode, struct file *file)
{
	printk("--- %s ---\r\n",__func__);

	//配置LED的GPIO口的模式
	*gpio_con &= ~(0x3<<(pin<<2));
	*gpio_con |= (0x1<<(pin<<2));

	return 0;
}

static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.open  = led_open,
};


static int led_probe(struct platform_device *pdev)
{
	struct resource *res;

	printk("--- %s ---\r\n",__func__);

	//获取平台资源
	res = platform_get_resource(pdev,IORESOURCE_MEM,0);
	gpio_con = ioremap(res->start,res->end - res->start + 1);
	gpio_dat = gpio_con + 1;

	res = platform_get_resource(pdev,IORESOURCE_IRQ,0);
	pin = res->start;

	//注册字符设备
	major = register_chrdev(0,"myled1",&led_fops);

	led_class = class_create(THIS_MODULE,"myled");

	led_device = class_device_create(led_class,NULL,MKDEV(major,0),NULL,"led");
	

	return 0;
}

static int led_remove(struct platform_device *pdev)
{
	printk("--- %s ---\r\n",__func__);

	class_device_destroy(led_class,MKDEV(major,0));
	class_destroy(led_class);
	unregister_chrdev(major,"myled1");
	iounmap(gpio_con);

	return 0;			
}

static struct platform_driver led_drv = {
	.probe  = led_probe,
	.remove = led_remove,
	.driver = {
		.name = "myled",
	},
};


static int __init led_drv_init(void)
{
	printk("--- %s ---\r\n",__func__);

	platform_driver_register(&led_drv);


	return 0;	
}

static void __exit led_drv_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	platform_driver_unregister(&led_drv);
}

module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("led platform bus device test");



