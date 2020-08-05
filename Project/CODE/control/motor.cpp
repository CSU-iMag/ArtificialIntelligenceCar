#include "motor.hpp"
#include "car.hpp"
#include "communication.hpp"
#include "gui.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"

Motor::Motor(PWMCH_enum ch, float OutMin, float OutMax)
    : duty(0), PWM_Channel(ch), speedCtrl(std::make_pair(OutMin, OutMax)) {}

void Motor::Init() { pwm_init(PWM_Channel, MOTOR_FREQ, 0); }

/**
 * @name   设置电机PWM
 * @param  duty   PWM占空比   range:0-100%
 * @note   MotorL.SetDuty(50);
 */
void Motor::SetDuty(float Duty) {
    uint32 pwm = PERCENT_TO_TICKS(Duty);
    CAR_ERROR_CHECK(pwm < PWM_DUTY_MAX);
    pwm_duty(PWM_Channel, pwm);
    duty = Duty;
}

void Motor::Stop(void) {
    SetDuty(0);
    speedCtrl.Reset();
}

void motor_pid_schedule() {
    CRITICAL_REGION_ENTER();
    auto dutyL = Car.MotorL.speedCtrl.Realize(Car.MotorL.target - Car.EncoderL.GetSpeed());
    auto dutyR = Car.MotorR.speedCtrl.Realize(Car.MotorL.target - Car.EncoderR.GetSpeed());
    Car.MotorL.SetDuty(dutyL);
    Car.MotorR.SetDuty(dutyR);
    CRITICAL_REGION_EXIT();
}
