#ifndef CAR_CFG_H
#define CAR_CFG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> ��λ��ͨ��

// <o> ���� (ms)
#define DIR_PD_PERIOD 30

// <o> �ٶ� (ms)
#define SPEED_PD_PERIOD 23

// <o> AI (ms)
#define AI_PD_PERIOD -1

// </h> ///////////////////////////////

// <h> ��Ŵ�����

// <o> Signal Count <3-12>
#define ADC_CNT 12

// <o> ǰհ�� <0-11>
#define MagFrontL 10

// <o> ǰհ�� <0-11>
#define MagFrontM 4

// <o> ǰհ�� <0-11>
#define MagFrontR 11

// <o>��ͷ�����<0-11>
#define MagLeftL 3

// <o>��ͷ���<0-11>
#define MagLeftX 9

// <o>��ͷ����<0-11>
#define MagLeftY 0

// <o>��ͷ�м�<0-11>
#define MagMiddleM 7

// <o>������<0-11>
#define MagMiddleL 1

// <o>������<0-11>
#define MagMiddleR 2

// <o>��ͷ����<0-11>
#define MagRightR 6

// <o>��ͷ�Һ�<0-11>
#define MagRightX 8

// <o>��ͷ����<0-11>
#define MagRightY 5

// <o> �����ж���ֵ <0-300>
#define MAG_DERAIL_THRESHOLD 52

// </h> ///////////////////////////////

// <h> �������

// <o> �����ֵ <0-10000>
//  <i>Default: 1500
#define STEER_CENTER 1250

// <o> ���ұ� <0-10000>
//  <i>Default: 400
#define STEER_MIN 1090

// <o> ����� <0-10000>
//  <i>Default: 400
#define STEER_MAX 1412

// <o> ���PWMƵ�� <50-200>
//  <i>Default: 150
#define STEER_FREQ 60

// </h> ////////////////////////////////

// <h> ���ֵ������

// <c>Auto Stop
#define AUTO_STOP
// </c>
// <i> ʧ���Զ�ͣ��

// <o>���PWMƵ��(Hz)<13000-17000>
#define MOTOR_FREQ 16661

// <o>���ռ�ձ�<0-100>
//  <i>Default: 72
#define MOTOR_MAX 70

// <o>��Сռ�ձ�<0-100>
//  <i>Default: 26
#define MOTOR_MIN 38

// </h> ////////////////////////////////

// <h> CPUռ����

// <o>Sample period<1-9999>
//  <i>(ms)
#define CPU_USAGE_PERIOD 60

// </h> //////////////////////////////

// <h>Filter Configuration

// <o> ������
#define FILTER_SPEED_SIZE 3

// <o> ���
#define FILTER_MAGNET_SIZE 3u

// <o> ��ֵ�˲�ȡֵ
// <i> ��ֵ�˲�Ԫ�ظ���
#define FILTER_MEDIAN_SIZE 5

// </h> //////////////////////////////

// <h>Interrupt Configuration

// <o> Button Priority<0-15>
#define BUTTON_PRIORITY 3

// </h> ////////////////////////////

// <h> Flash Configuration

// <o> ���Բ���-���� <512-1024>
// <i> �Ժ��ٶ�㼸�׷���
#define SLN_DEBUG_SECTOR 977
// <o> ���Բ���-ҳ <0-16>
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
