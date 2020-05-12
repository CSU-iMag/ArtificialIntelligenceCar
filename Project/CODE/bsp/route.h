#ifndef ROUTE_H
#define ROUTE_H

#include "headfile.h"
#include "car_config.h"

// ADC
#define BAT_ADC ADC_1
#define BAT_ADC_CH ADC1_CH15_B26
#define MAGNET_ADC ADC_1

//这两个必须同时改动
#define MAG_SAMPLE_RESOLUTION ADC_12BIT
#define MAG_SAMPLE_RESOLUTION_NUM 12
//这两个必须同时改动

//static const ADCCH_enum ADCList[12] = {
//    ADC1_CH10_B21,
//    ADC1_CH7_B18,
//    ADC1_CH8_B19,
//    ADC1_CH9_B20,
//    ADC1_CH11_B22,
//    ADC1_CH14_B25,
//    ADC1_CH13_B24,
//    ADC1_CH0_B27,
//    ADC1_CH4_B15,
//    ADC1_CH3_B14,
//    ADC1_CH5_B16,
//    ADC1_CH6_B17
//};

// Buttons
static const PIN_enum ButtonList[BUTTON_CNT] = {B30, B31, D4, D3, C27, D1};

// LED
#define LED_PIN0 B9
#define LED_PWM0 PWM2_MODULE3_CHA_B9
#define LED_PWM1 PWM4_MODULE3_CHA_C31
#define LED_PWM2 PWM2_MODULE3_CHB_C19
#define LED_PIN3 C23

// HC-06
#define HC06_UART USART_1
// #define HC06_LPUART LPUART1
#define HC06_RX UART1_RX_B13
#define HC06_TX UART1_TX_B12
#define HC06_STA C26
#define HC06_IRQn LPUART1_IRQn

// S3010舵机
#define S3010_PWM PWM2_MODULE3_CHA_D2

// 霍尔传感器
#define HALL_PIN C18

//脉冲编码器
#define encoderL_QTIMER QTIMER_1
#define encoderR_QTIMER QTIMER_1
#define encoderL_LSB_QTIMER QTIMER1_TIMER2_C2
#define encoderR_LSB_QTIMER QTIMER1_TIMER0_C0
#define encoderL_DIR_QTIMER QTIMER1_TIMER3_C24
#define encoderR_DIR_QTIMER QTIMER1_TIMER1_C1

//RS380电机*2
#define MOTOR_L  PWM1_MODULE3_CHB_C17
#define MOTOR_R  PWM1_MODULE3_CHA_C16
#define MOTOR_EN_PIN C20

#define ENCODER_CHANNEL PIT_CH0
#define MOTOR_CHANNEL PIT_CH1
#define STEER_CHANNEL PIT_CH2

#endif
