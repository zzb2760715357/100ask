#include <linux/module.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/dma.h>


static struct net_device *vir_net_dev; 



static int __init virtual_net_init(void)
{
	printk("--- %s ---\r\n",__func__);

	vir_net_dev = alloc_netdev(0, "vnet%d", ether_setup);


	register_netdev(vir_net_dev);

	return 0;
}

static void __exit virtual_net_exit(void)
{
	printk("--- %s ---\r\n",__func__);

	unregister_netdev(vir_net_dev);
	free_netdev(vir_net_dev);
	
}


module_init(virtual_net_init);
module_exit(virtual_net_exit);

MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("virtual net driver");
MODULE_LICENSE("GPL");



