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
|   |
|   |---01th_spi_i2c_adc_jz2440_oled           裸机oled,sdram demo
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
|  |
|  |---18-spi-oled                   SPI接口的OLED,flash测试程序
|
|
|---devicetree4.19                               设备树相关
|  |
|  |---001_led_drv_traditional                   传统的点亮LED方法
|  |
|  |---002_led_dev_bus_drv                       dev bus dri bring up led
|  |
|  |---003_led_devicetree                        devicetree bring up led
|  |
|  |
|
|
|
|---c++                                          c++学习笔记
|  |
|  |---01th_oop                                  面向对象的基本编程
|  |
|  |---02th_accesscontrol                        访问控制以及this成员的使用
|  |
|  |---03th_ProgramStructure                     在类的外部定义函数,拆分函数,main.cpp只关心怎么调用、不同的类出现同名字的函数使用namespace、cout
|  |
|  |---04th_overload                             函数的重载
|  |
|  |---05th_point_reference                      指针和引用
|  |
|  |---06th_constructor                          构造函数、析构函数、默认的拷贝构造函数、构造函数与析构函数的构造顺序
|  |
|  |---07th_static                               静态成员函数以及变量的访问和使用
|  |
|  |---08th_friend                               友元函数的定义
|  |
|  |---09th_operator                             重载操作运算+、++p、p++、<<、=、
|  |
|  |---10th_inheritance                          继承、public、private、protect的访问控制、using改变子类继承父类的变量
|  |
|  |---11th_multi_inheritance                    多重继承、虚拟继承
|  |
|  |---12th_constructor_2                        多重继承构造顺序、
|  |
|  |---13th_Polymorphism                         多态、虚函数、析构函数一般为虚函数、
|  |
|  |---14th_convert                              类型的转换
|  |
|  |---15th_abstract                             抽象类和类编程
|  |
|  |---16th_template                             模板和重载，以及使用的顺序
|  |
|  |---17th_class_template                       类模板以及类的定做
|  |
|  |---18th_exception                            捕获异常
|  |
|  |---19th_smartpointer                         智能指针
|  |
|  |---20th_light_smartpointer                   轻量级智能指针保证count变量线程安全
|  |
|  |---21th_strongpointer_weakpointer            弱指针的使用
|  |
|  |---22th_singleton                            单例模式
|  |
|  |---23th_bridge_mode                          桥接模式
|
|
|
|
|
|---digital-Frame                                数码相框学习笔记
|  |
|  |---1-char-code                               字符编码






