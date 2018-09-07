#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/proc_fs.h>


static struct proc_dir_entry *entry;
static char mylog_buf[1024];

ssize_t mykmsg_read (struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	if (copy_to_user(buf,mylog_buf,10))
		return -EFAULT;

	return 10;	
}

static const struct file_operations proc_fops = {
	.read = mykmsg_read,
};

static int __init mykmsg_init(void)
{
	printk("-- %s --\r\n",__func__);

	sprintf(mylog_buf,"abcdefghijkobqrst");

	entry = create_proc_entry("mykmsg",S_IRUSR,&proc_root);
	if (entry){
		entry->proc_fops = &proc_fops;
	}

	return 0;
}

static void __exit mykmsg_exit(void)
{
	printk("-- %s --\r\n",__func__);

	remove_proc_entry("mykmsg",&proc_root);
}

module_init(mykmsg_init);
module_exit(mykmsg_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("my kmsg");



