#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>


static int usbmouse_as_key_probe (struct usb_interface *intf,const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);

	printk("--- %s ---\r\n",__func__);
	printk("found usbmouse\r\n");

	printk("bcdUSB = %x\r\n",dev->descriptor.bcdUSB);
	printk("VID    = 0x%x\r\n",dev->descriptor.idVendor);
	printk("PID    = 0x%x\r\n",dev->descriptor.idProduct);

	return 0;
}

static void usbmouse_as_key_disconnect (struct usb_interface *intf)
{
	printk("--- %s ---\r\n",__func__);	
	printk("disconnect usbmouse\r\n");
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



