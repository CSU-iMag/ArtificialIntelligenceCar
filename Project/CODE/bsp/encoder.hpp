#ifndef ENCODER_H
#define ENCODER_H

#include "board.h"
#include "pid.hpp"
#include "route.h"
#include "scheduler.hpp"
#include "zf_qtimer.h"
#include "filter.hpp"

class Encoder {
    friend void pulse_encoder_schedule();
    friend void motor_pid_schedule();

  private:
    const QTIMERN_enum Channel; //编码器计数通道
    const QTIMER_PIN_enum LSB;  //编码器LSB线
    const QTIMER_PIN_enum DIR;  //编码器DIR线
    double distance;         //实际距离
	  // filter_movAve_t filter_speed;

  public:
    float speed;          //实际速度
    Encoder(QTIMERN_enum QTIMER_Channel, QTIMER_PIN_enum QTIMER_LSB,
            QTIMER_PIN_enum QTIMER_DIR);
    void Init();
    void CalSpeed();
    void CalDistance();
    __inline float GetSpeed() { return speed; }
    __inline float GetDistance() { return distance; }
};

void pulse_encoder_schedule();

#endif
