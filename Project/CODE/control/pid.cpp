#include "pid.hpp"
#include "util.h"
#include <string>
extern "C" {
void arm_pid_reset_f32(arm_pid_instance_f32 *);
void arm_pid_init_f32(arm_pid_instance_f32 *, int32_t);
}

PID::PID(std::pair<float, float> OutLim)
    : minOut(OutLim.first), maxOut(OutLim.second) {
    arm_pid_reset_f32(&instance);
}

void PID::SetPID(float P, float I, float D) {
    instance.Kp = P;
    instance.Ki = I;
    instance.Kd = D;
    arm_pid_init_f32(&instance, 0);
}

PID &PID::operator=(const pid_coefficient_t &coeff) {
    SetPID(coeff.Kp, coeff.Ki, coeff.Kd);
    return *this;
}

void PID::SetK(char index, float32_t varia) {
    switch (std::toupper(index)) {
    case 'P':
        instance.Kp += varia;
        break;
    case 'I':
        instance.Ki += varia;
        break;
    case 'D':
        instance.Kd += varia;
        break;
    default:
        DEBUG_LOG("Invalid parameters!\n");
        return;
    }
    arm_pid_init_f32(&instance, 0);
}

void PID::Reset() { arm_pid_reset_f32(&instance); }

float PID::Realize(float err) {
    //    float output = arm_pid_f32(&instance, err);
    //    LIMITING(output, minOut, maxOut);
    //    return output;
    arm_pid_f32(&instance, err);
    float output = LIMITING(instance.state[2], minOut, maxOut);
    return output;
}
