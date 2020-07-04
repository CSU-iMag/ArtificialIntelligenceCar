/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		OLED
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note
                                        ���߶��壺
                                        ------------------------------------
                                            OLEDҺ��    ��Ƭ��
                                        D0 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SCL_PIN
 �궨�� D1          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SDA_PIN   �궨�� RES
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_RST_PIN   �궨�� DC
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_DC_PIN    �궨�� CS
 �鿴SEEKFREE_OLED�ļ��ڵ�OLED_CS_PIN    �궨��
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
