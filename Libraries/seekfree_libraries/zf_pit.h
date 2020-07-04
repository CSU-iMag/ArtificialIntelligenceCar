/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		pit
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#ifndef _zf_pit_h
#define _zf_pit_h

#include "common.h"
#include "fsl_pit.h"

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PerClk) //����PIT��ʱ��������ʱ��

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

//��ȡPIT��ʱ���ı�־λ
#define PIT_FLAG_GET(pit_ch) PIT_GetStatusFlags(PIT, pit_ch)

//���PIT��ʱ���ı�־λ
#define PIT_FLAG_CLEAR(pit_ch) PIT_ClearStatusFlags(PIT, pit_ch, kPIT_TimerFlag)

//------------------------------------���´�������PIT�ж�------------------------------------
//-----------------���ñ��꺯��ǰ������ȵ���pit_init�������Գ�ʼ��PIT����-------------------
#define pit_interrupt_ms(pit_ch, time)                                         \
    pit_interrupt(pit_ch, MSEC_TO_COUNT(time, PIT_SOURCE_CLOCK)) //(��λΪ ����)
#define pit_interrupt_us(pit_ch, time)                                         \
    pit_interrupt(pit_ch, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK)) //(��λΪ ΢��)

//------------------------------------���´������ڻ�ȡPIT��ʱʱ��------------------------------------
//-----------------���ñ��꺯��ǰ������ȵ���pit_init�������Գ�ʼ��PIT����-------------------
#define pit_get_ms(pit_ch)                                                     \
    COUNT_TO_MSEC(pit_get(pit_ch), PIT_SOURCE_CLOCK) //(��λΪ ����)
#define pit_get_us(pit_ch)                                                     \
    COUNT_TO_USEC(pit_get(pit_ch), PIT_SOURCE_CLOCK) //(��λΪ ΢��)
#define pit_get_ns(pit_ch)                                                     \
    COUNT_TO_USEC((uint64)pit_get(pit_ch) * 1000,                              \
                  PIT_SOURCE_CLOCK) //(��λΪ ����)

#endif
