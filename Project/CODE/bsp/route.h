#ifndef ROUTE_H
#define ROUTE_H

#include "car_config.h"
#include "zf_pwm.h"

// S3010舵机
#define S3010_PWM PWM4_MODULE3_CHA_C31

// RS380电机*2
#define MOTOR_L PWM1_MODULE3_CHA_B10
#define MOTOR_R PWM1_MODULE3_CHB_B11

#define BEEP_PWM PWM2_MODULE3_CHA_C18

// #define LED_PWM PWM2_MODULE3_CHA_B9

static const int MagForDeep[] = {MagLeftL,  MagLeftX,   MagMiddleM, MagRightX,
                                 MagRightR, MagMiddleL, MagMiddleR};

#endif
