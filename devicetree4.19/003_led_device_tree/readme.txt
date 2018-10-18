001th 002th 是使用两种方法写的设备树文件jz2440.dts与对应的驱动程序led_drv.c
001th中的led_drv.c只支持001th的jz2440.dts
002th中的led_drv.c同时支持001th和002th中的jz2440.dts


前提:
请使用本视频中提供的u-boot, uImage, fs_mini_mdev_new.yaffs2
单板上必须烧写我们提供的上述文件。

实验方法:
1. 编译设备树文件jz2440.dts得到jz2440.dtb
   
   假设内核目录为: /work/system/linux-4.19-rc3/
   
   设置PATH使用新工具链:
   export  PATH=PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/work/system/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi/bin
   
   把jz2440.dts复制到内核目录:
   cp jz2440.dts  /work/system/linux-4.19-rc3/arch/arm/boot/dts
   
   编译设备树:
   cd /work/system/linux-4.19-rc3/
   make dtbs
   
   得到 /work/system/linux-4.19-rc3/arch/arm/boot/dts/jz2440.dtb
   
   把dtb文件复制到nfs目录, 假设为 /work/nfs_root

2. 使用jz2440.dtb启动内核

   启动板子进入u-boot :
   
   从NFS下载dtb: 
   nfs 32000000 192.168.1.123:/work/nfs_root/jz2440.dtb
   
   烧写到nand flash:
   nand erase device_tree; nand write.jffs2 32000000 device_tree
   
   重启单板


3. 编译led_drv.c

   设置PATH使用新工具链:
   export  PATH=PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/work/system/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi/bin

   cd 001th/driver  或  cd 002th/driver
   
   make  // 得到led_drv.ko
   
4. 编译app ledtest
   
   注意！注意！注意！注意！注意！注意！注意！注意！
   编译APP用的工具链不一样！！！！
   
   设置PATH使用新工具链:
   export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/local/arm/4.3.2/bin
   
   arm-linux-gcc -o ledtest ledtest.c


5. 测试
   在单板上通过nfs得到led_drv.ko和ledtest
   
   测试命令:
   insmod led_drv.ko
   ./ledtest on
   ./ledtest off
   
   
