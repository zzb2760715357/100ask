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

static void led_release(struct device * dev)
{
	printk("--- %s ---\r\n",__func__);
}

static struct resource	led_resource[] ={
	[0] = {
		.start = S3C2440_GPF(5),
		.end =S3C2440_GPF(5),
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


