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

#include <linux/mtd/cfi.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>



static struct spi_device *spi_flash;
static struct mtd_info spi_flash_dev;

static void SPIFlashReadID(int *pMID, int *pDID)
{
	unsigned char tx_buf[4];
	unsigned char rx_buf[4];

	tx_buf[0] = 0x90;
	tx_buf[1] = 0;
	tx_buf[2] = 0;
	tx_buf[3] = 0;

	spi_write_then_read(spi_flash, tx_buf,4, rx_buf, 2);

	*pMID =  rx_buf[0];
	*pDID =  rx_buf[1];


}

static void SPIFlashWriteEnable(int enable)
{
	unsigned char val = enable ? 0x06:0x04;

	spi_write(spi_flash,&val,1);
}

static unsigned char SPIFlashReadStatusReg1(void)
{
    unsigned char val;
    unsigned char cmd = 0x05;
  
    spi_write_then_read(spi_flash, &cmd, 1, &val, 1);
  
    return val;
}

static unsigned char SPIFlashReadStatusReg2(void)
{
   unsigned char val;
    unsigned char cmd = 0x35;
  
    spi_write_then_read(spi_flash, &cmd, 1, &val, 1);
  
    return val;
}

static void SPIFlashWaitWhenBusy(void)
{
    while (SPIFlashReadStatusReg1() & 1)
    {
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(HZ/100);
    }
}

static void SPIFlashWriteStatusReg(unsigned char reg1, unsigned char reg2)
{    
	unsigned char tx_buf[3];

    SPIFlashWriteEnable(1);  

	tx_buf[0] = 0x01;
	tx_buf[1] = reg1;
	tx_buf[2] = reg2;

	spi_write(spi_flash, tx_buf, 3);

    SPIFlashWaitWhenBusy();
}

static void SPIFlashClearProtectForStatusReg(void)
{
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(1<<7);
    reg2 &= ~(1<<0);

    SPIFlashWriteStatusReg(reg1, reg2);
}


static void SPIFlashClearProtectForData(void)
{
    /* cmp=0,bp2,1,0=0b000 */
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(7<<2);
    reg2 &= ~(1<<6);

    SPIFlashWriteStatusReg(reg1, reg2);
}

/* erase 4K */
void SPIFlashEraseSector(unsigned int addr)
{
	unsigned char tx_buf[4];

	tx_buf[0] = 0x20;
	tx_buf[1] = addr >> 16;
	tx_buf[2] = addr >> 8;
	tx_buf[3] = addr & 0xff;

    SPIFlashWriteEnable(1);  

	spi_write(spi_flash, tx_buf, 4);

    SPIFlashWaitWhenBusy();
}

/* program */
void SPIFlashProgram(unsigned int addr, unsigned char *buf, int len)
{
	unsigned char tx_buf[4];

	struct spi_transfer t[] = {
		{
			.tx_buf 	= tx_buf,
			.len		= 4,
		},{
			.tx_buf 	= buf,
			.len		= len,
		},
	};
	struct spi_message	m;

	tx_buf[0] = 0x02;
	tx_buf[1] = addr >> 16;
	tx_buf[2] = addr >> 8;
	tx_buf[3] = addr & 0xff;
    
    SPIFlashWriteEnable(1);  

	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	spi_message_add_tail(&t[1], &m);
	spi_sync(spi_flash, &m);
    
    SPIFlashWaitWhenBusy();
    
}

void SPIFlashRead(unsigned int addr, unsigned char *buf, int len)
{
    /* spi_write_then_read规定了tx_cnt+rx_cnt < 32
     * 所以对于大量数据的读取，不能使用该函数
     */
     
    unsigned char tx_buf[4];   
	struct spi_transfer	t[] = {
            {
    			.tx_buf		= tx_buf,
    			.len		= 4,
        	},
			{
    			.rx_buf		= buf,
    			.len		= len,
			},
		};
	struct spi_message	m;

    tx_buf[0] = 0x03;
    tx_buf[1] = addr >> 16;
    tx_buf[2] = addr >> 8;
    tx_buf[3] = addr & 0xff;

	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	spi_message_add_tail(&t[1], &m);
	spi_sync(spi_flash, &m);    
}


void SPIFlashInit(void)
{
    SPIFlashClearProtectForStatusReg();
    SPIFlashClearProtectForData();
}


static int spi_flash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	unsigned int addr = instr->addr;
	unsigned int len = 0;

    /* 判断参数 */
    if ((addr & (spi_flash_dev.erasesize - 1)) || (instr->len & (spi_flash_dev.erasesize - 1)))
    {
        printk("addr/len is not aligned\n");
        return -EINVAL;
    }

	for (len = 0; len < instr->len;len += 4096){
		SPIFlashEraseSector(addr);
		addr += 4096;
	}


	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);
	return 0;
}

static int spi_flash_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
	SPIFlashRead(from, buf, len);
	*retlen = len;
	return 0;
}


static int spi_flash_write(struct mtd_info *mtd, loff_t to, size_t len,
	size_t *retlen, const u_char *buf)
{
	unsigned int addr = to;
	unsigned int wlen = 0;

    /* 判断参数 */
    if ((to & (spi_flash_dev.erasesize - 1)) || (len & (spi_flash_dev.erasesize - 1)))
    {
        printk("addr/len is not aligned\n");
        return -EINVAL;
    }

	for(wlen = 0;wlen <  len;wlen += 256){
		SPIFlashProgram(addr, (unsigned char *) buf,256);
		addr += 256;
		buf += 256;
	}
	

	*retlen = len;
	return 0;
}

static int __devinit flash_spi_probe(struct spi_device *spi)
{
	int mid,did;
	spi_flash = spi;

	memset(&spi_flash_dev,0,sizeof(spi_flash_dev));
	s3c2410_gpio_cfgpin(spi->chip_select, S3C2410_GPIO_OUTPUT);
	SPIFlashInit();
	SPIFlashReadID(&mid,&did);
	printk("SPI Flash ID:%02x %02x\r\n",mid,did);


	
	spi_flash_dev.name = "spi_flash";
	spi_flash_dev.type = MTD_NORFLASH;
	spi_flash_dev.flags = MTD_CAP_NORFLASH;
	spi_flash_dev.size = 0x200000;  /* 2M */
	spi_flash_dev.writesize = 1;
	spi_flash_dev.writebufsize = 4096; /* Mimic CFI NOR flashes */
	spi_flash_dev.erasesize = 4096;

	spi_flash_dev.owner = THIS_MODULE;
	spi_flash_dev._erase = spi_flash_erase;
	spi_flash_dev._read =spi_flash_read;
	spi_flash_dev._write = spi_flash_write;


	mtd_device_register(&spi_flash_dev, NULL, 0);

	return 0;
}


static int __devexit flash_spi_remove(struct spi_device *spi)
{
	mtd_device_unregister(&spi_flash_dev);

	return 0;
}

static struct spi_driver spi_oled_drv = {
	.driver = {
			.name	= "spi_flash",
			.owner	= THIS_MODULE,
		},
		.probe		= flash_spi_probe,
		.remove		= __devexit_p(flash_spi_remove),
};



static int __init spi_flash_init(void)
{
	return spi_register_driver(&spi_oled_drv);
}

static void __exit spi_flash_exit(void)
{
	spi_unregister_driver(&spi_oled_drv);	
}


module_init(spi_flash_init);
module_exit(spi_flash_exit);
MODULE_DESCRIPTION("FLASH SPI Driver");
MODULE_AUTHOR("zhuangzebin");
MODULE_LICENSE("GPL");


