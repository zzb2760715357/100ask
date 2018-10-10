#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/spi/s3c24xx.h>
#include <linux/module.h>

#include <plat/regs-spi.h>

#include <plat/fiq.h>
#include <asm/fiq.h>
#include <mach/regs-gpio.h>
#include <mach/leds-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>


static struct spi_master *spi0_controller;
static struct spi_master *spi1_controller;


struct s3c_spi_info {
	int irq;
	unsigned int reg_base;
	struct completion done;
	struct spi_transfer *cur_t;
	int cur_cnt;
	struct platform_device *pdev;
};

static int s3c2440_spi_setup(struct spi_device *spi)
{
	struct s3c_spi_info *info;
	struct clk *clk;

	int cpol  = 0;
	int cpha = 0;

	int div;

	info = spi_master_get_devdata(spi->master);
	clk = clk_get(NULL,"plck");

	if (spi->mode & 1){
		cpha = 1;
	}
	if (spi->mode & 2){
		cpol = 1;
	}

	writeb((1<<5)|(1<<4)|(1<<3)|(cpol <<2)|(cpha << 1),info->reg_base+S3C2410_SPCON);

	div = DIV_ROUND_UP(clk_get_rate(clk), spi->max_speed_hz * 2) - 1;
	clk_put(clk);

	if (div > 255)
		div = 255;

	writeb(div,info->reg_base+S3C2410_SPPRE);

	return 0;
}

static int s3c2440_spi_transfer(struct spi_device *spi,struct spi_message *mesg)
{
	struct spi_master *master = spi->master;
	struct s3c_spi_info *info;
	struct spi_transfer *t = NULL;

    info = spi_master_get_devdata(master);
    
    /* 1. 选中芯片 */
    s3c2410_gpio_setpin(spi->chip_select, 0);  /* 默认为低电平选中 */

	master->setup(spi);

	list_for_each_entry(t, &mesg->transfers, transfer_list){
		info->cur_cnt = 0;
		info->cur_t = t;
		init_completion(&info->done);

		if (t->tx_buf){
			writeb(((unsigned char *)t->tx_buf)[0],info->reg_base+S3C2410_SPTDAT);
			wait_for_completion(&info->done);
		}else if(t->rx_buf){
			writeb(0xff,info->reg_base+S3C2410_SPTDAT);
			wait_for_completion(&info->done);
		}
		
	}

	mesg->status = 0;
	mesg->complete(mesg->context);

    /* 3. 取消片选 */
    s3c2410_gpio_setpin(spi->chip_select, 1);  /* 默认为低电平选中 */
    
    return 0;
}

static irqreturn_t s3c2440_spi_irq(int irq, void *dev_id)
{
    struct spi_master *master = (struct spi_master *)dev_id;
    struct s3c_spi_info *info = spi_master_get_devdata(master);
    struct spi_transfer *t = info->cur_t;

    if (!t)
    {
        /* 误触发 */
    	return IRQ_HANDLED;            
    }

    /* 处理 spi_transfer */

    if (t->tx_buf) /* 是发送 */
    {
        info->cur_cnt++;
        
        if (info->cur_cnt < t->len)/* 没发完? */
            writeb(((unsigned char *)t->tx_buf)[info->cur_cnt], info->reg_base + S3C2410_SPTDAT);        
        else
            complete(&info->done); /* 唤醒 */
    }
    else /* 接收 */
    {
        /* 读/存数据 */
        ((unsigned char *)t->rx_buf)[info->cur_cnt] = readb(info->reg_base + S3C2410_SPRDAT);
        info->cur_cnt++;
        
        if (info->cur_cnt < t->len)/* 没收完? */
            writeb(0xff, info->reg_base + S3C2410_SPTDAT);        
        else
            complete(&info->done); /* 唤醒 */
    }
	return IRQ_HANDLED;    
}

static void s3c2440_spi_control_init(int which)
{
	struct clk *clk = clk_get(NULL,"spi");

	clk_enable(clk);
	clk_put(clk);

	if (which == 0){
		s3c2410_gpio_cfgpin(S3C2410_GPE(11), S3C2410_GPE11_SPIMISO0);
		s3c2410_gpio_cfgpin(S3C2410_GPE(12), S3C2410_GPE12_SPIMOSI0);
		s3c2410_gpio_cfgpin(S3C2410_GPE(13), S3C2410_GPE13_SPICLK0);
	}else if (which == 1){
		s3c2410_gpio_cfgpin(S3C2410_GPG(5), S3C2410_GPG5_SPIMISO1);
		s3c2410_gpio_cfgpin(S3C2410_GPG(6), S3C2410_GPG6_SPIMOSI1);
		s3c2410_gpio_cfgpin(S3C2410_GPG(7), S3C2410_GPG7_SPICLK1);
	}
}

static struct spi_master *create_spi_master_s3c2440(int bus_num,unsigned int reg_base_phy,int irq)
{
	int ret;
	struct spi_master *master;
	struct s3c_spi_info *info;
	struct platform_device *pdev;


	pdev = platform_device_alloc("s3c2440_spi", bus_num);
	platform_device_add(pdev);

    master = spi_alloc_master(&pdev->dev, sizeof(struct s3c_spi_info));
    master->bus_num = bus_num;
    master->num_chipselect = 0xffff;
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
	
	master->setup  = s3c2440_spi_setup;
	master->transfer = s3c2440_spi_transfer;

	info = spi_master_get_devdata(master);
	info->reg_base = (unsigned int )ioremap(reg_base_phy, 0x18);
	info->irq = irq;
	info->pdev = pdev;

	s3c2440_spi_control_init(bus_num);

	ret = request_irq(irq, s3c2440_spi_irq, 0, "s3c2440_spi", master);

	spi_register_master(master);

	return master;
}

static void destroy_spi_master_s3c2440(struct spi_master *master)
{
	struct s3c_spi_info *info = spi_master_get_devdata(master);

	spi_unregister_master(master);
	platform_device_del(info->pdev);
	platform_device_put(info->pdev);
	free_irq(info->irq, master);
	iounmap((void *)info->reg_base);


}
static int __init spi_s3c2440_init(void)
{
	spi0_controller = create_spi_master_s3c2440(0, 0x59000000, IRQ_SPI0);
	spi1_controller = create_spi_master_s3c2440(1, 0x59000020, IRQ_SPI1);

    return 0;
}

static void __exit spi_s3c2440_exit(void)
{
	destroy_spi_master_s3c2440(spi0_controller);
	destroy_spi_master_s3c2440(spi1_controller);
}


module_init(spi_s3c2440_init);
module_exit(spi_s3c2440_exit);
MODULE_DESCRIPTION("FLASH SPI Driver");
MODULE_AUTHOR("zhuangzebin");
MODULE_LICENSE("GPL");

