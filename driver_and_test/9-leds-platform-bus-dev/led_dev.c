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


static void led_release(struct device * dev)
{
	printk("--- %s ---\r\n",__func__);
}

static struct resource	led_resource[] ={
	[0] = {
		.start = 0x56000050,
		.end = 0x56000050 + 8 -1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = 5,
		.end = 5,
		.flags = IORESOURCE_IRQ,
	},
};


static struct platform_device led_pdev = {
	.name = "myled",
	.id = -1,
	.num_resources = ARRAY_SIZE(led_resource),
	.resource = led_resource,
	.dev = {
		.release = led_release,
	},
};


static int __init led_dev_init(void)
{
	printk("--- %s ---\r\n",__func__);

	platform_device_register(&led_pdev);
	
	return 0;
}

static void __exit led_dev_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	platform_device_unregister(&led_pdev);		
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("led platform bus device test");


