#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/mfd/88pm860x.h>
#include <linux/slab.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>




static int major;
static struct class *class;
static struct i2c_client *at24cxx_client;


static const struct i2c_device_id at24cxx_id_table[] = {
	{ "at24c08", 0 },
	{}
};

ssize_t at24cxx_read (struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	unsigned char addr,data;

	if (copy_from_user(&addr,buf,1))
		return -EFAULT;
	data = i2c_smbus_read_byte_data(at24cxx_client,addr);
	if (copy_to_user(buf,&data,1))
		return -EFAULT;

	return 1;
}
ssize_t at24cxx_write (struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	unsigned char ker_buf[2];
	unsigned char addr,data;

	if (copy_from_user(ker_buf,buf,2))
		return -EFAULT;
	addr = ker_buf[0];
	data = ker_buf[1];

	printk("addr = 0x%02x,data = 0x%02x\r\n",addr,data);

	if (!i2c_smbus_write_byte_data(at24cxx_client, addr, data))
		return 2;
	else
		return -EIO;

	
}


const struct file_operations at24cxx_fops = {
	.owner = THIS_MODULE,
	.read = at24cxx_read,
	.write = at24cxx_write ,
};


static int __devinit at24cxx_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("--- %s:%d ---\r\n",__func__,__LINE__);
	at24cxx_client = client;

	major = register_chrdev(0, "at24cxx", &at24cxx_fops);
	class = class_create(THIS_MODULE,"at24cxx");
	device_create(class, NULL, MKDEV(major,0), NULL, "at24cxx");

	return 0;
}
static int __devexit at24cxx_remove(struct i2c_client *client)
{
	printk("--- %s:%d ---\r\n",__func__,__LINE__);

	device_destroy(class, MKDEV(major,0));
	class_destroy(class);
	unregister_chrdev(major, "at24cxx");

	return 0;
}


static struct i2c_driver at24cxx_driver = {
	.driver	= {
		.name	= "100ask",
		.owner	= THIS_MODULE,
	},
	.probe		= at24cxx_probe,
	.remove		= __devexit_p(at24cxx_remove),
	.id_table	= at24cxx_id_table,
};


static int at24cxx_drv_init(void)
{
	i2c_add_driver(&at24cxx_driver);

	return 0;
}

static void at24cxx_drv_exit(void)
{
	i2c_del_driver(&at24cxx_driver);
}

module_init(at24cxx_drv_init);
module_exit(at24cxx_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("AT24CXX Driver");




