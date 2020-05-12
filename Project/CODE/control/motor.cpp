#include "motor.hpp"
#include "car.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"

Motor::Motor(PWMCH_enum ch, float OutMin, float OutMax)
    : PWM_Channel(ch), speedCtrl(std::make_pair(OutMin, OutMax)) {}

void Motor::Init() { pwm_init(PWM_Channel, MOTOR_FREQ, 0); }

/**
 * @name     ���õ��PWM
 * @param  duty   PWMռ�ձ�   range:0-100/%
 * @note   MotorL.PWM_set(50);
 */
void Motor::SetDuty(float duty) {
    uint32 pwm = PERCENT_TO_TICKS(duty);
    CAR_ERROR_CHECK(pwm < PWM_DUTY_MAX);
    pwm_duty(PWM_Channel, pwm);
}

void Motor::Stop(void) {
    pwm_duty(PWM_Channel, 0);
    speedCtrl.Reset();
}

void motor_pid_schedule() {
    CRITICAL_REGION_ENTER();
    auto dutyL =
        Car.MotorL.speedCtrl.Realize(Car.TargetSpeed - Car.EncoderL.GetSpeed());
    auto dutyR =
        Car.MotorR.speedCtrl.Realize(Car.TargetSpeed - Car.EncoderR.GetSpeed());
    Car.MotorL.SetDuty(dutyL);
    Car.MotorR.SetDuty(dutyR);
    Car.MotorL.duty = dutyL;
    Car.MotorR.duty = dutyR;
    CRITICAL_REGION_EXIT();
}
