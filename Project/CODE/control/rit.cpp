#include "rit.hpp"
#include "car_config.h"
#include "encoder.hpp"
#include "headfile.h"
#include "magnet.hpp"
#include "motor.hpp"
#include "scheduler.hpp"
#include "steer.hpp"
#include "deep.hpp"
#include "zf_pit.h"

void rit_init(void) {
    pit_init();                                        //初始化pit外设
    pit_interrupt_ms(ENCODER_CHANNEL, ENCODER_PERIOD); //初始化pit通道0 周期
    NVIC_SetPriority(PIT_IRQn,
                     RIT_PRIORITY); //设置中断优先级 范围0-15 越小优先级越高
                                    //四路PIT共用一个PIT中断函数
}

extern "C" void PIT_IRQHandler(void) {
    if (PIT_FLAG_GET(ENCODER_CHANNEL)) {
        PIT_FLAG_CLEAR(ENCODER_CHANNEL);
        pulse_encoder_schedule();
    }

    if (PIT_FLAG_GET(MOTOR_CHANNEL)) {
        PIT_FLAG_CLEAR(MOTOR_CHANNEL);
        motor_pid_schedule();
    }

    if (PIT_FLAG_GET(STEER_CHANNEL)) {
        PIT_FLAG_CLEAR(STEER_CHANNEL);
        steer_schedule();
        deep_schedule();
    }

    //    if (PIT_FLAG_GET(PIT_CH3)) {
    //        PIT_FLAG_CLEAR(PIT_CH3);
    //    }

    __DSB();
}
