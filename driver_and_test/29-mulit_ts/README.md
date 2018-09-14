###多点触摸屏
 输入子系统：实现了open、read、write等操作
 input.c提供了fops等操作 ,根据次设备号找到对应的fops结构体 
我们只需要实现硬件相关的代码 
构造input_device,调用input_event上报数据 
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




