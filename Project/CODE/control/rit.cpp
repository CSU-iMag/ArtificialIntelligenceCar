#include "car.hpp"
#include "encoder.hpp"
#include "motor.hpp"
#include "peripherals.h"
#include "steer.hpp"
#include "util.h"
#include "zf_pit.h"

extern "C" void PIT_IRQHandler(void) {
    if (PIT_FLAG_GET(PIT_ENCODER_CH)) {
        PIT_FLAG_CLEAR(PIT_ENCODER_CH);
        pulse_encoder_schedule();
    }

    if (PIT_FLAG_GET(PIT_MOTOR_CH)) {
        PIT_FLAG_CLEAR(PIT_MOTOR_CH);
        // if (IS_SET(Car.Enabled, SW_SPEED))
        //     motor_pid_schedule();
    }

    if (PIT_FLAG_GET(PIT_STEER_CH)) {
        PIT_FLAG_CLEAR(PIT_STEER_CH);
        if (IS_SET(Car.Enabled, SW_STEER))
            sched_event_put(steer_schedule);
    }

    __DSB();
}
