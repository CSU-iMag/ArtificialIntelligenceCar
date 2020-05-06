#ifndef PID_H
#define PID_H

#include <arm_math.h>
#include <utility>

struct pid_coefficient_t {
    float32_t Kp, Ki, Kd;
} __PACKED;

class PID {
  private:
    float minOut, maxOut;

  public:
    arm_pid_instance_f32 instance = {0x0};

    PID(std::pair<float, float> OutMinMax);
    PID &operator=(const pid_coefficient_t &coeff);
    
    void SetPID(float Kp, float Ki, float Kd);
    void SetK(char index,float32_t varia);
    void Reset();
    float Realize(float err);
};

#endif
