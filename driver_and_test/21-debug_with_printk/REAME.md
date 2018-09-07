### 一、通过往驱动程序不断的加入printk，通过折半查找法，定位是哪里出了问题.
### 通过printk定位到哪里有问题，查看 *gpfcon是这里出了问题，应该是访问了非法地址，查看 gpfcon这个值是怎么设置的
### 二、动态修改打印机级别
	cat /proc/sys/kernel/printk
	7	4	1	7
	echo "8 4 1 7 " >/proc/sys/kernel/printk是KERN_DEBUG也能够打印
### 去掉所有的打印信息

