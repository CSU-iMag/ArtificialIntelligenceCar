#ifndef CAR_CFG_H
#define CAR_CFG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> 上位机通信

// <o> 方向 (ms)
#define DIR_PD_PERIOD 30

// <o> 速度 (ms)
#define SPEED_PD_PERIOD 23

// <o> AI (ms)
#define AI_PD_PERIOD -1

// </h> ///////////////////////////////

// <h> 电磁传感器

// <o> Signal Count <3-12>
#define ADC_CNT 12

// <o> 前瞻左 <0-11>
#define MagFrontL 10

// <o> 前瞻中 <0-11>
#define MagFrontM 4

// <o> 前瞻右 <0-11>
#define MagFrontR 11

// <o>车头最左边<0-11>
#define MagLeftL 3

// <o>车头左横<0-11>
#define MagLeftX 9

// <o>车头左竖<0-11>
#define MagLeftY 0

// <o>车头中间<0-11>
#define MagMiddleM 7

// <o>车身左<0-11>
#define MagMiddleL 1

// <o>车身右<0-11>
#define MagMiddleR 2

// <o>车头最右<0-11>
#define MagRightR 6

// <o>车头右横<0-11>
#define MagRightX 8

// <o>车头右竖<0-11>
#define MagRightY 5

// <o> 出轨判定阈值 <0-300>
#define MAG_DERAIL_THRESHOLD 52

// </h> ///////////////////////////////

// <h> 舵机控制

// <o> 舵机中值 <0-10000>
//  <i>Default: 1500
#define STEER_CENTER 1250

// <o> 最右边 <0-10000>
//  <i>Default: 400
#define STEER_MIN 1090

// <o> 最左边 <0-10000>
//  <i>Default: 400
#define STEER_MAX 1412

// <o> 舵机PWM频率 <50-200>
//  <i>Default: 150
#define STEER_FREQ 60

// </h> ////////////////////////////////

// <h> 后轮电机配置

// <c>Auto Stop
#define AUTO_STOP
// </c>
// <i> 失控自动停车

// <o>电机PWM频率(Hz)<13000-17000>
#define MOTOR_FREQ 16661

// <o>最大占空比<0-100>
//  <i>Default: 72
#define MOTOR_MAX 70

// <o>最小占空比<0-100>
//  <i>Default: 26
#define MOTOR_MIN 38

// </h> ////////////////////////////////

// <h> CPU占用率

// <o>Sample period<1-9999>
//  <i>(ms)
#define CPU_USAGE_PERIOD 60

// </h> //////////////////////////////

// <h>Filter Configuration

// <o> 编码器
#define FILTER_SPEED_SIZE 3

// <o> 电磁
#define FILTER_MAGNET_SIZE 3u

// <o> 中值滤波取值
// <i> 锟斤拷值锟剿诧拷元锟截革拷锟斤拷
#define FILTER_MEDIAN_SIZE 5

// </h> //////////////////////////////

// <h>Interrupt Configuration

// <o> Button Priority<0-15>
#define BUTTON_PRIORITY 3

// </h> ////////////////////////////

// <h> Flash Configuration

// <o> 调试参数-扇区 <512-1024>
// <i> 以后再多搞几套方案
#define SLN_DEBUG_SECTOR 977
// <o> 调试参数-页 <0-16>
#define SLN_DEBUG_PAGE 0

// </h> ////////////////////////////

// <h> GUI Configuration

// <c> Use Chinese font
#define USE_CHINESE_FONT
// </c>

// </h> ////////////////////////////

// <h> Button Configuration

// <o> Number of Keys <0-9>
// <i> Default 6 in motherboard v1.0
#define BUTTON_CNT 6

// <o> Key scan period
#define KEY_SCAN_PERIOD 30

// <o> Long Push Threshold
#define LONG_PUSH_THRESHOLD 1000

// <o> Double Click Threshold
#define DBL_CLK_THRESHOLD 1000

// </h> ////////////////////////////

// <<< end of configuration section >>>

#endif
