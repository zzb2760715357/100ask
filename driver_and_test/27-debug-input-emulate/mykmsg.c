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

#define MYLOG_BUF_LEN (1024*1024)


static struct proc_dir_entry *entry;

static char *mylog_buf;
static char tmp_buf[1024];
static int  mylog_r = 0;
static int  mylog_r_for_read = 0;
static int  mylog_w = 0;

static DECLARE_WAIT_QUEUE_HEAD(mymsg_waitq);


static int is_mylog_empty(void)
{
	return (mylog_r == mylog_w);
}

static int is_mylog_empty_for_read(void)
{
	return (mylog_r_for_read == mylog_w);
}


static int is_mylog_full(void)
{
	if ((mylog_w + 1) % MYLOG_BUF_LEN == mylog_r){
		return 1;
	}else{
		return 0;
	}
}

static void mylog_putc(char c)
{
	if (is_mylog_full()){
		//¶ªµôÒ»¸öÊî¼Ù
		mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;

		if ((mylog_r_for_read + 1)%MYLOG_BUF_LEN == mylog_r)
			mylog_r_for_read = mylog_r;
	}
	
	mylog_buf[mylog_w] = c;
	mylog_w = (mylog_w + 1) % MYLOG_BUF_LEN;

	wake_up_interruptible(&mymsg_waitq);
}

static int mylog_getc(char *p)
{
	if (is_mylog_empty()){
		return 0;
	}
	*p = mylog_buf[mylog_r];
	mylog_r = (mylog_r + 1) % MYLOG_BUF_LEN;

	return 1;
}

static int mylog_getc_for_read(char *p)
{
	if (is_mylog_empty_for_read()){
		return 0;
	}
	*p = mylog_buf[mylog_r_for_read];
	mylog_r_for_read = (mylog_r_for_read + 1) % MYLOG_BUF_LEN;

	return 1;
}


int myprintk(const char *fmt, ...)
{
	va_list args;
	int i;
	int j;

	va_start(args, fmt);
	i = vsnprintf(tmp_buf, INT_MAX, fmt, args);
	va_end(args);
	
	for (j = 0; j < i; j++)
		mylog_putc(tmp_buf[j]);
		
	return i;
}



ssize_t mykmsg_read (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	int error = -1;
	int i = 0;
	char c;
	

	if ((file->f_flags & O_NONBLOCK) && is_mylog_empty_for_read())
		return -EAGAIN;
	
	error = wait_event_interruptible(mymsg_waitq,!is_mylog_empty_for_read());
	
	while (!error && (mylog_getc_for_read(&c)) && i < count) {
			error = __put_user(c,buf);
			buf++;
			i++;
	}

	if (!error)
		error = i;

	return error;

}

static int mykmsg_open (struct inode *inode, struct file *file)
{
	mylog_r_for_read = mylog_r;
	
	return 0;
}


static const struct file_operations proc_fops = {
	.open = mykmsg_open,
	.read = mykmsg_read,
};

static int __init mykmsg_init(void)
{
	printk("-- %s --\r\n",__func__);

	mylog_buf = kmalloc(MYLOG_BUF_LEN,GFP_KERNEL);
	if (!mylog_buf){
		printk("can't alloc for mylog buf\r\n");
		return -EIO;
	}

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

	kfree(mylog_buf);
}

module_init(mykmsg_init);
module_exit(mykmsg_exit);

EXPORT_SYMBOL(myprintk);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("my kmsg");



