#ifndef ROUTE_H
#define ROUTE_H

#include "car_config.h"
#include "zf_pwm.h"
#include "pin_mux.h"

// Buttons
static const char ButtonList[BUTTON_CNT] = {
    BOARD_INITPINS_K1_PIN, BOARD_INITPINS_K2_PIN, BOARD_INITPINS_K3_PIN,
    BOARD_INITPINS_K4_PIN, BOARD_INITPINS_K5_PIN, BOARD_INITPINS_K6_PIN};

// S3010舵机
#define S3010_PWM PWM4_MODULE3_CHA_C31

//脉冲编码器
// #define encoderL_QTIMER QTIMER_1
// #define encoderR_QTIMER QTIMER_1
// #define encoderL_LSB_QTIMER QTIMER1_TIMER2_C2
// #define encoderR_LSB_QTIMER QTIMER1_TIMER0_C0
// #define encoderL_DIR_QTIMER QTIMER1_TIMER3_C24
// #define encoderR_DIR_QTIMER QTIMER1_TIMER1_C1

// RS380电机*2
#define MOTOR_L PWM1_MODULE3_CHB_B11
#define MOTOR_R PWM1_MODULE3_CHA_B10

#define LED_PWM PWM2_MODULE3_CHA_B9

// #define ENCODER_CHANNEL PIT_CH0
// #define MOTOR_CHANNEL PIT_CH1
// #define STEER_CHANNEL PIT_CH2

#endif
