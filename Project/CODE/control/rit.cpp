#include "car_config.h"
#include "encoder.hpp"
#include "motor.hpp"
#include "route.h"
#include "steer.hpp"
#include "zf_pit.h"
#include "peripherals.h"

extern "C" void PIT_IRQHandler(void) {
    if (PIT_FLAG_GET(PIT_ENCODER_CH)) {
        PIT_FLAG_CLEAR(PIT_ENCODER_CH);
        pulse_encoder_schedule();
    }

    if (PIT_FLAG_GET(PIT_MOTOR_CH)) {
        PIT_FLAG_CLEAR(PIT_MOTOR_CH);
        motor_pid_schedule();
    }

    if (PIT_FLAG_GET(PIT_STEER_CH)) {
        PIT_FLAG_CLEAR(PIT_STEER_CH);
        sched_event_put(steer_schedule);
    }

    __DSB();
}
