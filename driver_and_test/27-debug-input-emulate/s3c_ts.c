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
static struct timer_list ts_timer;

extern int myprintk(const char *fmt, ...);

static void s3c_ts_timer_function (unsigned long data);


static void enter_wait_pen_down_mode(void)
{
	s3c_ts_regs->adctsc =  0xd3;
}

static void enter_wait_pen_up_mode(void)
{
	s3c_ts_regs->adctsc =  0x1d3;
}
static void enter_measure_xy_mode(void)
{
	s3c_ts_regs->adctsc = (1<<3)|(1<<2);
}

static void start_adc(void)
{
	s3c_ts_regs->adccon |= (1<<0);
}

static void write_input_event_to_file(unsigned int time,unsigned int type,unsigned int code,int val)
{
	myprintk("0x%08x %x%08x %x%08 %d\r\n",time,type,code,val);
}

static irqreturn_t pen_down_up_irq(int irq, void *dev_id)
{
	if (s3c_ts_regs->adcdat0 & (1<<15)){
		//printk("pen up\r\n");	
		input_report_abs(s3c_ts_dev,ABS_PRESSURE,0);
		write_input_event_to_file(jiffies,EV_ABS,ABS_PRESSURE,0);
		
		input_report_key(s3c_ts_dev,BTN_TOUCH,0);
		write_input_event_to_file(jiffies,EV_KEY,BTN_TOUCH,0);
		
		input_sync(s3c_ts_dev);
		write_input_event_to_file(jiffies,EV_SYN, SYN_REPORT,0);
		
		enter_wait_pen_down_mode();
	}else {
		//printk("pen down\r\n");
		//enter_wait_pen_up_mode();
		enter_measure_xy_mode();
		start_adc();
	}

	return IRQ_HANDLED;
}

static int s3c_filter_ts(int x[],int y[])
{
#define ERR_LIMIT 10
	int avr_x,avr_y;
	int det_x,det_y;

	avr_x = (x[0]+x[1])/2;
	avr_y = (y[0]+y[1])/2;

	det_x = (x[2]>avr_x)?(x[2] - avr_x):(avr_x-x[2]);
	det_y = (y[2]>avr_y)?(y[2] - avr_y):(avr_y-y[2]);

	if ((det_x>ERR_LIMIT)||(det_y>ERR_LIMIT)){
		return 0;
	}

	
	avr_x = (x[1]+x[2])/2;
	avr_y = (y[1]+y[2])/2;

	det_x = (x[3]>avr_x)?(x[3] - avr_x):(avr_x-x[3]);
	det_y = (y[3]>avr_y)?(y[3] - avr_y):(avr_y-y[3]);

	if ((det_x>ERR_LIMIT)||(det_y>ERR_LIMIT)){
		return 0;
	}


	return 1;
}

static irqreturn_t adc_irq(int irq, void *dev_id)
{
	static int cnt = 0;
	static int x[4],y[4];
	int adcdata0,adcdata1;

	adcdata0 = s3c_ts_regs->adcdat0;
	adcdata1 = s3c_ts_regs->adcdat1;


	//优化触摸数据采集完成时候发现已经松开则舍弃这次操作
	if (s3c_ts_regs->adcdat0 & (1<<15)){
		//松开
		cnt = 0;
		input_report_abs(s3c_ts_dev,ABS_PRESSURE,0);
		write_input_event_to_file(jiffies,EV_ABS,ABS_PRESSURE,0);
		
		input_report_key(s3c_ts_dev,BTN_TOUCH,0);
		write_input_event_to_file(jiffies,EV_KEY,BTN_TOUCH,0);
		
		input_sync(s3c_ts_dev);
		write_input_event_to_file(jiffies,EV_SYN,SYN_REPORT,0);
		
		enter_wait_pen_up_mode();
	}else{
		x[cnt] = adcdata0 &0x3ff;
		y[cnt] = adcdata1 &0x3ff;
		cnt++;
		if (cnt == 4){
			if (s3c_filter_ts(x,y)){
				//printk("adc_irq cnt = %d\tx = %d\ty = %d\r\n",++cnt,(x[0]+x[1]+x[2]+x[3])/4,(y[0]+y[1]+y[2]+y[3])/4);	
				input_report_abs(s3c_ts_dev,ABS_X,(x[0]+x[1]+x[2]+x[3])/4);
				write_input_event_to_file(jiffies,EV_ABS,ABS_X,(x[0]+x[1]+x[2]+x[3])/4);
				
				input_report_abs(s3c_ts_dev,ABS_Y,(y[0]+y[1]+y[2]+y[3])/4);
				write_input_event_to_file(jiffies,EV_ABS,ABS_Y,(y[0]+y[1]+y[2]+y[3])/4);
				
				input_report_abs(s3c_ts_dev,ABS_PRESSURE,1);
				write_input_event_to_file(jiffies,EV_ABS,ABS_PRESSURE,1);
				
				input_report_key(s3c_ts_dev,BTN_TOUCH,1);
				write_input_event_to_file(jiffies, EV_KEY, BTN_TOUCH, 1);
				
				input_sync(s3c_ts_dev);
				write_input_event_to_file(jiffies, EV_SYN, SYN_REPORT,0);
				
			}
			enter_wait_pen_up_mode();
			cnt = 0;
			//启动定时器处理长按或滑动问题
			mod_timer(&ts_timer,jiffies+HZ/100);
		}else{
			enter_measure_xy_mode();
			start_adc();
		}
	}
	
	return IRQ_HANDLED;
}

static void s3c_ts_timer_function (unsigned long data)
{
	if (s3c_ts_regs->adcdat0 &(1<<15)){

		//松开
		input_report_abs(s3c_ts_dev,ABS_PRESSURE,0);
		write_input_event_to_file(jiffies,EV_ABS,ABS_PRESSURE,0);
		
		input_report_key(s3c_ts_dev,BTN_TOUCH,0);
		write_input_event_to_file(jiffies,EV_KEY,BTN_TOUCH,0);
		
		input_sync(s3c_ts_dev);
		write_input_event_to_file(jiffies,EV_SYN, SYN_REPORT,0);
		
		enter_wait_pen_down_mode();
	}else{
		enter_measure_xy_mode();
		start_adc();
	}
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

	if (request_irq(IRQ_ADC,adc_irq,IRQF_SAMPLE_RANDOM,"adc",NULL)){
		printk("request IRQ_ADC irq error\r\n");
		free_irq(IRQ_TC,NULL);
		iounmap(s3c_ts_regs);
		return -EIO;
	}

	//等待电压稳定之后再采集
	s3c_ts_regs->adcdly = 0xffff;

	//添加定时器处理长按滑动问题
	init_timer(&ts_timer);
	ts_timer.function = s3c_ts_timer_function;
	add_timer(&ts_timer);

	enter_wait_pen_down_mode();

	return 0;
}

static void __exit s3c_ts_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	del_timer(&ts_timer);
	free_irq(IRQ_ADC,NULL);
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



