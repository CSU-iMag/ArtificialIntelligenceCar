#ifndef STEERx_H
#define STEERx_H

#include "pid.hpp"
#include "util.h"
#include "zf_pwm.h"

class Steer {
    PWMCH_enum PWM_Channel;

  public:
    float steerOffset;
    PID steerCtrl;
    uint8_t steerDutyforAI;
    float steerPID_Input;
    Steer(PWMCH_enum ch, float OutMin, float OutMax);
    void Init();
    void ModifyOffset(int8 varia);
    void DutySet(float duty);
    __inline void WidthSet(uint16_t us) { DutySet(PULSE_TO_PERCENT(us)); }
};

void steer_schedule();

#endif
