.
|---jz2440裸机代码
|   |---gpio
|   |   |---key_leds  按键控制LED
|   |   |---led_on    汇编点亮LED
|   |   |---led_on_c  C语言的方式点亮LED
|   |   |---leds      流水灯
|   |
|   |---hello         简单makefile编写
|   |
|   |---sdram         在sdram中运行程序
|   |
|   |---mmu           通过虚拟地址点亮LED
|   |
|   |---nand          nand实验
|   |
|   |---int           按键中断控制LED
|   |
|   |---timer         定时器反转LED状态
|   |
|   |---uart          串口实验
|   |
|   |---RTC           RTC实验
|   |
|   |---lcd_3.5_4.3   LCD显示控制
|   |
|   |---adc_ts        触摸屏和ADC实验
|   |
|   |---my_bootloader 自己编写的简单bootloader
|   |
|   |---i2c-at24c02   at24c02的读写程序
|
|
|
|---driver_and_test  		字符设备驱动相关的例子
|  |---1-first_drv   		LED设备驱动 
|  |
|  |---2-button-drv  		按键驱动
|  |
|  |---3-button-irq  		按键中断驱动
|  |
|  |---4-button-poll 		按键poll驱动
|  |
|  |---5-button-fasync 		异步通知驱动
|  |
|  |---6-button-atomic-sem-NONBLOCK  原子操作、信号量、阻塞与非阻塞实验
|  |
|  |---7-button-timer  		添加定时器消除抖动
|  |
|  |---8-button-input  		输入子系统上报按键值
|  |
|  |---9-leds-platform-bus-dev  平台设备驱动模型
|  |
|  |---10-lcd        		LCD驱动程序
|  |
|  |---11-ts         		触摸驱动程序
|  |
|  |---12-usb-drv         	usbmouse as key driver
|  |
|  |---13-ramblock        	内存模拟硬盘实验
|  |
|  |---14-nand            	nandflash 驱动程序
|  |
|  |---15-norflash        	norflash  驱动程序
|  |
|  |---16-dm9000c         	DM9000网卡驱动
|  |
|  |---17-virtual-net     	添加虚拟网卡驱动程序
|  |
|  |---18-at24c02-drv       AT24C02的I2C驱动程序
|  |
|  |---19-cdev              cdev driver test
