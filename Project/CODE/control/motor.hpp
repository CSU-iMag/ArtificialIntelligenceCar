#ifndef MOTORx_H
#define MOTORx_H

#include "board.h"
#include "pid.hpp"
#include "route.h"
#include "scheduler.hpp"
#include "zf_pwm.h"
#include "zf_qtimer.h"
class Motor {
    friend void motor_pid_schedule();

  private:
    PWMCH_enum PWM_Channel;

  public:
    PID speedCtrl;
    float duty;
    float target;
    
    Motor(PWMCH_enum ch, float OutMin, float OutMax);
    void Init();
    void SetDuty(float duty);
    void Stop(void);
};

void motor_pid_schedule(void);

#endif
