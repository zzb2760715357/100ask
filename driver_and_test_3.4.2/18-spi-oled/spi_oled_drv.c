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

#include <linux/spi/spi.h>
#include <linux/of_spi.h>

#include <mach/regs-gpio.h>
#include <mach/leds-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>




static int spi_oled_dc_pin;
struct spi_device *spi_oled_dev;
static unsigned char *ker_buf;
static int major;
static struct class *class;

static void OLED_Set_DC(char val)
{
	s3c2410_gpio_setpin(spi_oled_dc_pin, val);
}
static void OLEDWriteCmd(unsigned char cmd)
{
    OLED_Set_DC(0); /* command */

    spi_write(spi_oled_dev, &cmd, 1);

    OLED_Set_DC(1); /*  */
}

static void OLEDWriteDat(unsigned char dat)
{
    OLED_Set_DC(1); /* data */

    spi_write(spi_oled_dev, &dat, 1);
   
    OLED_Set_DC(1); /*  */
}

static void OLEDSetPageAddrMode(void)
{
    OLEDWriteCmd(0x20);
    OLEDWriteCmd(0x02);
}

static void OLEDSetPos(int page, int col)
{
    OLEDWriteCmd(0xB0 + page); /* page address */

    OLEDWriteCmd(col & 0xf);   /* Lower Column Start Address */
    OLEDWriteCmd(0x10 + (col >> 4));   /* Lower Higher Start Address */
}


static void OLEDClear(void)
{
    int page, i;
    for (page = 0; page < 8; page ++)
    {
        OLEDSetPos(page, 0);
        for (i = 0; i < 128; i++)
            OLEDWriteDat(0);
    }
}

static void OLEDClearPage(int page)
{
    int i;
    OLEDSetPos(page, 0);
    for (i = 0; i < 128; i++)
        OLEDWriteDat(0);    
}


static void OLEDInit(void)
{
    /* 向OLED发命令以初始化 */
    OLEDWriteCmd(0xAE); /*display off*/ 
    OLEDWriteCmd(0x00); /*set lower column address*/ 
    OLEDWriteCmd(0x10); /*set higher column address*/ 
    OLEDWriteCmd(0x40); /*set display start line*/ 
    OLEDWriteCmd(0xB0); /*set page address*/ 
    OLEDWriteCmd(0x81); /*contract control*/ 
    OLEDWriteCmd(0x66); /*128*/ 
    OLEDWriteCmd(0xA1); /*set segment remap*/ 
    OLEDWriteCmd(0xA6); /*normal / reverse*/ 
    OLEDWriteCmd(0xA8); /*multiplex ratio*/ 
    OLEDWriteCmd(0x3F); /*duty = 1/64*/ 
    OLEDWriteCmd(0xC8); /*Com scan direction*/ 
    OLEDWriteCmd(0xD3); /*set display offset*/ 
    OLEDWriteCmd(0x00); 
    OLEDWriteCmd(0xD5); /*set osc division*/ 
    OLEDWriteCmd(0x80); 
    OLEDWriteCmd(0xD9); /*set pre-charge period*/ 
    OLEDWriteCmd(0x1f); 
    OLEDWriteCmd(0xDA); /*set COM pins*/ 
    OLEDWriteCmd(0x12); 
    OLEDWriteCmd(0xdb); /*set vcomh*/ 
    OLEDWriteCmd(0x30); 
    OLEDWriteCmd(0x8d); /*set charge pump enable*/ 
    OLEDWriteCmd(0x14); 

    OLEDSetPageAddrMode();

    OLEDClear();
    
    OLEDWriteCmd(0xAF); /*display ON*/    
}


#define OLED_CMD_INIT       0x100001
#define OLED_CMD_CLEAR_ALL  0x100002
#define OLED_CMD_CLEAR_PAGE 0x100003
#define OLED_CMD_SET_POS    0x100004

static long oled_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	int page;
	int col;

	switch(cmd){
		case OLED_CMD_INIT:
		{
			OLEDInit();
			break;
		}
		case OLED_CMD_CLEAR_ALL:
		{
			OLEDClear();
			break;
		}
		case OLED_CMD_CLEAR_PAGE:
		{	
			page =arg;
			OLEDClearPage(page);
			break;
		}
		case OLED_CMD_SET_POS:
		{
			page = arg & 0xff;
			col = (arg >> 8)& 0xff;
			OLEDSetPos(page,col);
			break;
		}
	}

	return 0;
}

static ssize_t oled_write (struct file *file,  char __user *buf, size_t count, loff_t * ppos)
{
	int ret;

	if (count > 4096)
		return -EINVAL;
	ret = copy_from_user(ker_buf, buf, count);
	OLED_Set_DC(1);
	spi_write(spi_oled_dev, ker_buf, count);

	return 0;
}


static const struct file_operations oled_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = oled_ioctl,
	.write = oled_write,
};

static int __devinit oled_spi_probe(struct spi_device *spi)
{
	spi_oled_dev = spi;
	spi_oled_dc_pin = (int)spi->dev.platform_data;

	s3c2410_gpio_cfgpin(spi_oled_dc_pin, S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(spi->chip_select,S3C2410_GPIO_OUTPUT);

	ker_buf = kmalloc(4096,GFP_KERNEL);

	major = register_chrdev(0, "oled", &oled_fops);
	class = class_create(THIS_MODULE,"oled");

	device_create(class,NULL,MKDEV(major,0),NULL,"oled");

	return 0;
}


static int __devexit oled_spi_remove(struct spi_device *spi)
{
	device_destroy(class, MKDEV(major,0));
	class_destroy(class);
	unregister_chrdev(major, "oled");

	kfree(ker_buf);

	  return 0;
}

static struct spi_driver spi_oled_drv = {
	.driver = {
			.name	= "oled",
			.owner	= THIS_MODULE,
		},
		.probe		= oled_spi_probe,
		.remove		= __devexit_p(oled_spi_remove),
};


static int __init spi_oled_init(void)
{
	return spi_register_driver(&spi_oled_drv);
}

static void __exit spi_oled_exit(void)
{
	spi_unregister_driver(&spi_oled_drv);	
}

module_init(spi_oled_init);
module_exit(spi_oled_exit);
MODULE_DESCRIPTION("OLED SPI Driver");
MODULE_AUTHOR("zhuangzebin");
MODULE_LICENSE("GPL");


