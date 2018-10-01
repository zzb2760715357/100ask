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
|   |
|   |---debug_with_led      使用LED进行调试
|   |
|   |---debug_with_serial   使用串口进行调试
|   |
|   |---debug_with_jtag     使用jtag进行调试
|   |
|   |---debug_with_gdb_eclipse   使用gdb或eclipse进行源码级别的调试
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
|  |
|  |---20-dma-driver        dma驱动程序
|  |
|  |---21-debug_with_printk 通过printk定位出问题的代码
|  |
|  |---22-proc_mykmsg       mykmsg
|  |
|  |---23-debug_regeditor   自己制作的io debug tool 
|  |
|  |---24-test-debug        打印应用程序出错的信息
|  |
|  |---25-test-system-call  自己制作相关的系统调用
|  |
|  |---26-debug-with-system-call        使用自己的系统调用进行调试 
|  |
|  |---27-debug-input-emulate           输入模拟器
|  |
|  |---28-linux-i2c                     linux3.4.2内核的i2c驱动编程
|  |
|  |---29-mulit_ts                      多点触摸驱动（暂时没有添加相关代码）
|
|
|
|
|
|---driver_and_test_3.4.2               linux3.4.2版本的内核驱动
|  |  
|  |---1-dm9000                     
|  |
|  |---2-first_drv
|  |
|  |---2-button-drv
|  |
|  |---3-button-irq
|  |
|  |---4-button-poll
|  |
|  |---5-button-fasync
|  |
|  |---6-button-atomic-sem-NONBLOCK
|  |
|  |---7-button-timer
|  |
|  |---8-button-input
|  |
|  |---9-lcd
|  |
|  |---11-ts
|  |
|  |---12-usb-drv
|  |
|  |---13-ramblock
|  |
|  |---14-nand
|  |
|  |---15-norflash
|  |
|  |---17-virtual-net





