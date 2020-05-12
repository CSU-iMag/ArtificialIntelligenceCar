智能车主控代码
==========
| 文件夹 | 说明 |
| - | - |
| ai | 调用NNCU控制舵机|
| bsp | 与主板相关的硬件|
| car | 全局变量 *Car* |
| control | 车速与方向控制|
| gui | OLED与按键交互|
| utility | 平台无关设施|
# 中文编码
只有 *car/car_config.h*, *gui/gui.cpp*, *gui/gui.hpp* 三个文件是 **GB2312**，其余文件均为 **UTF-8**. 

用`写字库.c`替换逐飞Flash例程，将中文字库加载到片上Flash。
# TODO
> 屏幕、串口、ADC改成DMA…

**car_config.h**一定要按格式写！！

*注意：* **Car** 全局只有一个！！！

原则上全局变量都放Car里。
# 资源分配
**GPT1** 给逐飞systick用。 
 **GPT2** 给软件定时器用。

中文字库用倒数45个flash扇区;
倒数第46个扇区存调试参数
# 说明书
[doxygen文档](https://github.com/CSU-iMag/ArtificialIntelligenceCar/blob/master/Libraries/doc/html.zip)
