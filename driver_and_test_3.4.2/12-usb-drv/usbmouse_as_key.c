#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>


static struct input_dev *usbmouse_dev;
static int len;
static char *usb_buf;
static dma_addr_t usb_buf_phys;
static struct urb *uk_urb;

static void usbmouse_as_key_irq(struct urb *urb)
{
#if 0
	int i;
	static int cnt = 0;
	printk("data cnt %d: ", ++cnt);
	for (i = 0; i < len; i++)
	{
		printk("%02x ", usb_buf[i]);
	}
	printk("\n");
#endif
	static unsigned char pre_val;

	if((pre_val&(1<<0)) != (usb_buf[0]&(1<<0))){
		input_event(usbmouse_dev,EV_KEY,KEY_L,(usb_buf[0]&(1<<0))?1:0);
		input_sync(usbmouse_dev);
	}

	if((pre_val&(1<<1)) != (usb_buf[0]&(1<<1))){
		input_event(usbmouse_dev,EV_KEY,KEY_S,(usb_buf[0]&(1<<1))?1:0);
		input_sync(usbmouse_dev);
	}

	if((pre_val&(1<<2)) != (usb_buf[0]&(1<<2))){
		input_event(usbmouse_dev,EV_KEY,KEY_ENTER,(usb_buf[0]&(1<<2))?1:0);
		input_sync(usbmouse_dev);
	}

	pre_val = usb_buf[0];


	/* USB鼠标数据含义
	 * data[0]: bit0-左键, 1-按下, 0-松开
	 *          bit1-右键, 1-按下, 0-松开
	 *          bit2-中键, 1-按下, 0-松开 
	 *
     */

	/* 重新提交urb */
	usb_submit_urb(uk_urb, GFP_KERNEL);
}


static int usbmouse_as_key_probe (struct usb_interface *intf,const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	int pipe;

	printk("--- %s ---\r\n",__func__);
	printk("found usbmouse\r\n");

	printk("bcdUSB = %x\r\n",dev->descriptor.bcdUSB);
	printk("VID    = 0x%x\r\n",dev->descriptor.idVendor);
	printk("PID    = 0x%x\r\n",dev->descriptor.idProduct);

	interface = intf->cur_altsetting;
	endpoint = &interface->endpoint[0].desc;

	usbmouse_dev = input_allocate_device();

	set_bit(EV_KEY,usbmouse_dev->evbit);
	set_bit(EV_REP,usbmouse_dev->evbit);

	set_bit(KEY_L,usbmouse_dev->keybit);
	set_bit(KEY_S,usbmouse_dev->keybit);
	set_bit(KEY_ENTER,usbmouse_dev->keybit);

	input_register_device(usbmouse_dev);

	/* 源: USB设备的某个端点 */
	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);

	//长度
	len = endpoint->wMaxPacketSize;

	//目标
	usb_buf = usb_alloc_coherent(dev,len,GFP_ATOMIC,&usb_buf_phys);

	//使用
	uk_urb = usb_alloc_urb(0,GFP_KERNEL);
	usb_fill_int_urb(uk_urb,dev,pipe,usb_buf,len,usbmouse_as_key_irq,NULL,endpoint->bInterval);
	uk_urb->transfer_dma = usb_buf_phys;
	uk_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	usb_submit_urb(uk_urb,GFP_KERNEL);
	
	return 0;
}

static void usbmouse_as_key_disconnect (struct usb_interface *intf)
{
	struct usb_device *dev = interface_to_usbdev(intf);

	printk("--- %s ---\r\n",__func__);	
	printk("disconnect usbmouse\r\n");
	usb_kill_urb(uk_urb);
	usb_free_urb(uk_urb);
	usb_free_coherent(dev,len,usb_buf,usb_buf_phys);
	
	input_unregister_device(usbmouse_dev);
	input_free_device(usbmouse_dev);
}

static struct usb_device_id usbmouse_as_key_id_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	/* Terminating entry */
};

static struct usb_driver usbmouse_as_key_driver = {
	.name		= "usbmouse_as_key",
	.probe		= usbmouse_as_key_probe,
	.disconnect	= usbmouse_as_key_disconnect,
	.id_table	= usbmouse_as_key_id_table,
};

static int __init usbmouse_as_key_init(void)
{
	printk("--- %s ---\r\n",__func__);

	usb_register(&usbmouse_as_key_driver);

	return 0;
}

static void __exit usbmouse_as_key_exit(void)
{
	printk("--- %s ---\r\n",__func__);
	usb_deregister(&usbmouse_as_key_driver);
}

module_init(usbmouse_as_key_init);
module_exit(usbmouse_as_key_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhuangzebin");
MODULE_DESCRIPTION("usbmouse as key test");



