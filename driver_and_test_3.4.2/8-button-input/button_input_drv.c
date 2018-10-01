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

#include <mach/gpio.h>
#include <linux/sched.h>


static struct input_dev * button_dev;
static struct timer_list button_timer;
struct pin_desc{
	int irq;
	char *name;
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[4] = {
	{IRQ_EINT0, "s2",S3C2410_GPF(0),KEY_L},
	{IRQ_EINT2, "s3",S3C2410_GPF(2),KEY_S},
	{IRQ_EINT11,"s4",S3C2410_GPG(3),KEY_ENTER},
	{IRQ_EINT19,"s5",S3C2410_GPG(11),KEY_LEFTSHIFT},
};

static struct pin_desc *irq_pd;

static irqreturn_t button_irq(int irq, void *dev_id)
{
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&button_timer,jiffies+HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void button_timer_handler(unsigned long data)
{
	struct pin_desc * pindesc = irq_pd;
	unsigned int pinval;

	if (!pindesc)
		return ;

	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if (pinval){
		//松开
		input_event(button_dev,EV_KEY,pindesc->key_val,0);
		input_sync(button_dev);
	}else{
		//按下
		input_event(button_dev,EV_KEY,pindesc->key_val,1);
		input_sync(button_dev);
	}
}

static int __init button_input_drv_init(void)
{
	int i ;

	printk("--- %s ---\r\n",__func__);

	//1. 分配一个input_dev结构体
	button_dev = input_allocate_device();

	//2.1 能产生哪类事件
	set_bit(EV_KEY,button_dev->evbit);
	set_bit(EV_REP,button_dev->evbit);

	//2.2 能产生这类操作里的哪些事件: L,S,ENTER,LEFTSHIT
	set_bit(KEY_L,button_dev->keybit);
	set_bit(KEY_S,button_dev->keybit);
	set_bit(KEY_LEFTSHIFT,button_dev->keybit);
	set_bit(KEY_ENTER,button_dev->keybit);

	//3. 注册
	input_register_device(button_dev);

	//4. 硬件相关的操作
	init_timer(&button_timer);
	button_timer.function = button_timer_handler;
	add_timer(&button_timer);
	
	for (i = 0;i < 4;i ++){
		if(request_irq(pins_desc[i].irq,button_irq,IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING,pins_desc[i].name,&pins_desc[i])){
			printk("request irq : %d error \r\n",pins_desc[i].irq);
		}
	}

	return 0;
}

static void __exit button_input_drv_exit(void)
{
	int i ;

	printk("--- %s ---\r\n",__func__);
	for(i = 0;i < 4;i ++){
		free_irq(pins_desc[i].irq,&pins_desc[i]);
	}	
	
	del_timer(&button_timer);
	input_unregister_device(button_dev);
	input_free_device(button_dev);

	
		
}

module_init(button_input_drv_init);
module_exit(button_input_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("Buttons input driver test");

