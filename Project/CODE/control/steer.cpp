#include "steer.hpp"
#include "car.hpp"
#include "car_config.h"
#include "gui.hpp"
#include "magnet.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"
#include <string>

//-------------------------------------------------------------------------------------------------------------------
//  @name     �������͵��PWM���Ƴ�ʼ��
//  @sample   Motor
//  MotorL(MOTOR_L,encoderL_QTIMER,encoderL_LSB_QTIMER,encoderL_DIR_QTIMER);
//  @sample   Motor
//  MotorR(MOTOR_R,encoderR_QTIMER,encoderR_LSB_QTIMER,encoderR_DIR_QTIMER);
//-------------------------------------------------------------------------------------------------------------------
Steer::Steer(PWMCH_enum ch, float OutMin, float OutMax)
    : PWM_Channel(ch), steerCtrl(std::make_pair(OutMin, OutMax)),
      steerOffset(0) {}

void Steer::Init() {
    pwm_init(PWM_Channel, STEER_FREQ,
             PERCENT_TO_TICKS(PULSE_TO_PERCENT(STEER_CENTER)));
}

void Steer::DutySet(float duty) {
    CAR_ERROR_CHECK(PERCENT_TO_TICKS(duty) < PWM_DUTY_MAX);
    pwm_duty(PWM_Channel, PERCENT_TO_TICKS(duty));
}

//-------------------------------------------------------------------------------------------------------------------
//  @name     pid���ƶ��
//-------------------------------------------------------------------------------------------------------------------
void steer_schedule() {
    if (Car.CtrlMode != ControlMode::PID)
        return;
    CRITICAL_REGION_ENTER();
    auto steering =
        Car.Steer3010.steerCtrl.Realize(MagErrorForPID) + STEER_CENTER;
    LIMITING(steering, STEER_MIN, STEER_MAX);
    Car.Steer3010.steerDutyforAI =
        RESCALE_VALUE(steering - STEER_MIN, 255, STEER_MAX - STEER_MIN);
    Car.Steer3010.WidthSet(steering);
    CRITICAL_REGION_EXIT();
}
