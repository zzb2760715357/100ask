## linux的I2C驱动
### i2c基本协议
** 一、 i2c控制器下面可以挂载多个设备、通过设备地址进行区分  **
** 二、通信基本协议:start信号、设备地址、 在scl低电平的时候可以更改数据，在高电平的时候保持不变、7位表示设备地址，最后一位表示是读还是写操作 。ACK信号在第9个SCL将SDA拉低。**

### I2C框架
	APP
	---------------------------
	Driver:		提供操作函数open、read等操作方法
	----------------------------------
	总线驱动：
	核心层：	提供i2c操作函数
	适配器层:	提供硬件操作方法	
	-------------------------------------------
	
	i2c_bus_type、i2c_driver、i2_client设备链表

### 如何使用i2c框架
** 参考内核文档编写 **
** 注册i2c_client、i2c_driver、在proebe中register一个设备驱动 **

### 设备的构建4种方法
#### 方法一：Declare the I2C devices by bus number
```
88pm860x-i2c.c (drivers\mfd)	
先定义一个i2c_board_info,使用i2c_register_board_info注册进去
static struct i2c_board_info mini2440_i2c_devs[] __initdata = {
	{
		I2C_BOARD_INFO("24c08", 0x50),
		.platform_data = &at24c08,
	},
};

i2c_register_adapter->i2c_register_board_info(0, mini2440_i2c_devs,
				ARRAY_SIZE(mini2440_i2c_devs));//存放在__i2c_board_list这个链表中  
							
何时使用 i2c_scan_static_board_info-> i2c_new_device

使用限制:必须在i2c_register_adapter之前构建好i2c_board_info，不太适合动态的加载
			
```

### 方法二：Instantiate the devices explicitly
	在方法一的基础上直接使用:i2c_new_device来创建相关的i2c控制器
	i2c_new_probed_device：对已经识别出来的才会创建
	i2c_new_device 默认存在
### 方法三：Probe an I2C bus for certain devices
** 如果不知道i2c设备在那个适配器，可以采用这种方法  .，参考这个代码模仿   drivers/hwmon/lm90.c**
      	
	static struct i2c_driver lm90_driver = {
	.class		= I2C_CLASS_HWMON,//去哪一类I2C适配器查找能支持的设备
	.driver = {
		.name	= "lm90",
	},
	.probe		= lm90_probe,
	.remove		= lm90_remove,
	.alert		= lm90_alert,
	.id_table	= lm90_id,
	.detect		= lm90_detect,        //用这个函数来检测能否找到设备
	.address_list	= normal_i2c,  //这些设备的地址
	};
	去class表示的这一类i2c适配器，用detect函数来确定能否找到address_list里的设备 ，如果能找到就调用i2c_new_device来注册i2c_client,这会和i2c_driver的id_table比较 
	

### 方法四：Instantiate from user-space
** 从用户空间创建设备 **	
**  echo eeprom 0x50 > /sys/bus/i2c/devices/i2c-3/new_device **


### 可以从用户空间直接访问，参考内核文档,dev-interface




---
#### linux3.4.2内核暂时不支持挂载网络文件系统，需要自己移植网卡驱动
#### linux3.4.2的内核要移植好学完内核的移植在过来搞吧。
#### 编写总线驱动程序
** 去掉内核对i2c总线的配置 **
** 硬件相关的设置 **
** 分配设置一个i2c_adapter,i2c_add_adapter() **




	



























	


