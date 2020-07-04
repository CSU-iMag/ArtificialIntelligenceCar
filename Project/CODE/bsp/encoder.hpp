#ifndef ENCODER_H
#define ENCODER_H

#include "board.h"
#include "filter.hpp"
#include "pid.hpp"
#include "route.h"
#include "scheduler.hpp"
#include "zf_qtimer.h"

class Encoder {
    friend void pulse_encoder_schedule();

    filter_movAve_t filter_moving;
    const qtmr_channel_selection_t Channel; //编码器计数通道
    double distance;                        //实际距离
    float speed;                            //实际速度

    void CalSpeed();
    void CalDistance();
    
  public:
    Encoder(qtmr_channel_selection_t QTIMER_Channel);
    __inline float GetSpeed() { return speed; }
    __inline float GetDistance() { return distance; }
    __inline void ClearDistance() { distance = 0; }
};

void pulse_encoder_schedule();
void tmr_init();

#endif
