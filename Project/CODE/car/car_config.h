#ifndef CAR_CFG_H
#define CAR_CFG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> ��λ��ͨ��

// <q> ʹ�ð�׿������������
//  <i> �رպ��ӡ������Ϣ
#define BLUETOOTH_ENABLED 0

// <o> HC-06 ������
// <i> Default 115200bps
// <921600=> 921600
// <460800=> 460800
// <115200=> 115200
// <38400=>  38400
// <9600=>   9600
#define HC06_BPS 9600u

// <s.4> HC-06 password
// <i> four digits
#define HC06_PASSWORD "2333"

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
// <o> PGA Count <0-12>
#define PGA_CNT 8

// <o>Sample period<1-100>
//  <i>(ms)
#define MAG_SAMPLE_PERIOD 5

// <o> ǰհ�� <0-11>
#define MagL_FRONT 9

// <o> ǰհ�� <0-11>
#define MagM_FRONT 10

// <o> ǰհ�� <0-11>
#define MagR_FRONT 11

// <o>MagL_ROW2<0-11>
#define MagL_ROW2 1

// <o>MagLM_ROW2<0-11>
#define MagLM_ROW2 3

// <o>MagM_ROW2<0-11>
#define MagM_ROW2 8

// <o>MagRM_ROW2<0-11>
#define MagRM_ROW2 0

// <o>MagR_ROW2<0-11>
#define MagR_ROW2 2

// <o>MagL_ROW3<0-11>
#define MagL_ROW3 6

// <o>MagR_ROW3<0-11>
#define MagR_ROW3 7

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
#define MOTOR_FREQ 52123

// <o>���ռ�ձ�<0-100>
//  <i>Default: 72
#define MOTOR_MAX 70

// <o>��Сռ�ձ�<0-100>
//  <i>Default: 26
#define MOTOR_MIN 3

// <o>Ŀ���ٶ�<0-200>
//  <i>cm/s
#define TARGET_SPEED 85

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

// <o> ����������������� <1-100>
//  <i>Default: 3ms
#define ENCODER_PERIOD 3

// <o> �ٶ�PID <1-100>
//  <i>Default: 6ms
#define MOTOR_PERIOD 6

// <o> ����PID <1-100>
//  <i>Default: 5ms
#define STEER_PERIOD 5

// <o>RIT Priority<0-15>
#define RIT_PRIORITY 2

// <o> MPU6050 Priority<0-15>
#define MPU6050_PRIORITY 3

// <o> HC06 Priority<0-15>
#define HC06_PRIORITY 5

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

// <h> LED Configuration

// <o> PWM Frequence <10-99999>
#define LED_PWM_FREQ 60000

// <o> Soft blink interval
#define LED_SOFT_BLINK_INTERVAL 60

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

// <o> Up <0-6>
#define BUTTON_UP_ID 1
// <o> Down <0-6>
#define BUTTON_DOWN_ID 5
// <o> Left <0-6>
#define BUTTON_LEFT_ID 2
// <o> Right <0-6>
#define BUTTON_RIGHT_ID 3
// <o> Enter <0-6>
#define BUTTON_ENTER_ID 0
// <o> Escape <0-6>
#define BUTTON_ESCAPE_ID 4 

// <o> Key scan period
#define KEY_SCAN_PERIOD 30

// <o> Long Push Threshold
#define LONG_PUSH_THRESHOLD 1000

// <o> Double Click Threshold
#define DBL_CLK_THRESHOLD 1000

// <c> Use Interrupt
//#define BUTTON_MODE_INT
// </c>
//<i> Default polling mode.

// </h> ////////////////////////////

// <<< end of configuration section >>>

#endif
