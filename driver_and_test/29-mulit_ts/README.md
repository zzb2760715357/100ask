###多点触摸屏

输入子系统：实现了open、read、write等操作<br>
 input.c提供了fops等操作 ,根据次设备号找到对应的fops结构体 <br>
我们只需要实现硬件相关的代码 <br>
构造input_device,调用input_event上报数据 <br>
---
---
#### 单点触摸和多点触摸有什么区别

上报数据：ABS_X、ABS_Y等
内核文档：上报的数据分成AB两类.
SYN_MT_REPORT
SYN_REPORT
typeA:简单粗暴型，只上报触电位置，不关心触点之间的关系
应用程序需要根据上报的点，根据点与点之间距离分析，这些点分别属于那条划线。
typeB:复杂，上报触点位置，也上报触点关系
触控IC提供：触点位置、触点关系、同一个ID值属于同一个划线.
ABS_MT_SLOT
ABS_MT_TARCKING_ID 
ABS_MT_SLOT
ABS_MT_TRACKING_ID
ABS_MT_POSITION_X
ABS_MT_POSITION_Y 
![typeA](/home/zhuang/图片/选区_014.png) 
![typeB](/home/zhuang/图片/选区_013.png) 

####  多点触摸屏驱动编写框架

分配input_dev
设置能产生那类事件
设置能产生这类事件中的那些
设置这些事件的范围
注册input_dev
硬件相关的操作，申请中断
在中断中获取触点数据，i2c设备是慢速设备，不能在中断服务程序中使用，工作队列，或者中断下半部进行使用.


#### tiny4412原理图





