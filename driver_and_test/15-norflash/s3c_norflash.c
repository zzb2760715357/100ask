#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <asm/io.h>

struct map_info *s3c_nor_map;
struct mtd_info *s3c_nor_mtd;

static struct mtd_partition s3c_nor_part[] = {
	[0] = {
        .name   = "bootloader_nor",
        .size   = 0x00040000,
		.offset	= 0,
	},
	[1] = {
        .name   = "root_nor",
        .offset = MTDPART_OFS_APPEND,
        .size   = MTDPART_SIZ_FULL,
	}
};


static int __init s3c_norflash_int(void)
{
	printk("--- %s ---\r\n",__func__);
	
	// 1. 分配map_info结构体
	s3c_nor_map = kzalloc(sizeof(struct map_info),GFP_KERNEL);

	// 2. 设置: 物理基地址(phys), 大小(size), 位宽(bankwidth), 虚拟基地址(virt)
	s3c_nor_map->name = "s3c_nor";
	s3c_nor_map->phys = 0;
	s3c_nor_map->size = 0x1000000;
	s3c_nor_map->bankwidth = 2;
	s3c_nor_map->virt = ioremap(s3c_nor_map->phys, s3c_nor_map->size);

	simple_map_init(s3c_nor_map);

	s3c_nor_mtd = do_map_probe("cfi_probe", s3c_nor_map);
	if (!s3c_nor_mtd){
		printk("use jedec probe\r\n");
		s3c_nor_mtd = do_map_probe("jedec_probe",s3c_nor_map);
	}

	if (!s3c_nor_mtd){
		iounmap(s3c_nor_map->virt);
		kfree(s3c_nor_map);
		return -EIO;
	}
	
	add_mtd_partitions(s3c_nor_mtd,s3c_nor_part,2);

	return 0;
}

static void __exit s3c_norflash_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	del_mtd_partitions(s3c_nor_mtd);
	iounmap(s3c_nor_map->virt);
	kfree(s3c_nor_map);
	
}

module_init(s3c_norflash_int);
module_exit(s3c_norflash_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("s3c norflash test");





