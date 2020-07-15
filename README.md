# :zap:电磁AI智能车
![GitHub language count](https://img.shields.io/github/languages/count/CSU-iMag/ArtificialIntelligenceCar)
![stars](https://img.shields.io/github/stars/CSU-iMag/ArtificialIntelligenceCar)
![GitHub repo size](https://img.shields.io/github/repo-size/CSU-iMag/ArtificialIntelligenceCar?style=plastic)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c6cbaef75e714a8387990f974cd1c991)](https://www.codacy.com/gh/CSU-iMag/ArtificialIntelligenceCar?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=CSU-iMag/ArtificialIntelligenceCar&amp;utm_campaign=Badge_Grade)

![GitHub last commit](https://img.shields.io/github/last-commit/CSU-iMag/ArtificialIntelligenceCar?style=for-the-badge)
![GitHub commits since latest release (by date) for a branch](https://img.shields.io/github/commits-since/CSU-iMag/ArtificialIntelligenceCar/v1.0?style=for-the-badge)

采集数据发送至上位机、运行训练好的模型
# :sparkles:FEATURE
- :rocket:纯电磁四轮
- :ambulance:基于状态机的**出轨、堵转**保护
- :art:1.3吋OLED与6个按键的*中文*GUI
- :boom:**一键**切换AI模式与PID模式！
- :card_file_box:可在*运行时*切换AI模型
- :wrench:在Keil中通过GUI配置全车参数
- :zap:PID采用CMSIS-DSP库实现
- :zap:兼顾效率与灵活性的滑动平均滤波器
- :lock:运行时错误检查：`CAR_ERROR_CHECK`
- :arrow_heading_up:*极简*[调度器](https://github.com/CSU-iMag/ArtificialIntelligenceCar/blob/master/Project/CODE/utility/scheduler.hpp)
- :clock3:基于*平衡二叉树*的[软件定时器](https://github.com/CSU-iMag/ArtificialIntelligenceCar/blob/master/Project/CODE/utility/timer.hpp)
- :battery:低电量自动关机
- :recycle:`std::set_new_handler`
# :eyes:PREVIEW
## 控制台
> 复位后打印的内容
![print](Pictures/print.png)
## 开始
> 图中柱子是电池电量，数字是“CPU占用率”
<img src="Pictures/IMG_20200511_130902.jpg" style="transform:rotate(180deg);">

## 主页
> 右上角数字代表“CPU占用率”的位数
<img src="Pictures/IMG_20200511_130913.jpg" style="transform:rotate(180deg);">

## 模型选择
<img src="Pictures/IMG_20200511_131122.jpg" style="transform:rotate(180deg);">

> 预存了3个model.nncu.c，可在运行时切换。
## 控制面板
<img src="Pictures/IMG_20200511_130925.jpg" style="transform:rotate(180deg);">
<img src="Pictures/IMG_20200511_130935.jpg" style="transform:rotate(180deg);">

## 电机设置
<img src="Pictures/IMG_20200511_131010.jpg" style="transform:rotate(180deg);">
<img src="Pictures/IMG_20200511_131000.jpg" style="transform:rotate(180deg);">

## 方向控制
<img src="Pictures/IMG_20200511_131032.jpg">

## 数字电位器
<img src="Pictures/IMG_20200511_131148.jpg" style="transform:rotate(180deg);">

## 电磁传感器
<img src="Pictures/IMG_20200511_205727.jpg">
<img src="Pictures/IMG_20200511_164132.jpg">

> 单击切换显示方式
## 显示波形
![g](Pictures/IMG_20200511_210009.jpg)
![u](Pictures/uper.png)
## 配置参数
![c](Pictures/config.png)
# :octocat:LIBRARY
> 致敬开源！
- [nncu](https://github.com/RockySong/nncu_tool)
- [SimpleGUI](https://gitee.com/Polarix/simplegui)
- [逐飞科技RT1064开源库](https://gitee.com/seekfree/RT1064_Library)
- [StateMachineCompact](https://online.visual-paradigm.com/w/twpaunrw/drive/#diagramlist:proj=0&log)
- [MCUXpressoSDK](https://mcuxpresso.nxp.com/api_doc/dev/1406/modules.html)
- [DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP/Source)
# :pushpin:DEPENDENCE
- 开发环境：Keil 5.30
- 编译器：ArmClang V6.14
- 使用MCUXpresso Config Tool初始化时钟和外设
- 请使用Keil打开`car_config.h`
# 十五届必胜 :tada: :tada: :tada:
