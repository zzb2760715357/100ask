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

static unsigned short force_addr[] = {ANY_I2C_BUS,0x60,I2C_CLIENT_END};
static unsigned short *forces[] = {force_addr,NULL};

static struct i2c_client_address_data addr_data = {
	.normal_i2c		= normal_i2c,
	.probe			= &ignore,
	.ignore			= &ignore,
	//.forces         = forces,
};

static struct i2c_driver at24c02_driver;
static int major;
static struct class *cls;
static struct class_device *cls_dev;
static struct i2c_client *at24c02_client;

static ssize_t at24c02_read (struct file *filep, char __user * buf, size_t count, loff_t *ppos)
{
	unsigned char address;
	unsigned char data;
	struct i2c_msg msg[2];
	int ret;

	if (count !=1)
		return -EINVAL;

	if (copy_from_user(&address,buf,1))
		return -EFAULT;

	//读的地址
	msg[0].addr = at24c02_client->addr;
	msg[0].buf = &address;
	msg[0].len = 1;
	msg[0].flags = 0;

	//启动读操作
	msg[1].addr = at24c02_client->addr;
	msg[1].buf = &data;
	msg[1].len = 1;
	msg[1].flags = I2C_M_RD;

	ret = i2c_transfer(at24c02_client->adapter,msg,2);
	if (ret == 2){
		if (copy_to_user(buf,&data,1))
			return -EFAULT;
		return 1;
	}else {
		return -EIO;
	};
}
static ssize_t at24c02_write (struct file *filep, const char __user * buf, size_t count, loff_t *ppos)
{
	unsigned char val[2];
	struct i2c_msg msg[1];
	int ret;

	if (count !=2)
		return -EINVAL;

	if (copy_from_user(val,buf,2))
		return -EFAULT;

	msg[0].addr = at24c02_client->addr;
	msg[0].buf = val;
	msg[0].len = 2;
	msg[0].flags = 0;

	ret = i2c_transfer(at24c02_client->adapter,msg,1);
	if (ret == 1){
		return 2;
	}else {
		return -EIO;
	}
}


const struct file_operations at24c02_fops = {
	.owner = THIS_MODULE,
	.read  = at24c02_read,
	.write = at24c02_write,
};


static int at24c02_detect (struct i2c_adapter *adapter, int address, int kind)
{
	printk("--- %s ---\r\n",__func__);

	at24c02_client = kzalloc(sizeof(struct i2c_client),GFP_KERNEL);
	at24c02_client->addr		= address;
	at24c02_client->adapter     = adapter;
	at24c02_client->driver      = &at24c02_driver;
	strcpy(at24c02_client->name,"at24cxx");
	i2c_attach_client(at24c02_client);

	major = register_chrdev(0,"at24cxx",&at24c02_fops);

	cls = class_create(THIS_MODULE,"at24cxx");
	cls_dev = class_device_create(cls,NULL,MKDEV(major,0),NULL,"at24cxx");

	return 0;
}

static int at24c02_attach_adapter(struct i2c_adapter *adapter)
{	
	printk("--- %s ---\r\n",__func__);

	return i2c_probe(adapter, &addr_data, at24c02_detect);
}

static int at24c02_detach_client(struct i2c_client *client)
{
	printk("--- %s ---\r\n",__func__);

	class_device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);
	unregister_chrdev(major,"at24cxx");

	i2c_detach_client(client);
	kfree(i2c_get_clientdata(client));

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

