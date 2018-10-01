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
#include <linux/ip.h>


static struct net_device *vir_net_dev; 

static void emulator_rx_packet(struct sk_buff *skb, struct net_device *dev)
{
	/* 参考LDD3 */
	unsigned char *type;
	struct iphdr *ih;
	__be32 *saddr, *daddr, tmp;
	unsigned char	tmp_dev_addr[ETH_ALEN];
	struct ethhdr *ethhdr;
	
	struct sk_buff *rx_skb;
		
	// 从硬件读出/保存数据
	/* 对调"源/目的"的mac地址 */
	ethhdr = (struct ethhdr *)skb->data;
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
	memcpy(ethhdr->h_dest, ethhdr->h_source, ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	/* 对调"源/目的"的ip地址 */    
	ih = (struct iphdr *)(skb->data + sizeof(struct ethhdr));
	saddr = &ih->saddr;
	daddr = &ih->daddr;

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;
	
	//((u8 *)saddr)[2] ^= 1; /* change the third octet (class C) */
	//((u8 *)daddr)[2] ^= 1;
	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	//printk("tx package type = %02x\n", *type);
	// 修改类型, 原来0x8表示ping
	*type = 0; /* 0表示reply */
	
	ih->check = 0;		   /* and rebuild the checksum (ip needs it) */
	ih->check = ip_fast_csum((unsigned char *)ih,ih->ihl);
	
	// 构造一个sk_buff
	rx_skb = dev_alloc_skb(skb->len + 2);
	skb_reserve(rx_skb, 2); /* align IP on 16B boundary */	
	memcpy(skb_put(rx_skb, skb->len), skb->data, skb->len);

	/* Write metadata, and then pass to the receive level */
	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY; /* don't check it */
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;

	// 提交sk_buff
	netif_rx(rx_skb);
}


static	int	virtual_net_send_packet(struct sk_buff *skb,struct net_device *dev)
{
	static int cnt = 0;

	printk("virtual_net_send_packet cnt = %d\r\n",++cnt);

	netif_stop_queue(dev);

	emulator_rx_packet(skb,dev);

	dev_kfree_skb(skb);
	netif_wake_queue(dev);
	


	//更新统计信息
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	return 0;
}
static const struct net_device_ops dm9000_netdev_ops = {
	.ndo_start_xmit		= virtual_net_send_packet,	
};

static int __init virtual_net_init(void)
{
	printk("--- %s ---\r\n",__func__);

	vir_net_dev = alloc_netdev(0, "vnet%d", ether_setup);

	//vir_net_dev->hard_start_xmit 	= virtual_net_send_packet;
	vir_net_dev->netdev_ops = &dm9000_netdev_ops;

	//设置mac地址
	vir_net_dev->dev_addr[0] = 0x08;
    vir_net_dev->dev_addr[1] = 0x89;
    vir_net_dev->dev_addr[2] = 0x89;
    vir_net_dev->dev_addr[3] = 0x89;
    vir_net_dev->dev_addr[4] = 0x89;
    vir_net_dev->dev_addr[5] = 0x89;

	vir_net_dev->flags           |= IFF_NOARP;
//	vir_net_dev->features        |= NETIF_F_NO_CSUM;	

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



