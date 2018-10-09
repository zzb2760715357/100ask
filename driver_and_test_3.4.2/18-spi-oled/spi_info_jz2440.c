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


static struct spi_board_info __initdata jive_spi_jz2440[] = {
	[0] = {
		.modalias	= "oled",
		.bus_num	= 1,
		.chip_select	= S3C2410_GPF(1),
		.mode		= SPI_MODE_0,	/* CPOL=1, CPHA=1 */
		.max_speed_hz	= 10000000,
		.platform_data	= (const void *)S3C2410_GPG(4),
	}, {
		.modalias	= "spi_flash",
		.bus_num	= 1,
		.chip_select	= S3C2410_GPG(2),
		.mode		= SPI_MODE_0,	/* CPOL=0, CPHA=0 */
		.max_speed_hz	= 80000000,
	},
};


static int __init spi_info_jz2440_init(void)
{
	return spi_register_board_info(jive_spi_jz2440,ARRAY_SIZE(jive_spi_jz2440));
}

module_init(spi_info_jz2440_init);
MODULE_DESCRIPTION("OLED SPI Driver");
MODULE_AUTHOR("zhuangzebin");
MODULE_LICENSE("GPL");







