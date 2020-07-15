#ifndef ROUTE_H
#define ROUTE_H

#include "car_config.h"
#include "zf_pwm.h"
#include "pin_mux.h"

// S3010舵机
#define S3010_PWM PWM4_MODULE3_CHA_C31

// RS380电机*2
#define MOTOR_L PWM1_MODULE3_CHB_B11
#define MOTOR_R PWM1_MODULE3_CHA_B10

#define LED_PWM PWM2_MODULE3_CHA_B9

#endif
