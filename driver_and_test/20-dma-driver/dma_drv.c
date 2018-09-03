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
#include <linux/cdev.h>
#include <linux/dma-mapping.h>


#define BUF_SIZE (512*1024)

#define DMA0_BASE_ADDR  0x4B000000
#define DMA1_BASE_ADDR  0x4B000040
#define DMA2_BASE_ADDR  0x4B000080
#define DMA3_BASE_ADDR  0x4B0000C0

#define MEM_CPY_NO_DMA 0
#define MEM_CPY_DMA    1

struct s3c_dma_regs {
	unsigned long disrc;
	unsigned long disrcc;
	unsigned long didst;
	unsigned long didstc;
	unsigned long dcon;
	unsigned long dstat;
	unsigned long dcsrc;
	unsigned long dcdst;
	unsigned long dmasktrig;
};


static int major = 0;
static struct class *cls;

static char *src;
static u32 src_phys;

static char *dst;
static u32 dst_phys;

static volatile struct s3c_dma_regs *dma_regs;
static int irq_num = 1;

static DECLARE_WAIT_QUEUE_HEAD(dma_wait);
static volatile int ev_dma;



static int s3c_dma_ioctl (struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	int i;

	memset(src,0xaa,BUF_SIZE);
	memset(dst,0x55,BUF_SIZE);

	switch (cmd)	
	{
		case MEM_CPY_NO_DMA:
		{
			for(i = 0;i < BUF_SIZE;i ++)
				dst[i] = src[i];

			if (memcpy(src,dst,BUF_SIZE) == 0){
				printk("MEM_CPY_NO_DMA OK\r\n");
			}else {
				printk("MEM_CPY_NO_DMA ERROR\r\n");
			}
			
			break;
		}

		case MEM_CPY_DMA:
		{
			dma_regs->disrc   = src_phys;
			dma_regs->disrcc  = (0<<1)|(0<<0);
			dma_regs->didst   = dst_phys;
			dma_regs->didstc  = (0<<2)|(0<<1)|(0<<0);
			dma_regs->dcon    = (1<<30)|(1<<29)|(0<<28)|(1<<27)|(0<<23)|(0<<20)|(BUF_SIZE<0);

			//Æô¶¯dma
			dma_regs->dmasktrig = (1<<1)|(1<<0);


			wait_event_interruptible(dma_wait,ev_dma);

			if (memcpy(src,dst,BUF_SIZE) == 0){
				printk("MEM_CPY_DMA OK\r\n");
			}else {
				printk("MEM_CPY_DMA ERROR\r\n");
			}
			
			break;
		}
	}

	return 0;
}


static const struct file_operations dma_fops = {
	.owner = THIS_MODULE,
	.ioctl = s3c_dma_ioctl,
};

static irqreturn_t s3c_dma_irq(int irq, void *devid)
{
	ev_dma = 1;

	wake_up_interruptible(&dma_wait);

	return IRQ_HANDLED;
}

static int __init s3c_dma_init(void)
{
	printk("--- %s ---\r\n",__func__);

	if(request_irq(IRQ_DMA3,s3c_dma_irq,0,"s3c_dma",&irq_num)){
		printk("can not request irq for dma\r\n");
		return -EBUSY;
	}

	src = dma_alloc_writecombine(NULL,BUF_SIZE,&src_phys,GFP_KERNEL);
	if (src == NULL){
		printk("can not alloc buffer for src");
		return -ENOMEM;
	}
	
	dst = dma_alloc_writecombine(NULL,BUF_SIZE,&dst_phys,GFP_KERNEL);
	if (dst == NULL){
		printk("can not alloc buffer for dst");
		dma_free_writecombine(NULL,BUF_SIZE,src,src_phys);
		return -ENOMEM;
	}

	
	major = register_chrdev(0,"s3c_dma",&dma_fops);
	cls = class_create(THIS_MODULE,"s3c_dma");
	class_device_create(cls,NULL,MKDEV(major,0),NULL,"dma");

	dma_regs = ioremap(DMA3_BASE_ADDR,BUF_SIZE);
	


	return 0;
}

static void __exit s3c_dma_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	iounmap(dma_regs);

	class_device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);
	unregister_chrdev(major,"s3c_dma");
	
	dma_free_writecombine(NULL,BUF_SIZE,dst,dst_phys);
	dma_free_writecombine(NULL,BUF_SIZE,src,src_phys);

	free_irq(IRQ_DMA3,&irq_num);
}


module_init(s3c_dma_init);
module_exit(s3c_dma_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dma driver test");
MODULE_AUTHOR("zhuangzebin");

