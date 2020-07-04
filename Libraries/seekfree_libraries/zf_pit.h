/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		pit
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#ifndef _zf_pit_h
#define _zf_pit_h

#include "common.h"
#include "fsl_pit.h"

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PerClk) //定义PIT定时器的输入时钟

#ifdef __cplusplus
extern "C" {
#endif

void pit_init(void);
void pit_interrupt(pit_chnl_t pit_ch, uint32 count);
uint32 pit_get(pit_chnl_t pit_ch);
void pit_close(pit_chnl_t pit_ch);

#ifdef __cplusplus
}
#endif

//获取PIT定时器的标志位
#define PIT_FLAG_GET(pit_ch) PIT_GetStatusFlags(PIT, pit_ch)

//清除PIT定时器的标志位
#define PIT_FLAG_CLEAR(pit_ch) PIT_ClearStatusFlags(PIT, pit_ch, kPIT_TimerFlag)

//------------------------------------以下代码用于PIT中断------------------------------------
//-----------------调用本宏函数前，务必先调用pit_init函数，以初始化PIT外设-------------------
#define pit_interrupt_ms(pit_ch, time)                                         \
    pit_interrupt(pit_ch, MSEC_TO_COUNT(time, PIT_SOURCE_CLOCK)) //(单位为 毫秒)
#define pit_interrupt_us(pit_ch, time)                                         \
    pit_interrupt(pit_ch, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK)) //(单位为 微秒)

//------------------------------------以下代码用于获取PIT计时时间------------------------------------
//-----------------调用本宏函数前，务必先调用pit_init函数，以初始化PIT外设-------------------
#define pit_get_ms(pit_ch)                                                     \
    COUNT_TO_MSEC(pit_get(pit_ch), PIT_SOURCE_CLOCK) //(单位为 毫秒)
#define pit_get_us(pit_ch)                                                     \
    COUNT_TO_USEC(pit_get(pit_ch), PIT_SOURCE_CLOCK) //(单位为 微秒)
#define pit_get_ns(pit_ch)                                                     \
    COUNT_TO_USEC((uint64)pit_get(pit_ch) * 1000,                              \
                  PIT_SOURCE_CLOCK) //(单位为 纳秒)

#endif
