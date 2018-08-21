#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/plat-s3c24xx/ts.h>

#include <asm/arch/regs-adc.h>
#include <asm/arch/regs-gpio.h>



static struct input_dev *s3c_ts_dev;

struct s3c_ts_regs{
	unsigned long adccon;
	unsigned long adctsc;
	unsigned long adcdly;
	unsigned long adcdat0;
	unsigned long adcdat1;
	unsigned long adcupdn;
};

static volatile struct s3c_ts_regs *s3c_ts_regs;


static void enter_wait_pen_down_mode(void)
{
	s3c_ts_regs->adctsc =  0xd3;
}

static void enter_wait_pen_up_mode(void)
{
	s3c_ts_regs->adctsc =  0x1d3;
}

static irqreturn_t pen_down_up_irq(int irq, void *dev_id)
{
	if (s3c_ts_regs->adcdat0 & (1<<15)){
		printk("pen up\r\n");	
		enter_wait_pen_down_mode();
	}else {
		printk("pen down\r\n");
		enter_wait_pen_up_mode();
	}

	return IRQ_HANDLED;
}

static int __init s3c_ts_init(void)
{
	struct clk *clk;

	printk("--- %s ---\r\n",__func__);

	// 分配一个input_dev结构体
	s3c_ts_dev = input_allocate_device();

	//设置能产生哪类事件
	set_bit(EV_KEY,s3c_ts_dev->evbit);
	set_bit(EV_ABS,s3c_ts_dev->evbit);

	//这类事件里的哪类事件
	set_bit(BTN_TOUCH,s3c_ts_dev->keybit);

	input_set_abs_params(s3c_ts_dev,ABS_X,0,0x3ff,0,0);
	input_set_abs_params(s3c_ts_dev,ABS_Y,0,0x3ff,0,0);
	input_set_abs_params(s3c_ts_dev,ABS_PRESSURE,0,1,0,0);
	
	//注册input_dev结构体
	input_register_device(s3c_ts_dev);

	//硬件相关的操作
	//使能时钟
	clk = clk_get(NULL,"adc");
	clk_enable(clk);

	//设置ADC/TS寄存器
	s3c_ts_regs = ioremap(0x58000000,sizeof(struct s3c_ts_regs));

	/* bit[14]  : 1-A/D converter prescaler enable
	 * bit[13:6]: A/D converter prescaler value,
	 *            49, ADCCLK=PCLK/(49+1)=50MHz/(49+1)=1MHz
	 * bit[0]: A/D conversion starts by enable. 先设为0
	 */
	s3c_ts_regs->adccon = (1<<14)|(49<<6);

	if (request_irq(IRQ_TC,pen_down_up_irq,IRQF_SAMPLE_RANDOM,"ts_pen",NULL))
	{
		printk("request IRQ_TC irq error\r\n");
		iounmap(s3c_ts_regs);
		return -EIO;	
	}

	enter_wait_pen_down_mode();

	return 0;
}

static void __exit s3c_ts_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	free_irq(IRQ_TC,NULL);
	iounmap(s3c_ts_regs);
	input_unregister_device(s3c_ts_dev);
	input_free_device(s3c_ts_dev);
	
}

module_init(s3c_ts_init);
module_exit(s3c_ts_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("tp driver");



