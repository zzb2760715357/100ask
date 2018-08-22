#include <linux/module.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>

#define RAMBLOCK_SIZE (1024*1024)


static struct gendisk *ramblock_disk;
static request_queue_t *ramblock_queue;
static int major;
static unsigned char *ramblock_buf;

static DEFINE_SPINLOCK(ramblock_lock);

static int ramblock_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	geo->heads = 2;
	geo->cylinders = 32;
	geo->sectors = RAMBLOCK_SIZE /2/32/512;

	return 0;
}


static void do_ramblock_request(request_queue_t *q)
{
	static int r_cnt = 0;
	static int w_cnt = 0;
	struct request *req;
	unsigned long offset;
	unsigned long len;
	
	//printk("%s:cnt = %d\r\n",__func__,cnt++);
	while ((req = elv_next_request(q)) != NULL) {
		offset = req->sector * 512;

		len = req->current_nr_sectors * 512;

		if (rq_data_dir(req) == READ){
			printk("do_ramblock_request read %d\r\n",r_cnt++);
			memcpy(req->buffer,ramblock_buf+offset,len);	
		}else {
			printk("do_ramblock_request write %d\r\n",w_cnt++);
			memcpy(ramblock_buf+offset,req->buffer,len);
		}
		
		end_request(req, 1);
	}
}


static struct block_device_operations ramblock_fops =
{
	.owner		= THIS_MODULE,
	.getgeo     = ramblock_getgeo,
};



static int __init ramblock_init(void)
{
	printk("--- %s ---\r\n",__func__);

	/* 1. 分配一个gendisk结构体 */
	ramblock_disk = alloc_disk(16);  //次设备号的个数，分区个数加1

	/* 2.1 分配/设置队列: 提供读写能力 */
	ramblock_queue = blk_init_queue(do_ramblock_request,&ramblock_lock);
	ramblock_disk->queue = ramblock_queue;

	//3. 设置其他属性
	major = register_blkdev(0,"ramblock");
	ramblock_disk->major = major;
    ramblock_disk->first_minor = 0;
    ramblock_disk->fops = &ramblock_fops;
    sprintf(ramblock_disk->disk_name, "ramblock");
	
	set_capacity(ramblock_disk,RAMBLOCK_SIZE/512);

	ramblock_buf = kzalloc(RAMBLOCK_SIZE,GFP_KERNEL);

	//注册
	add_disk(ramblock_disk);

	return 0;
}

static void __exit ramblock_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	
	unregister_blkdev(major,"ramblock");
	del_gendisk(ramblock_disk);
	put_disk(ramblock_disk);
	blk_cleanup_queue(ramblock_queue);
	kfree(ramblock_buf);
}

module_init(ramblock_init);
module_exit(ramblock_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("ramblock test driver");




