#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


#define   I2C_AT24C02        0xa0


/*
 * Generic i2c probe
 * concerning the addresses: i2c wants 7 bit (without the r/w bit), so '>>1'
 */
static unsigned short normal_i2c[] = {
	I2C_AT24C02 >> 1,
	I2C_CLIENT_END,
};

static unsigned short ignore = I2C_CLIENT_END;

static struct i2c_client_address_data addr_data = {
	.normal_i2c		= normal_i2c,
	.probe			= &ignore,
	.ignore			= &ignore,
};


static int at24c02_detect (struct i2c_adapter *adapter, int address, int kind)
{
	printk("--- %s ---\r\n",__func__);
		
}

static int at24c02_attach_adapter(struct i2c_adapter *adapter)
{	
	printk("--- %s ---\r\n",__func__);

	return i2c_probe(adapter, &addr_data, at24c02_detect);
}

static int at24c02_detach_client(struct i2c_client *client)
{
	printk("--- %s ---\r\n",__func__);

	return 0;
}


static struct i2c_driver at24c02_driver = {
	.driver = {
		.name = "at24c02",
	},
	.attach_adapter = at24c02_attach_adapter,
	.detach_client  = at24c02_detach_client,
};

static int __init at24c02_init(void)
{
	printk("--- %s ---\r\n",__func__);
	i2c_add_driver(&at24c02_driver);

	return 0;
}

static void __exit at24c02_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	i2c_del_driver(&at24c02_driver);
}


module_init(at24c02_init);
module_exit(at24c02_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("at24c02 driver");

