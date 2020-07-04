/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		OLED
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note
                                        接线定义：
                                        ------------------------------------
                                            OLED液晶    单片机
                                        D0 查看SEEKFREE_OLED文件内的OLED_SCL_PIN
 宏定义 D1          查看SEEKFREE_OLED文件内的OLED_SDA_PIN   宏定义 RES
 查看SEEKFREE_OLED文件内的OLED_RST_PIN   宏定义 DC
 查看SEEKFREE_OLED文件内的OLED_DC_PIN    宏定义 CS
 查看SEEKFREE_OLED文件内的OLED_CS_PIN    宏定义
                                        ------------------------------------
 ********************************************************************************************************************/

#ifndef _SEEKFREE_OLED_h
#define _SEEKFREE_OLED_h

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void oled_init(void);
void oled_fill(uint8 dat);
void oled_putpixel(uint8 x, uint8 y, uint8 data1);

#ifdef __cplusplus
}
#endif

/********************************************************************/

#endif
