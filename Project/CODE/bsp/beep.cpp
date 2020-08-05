#include "beep.hpp"
#include "car.hpp"
#include "zf_pwm.h"
#include "timer.hpp"

static void beep_onTime(sched_event_data_t dat) { Car.Buzzer.Mute(); }

static SoftTimer beep_timer(beep_onTime, 1);

void Beep::Mute() { pwm_freq(BEEP_PWM, 666, 0); }

void Beep::Init() { pwm_init(BEEP_PWM, 666, 0); }

void Beep::BeepFreq(uint32_t Freq) {
    CAR_ERROR_CHECK(Freq < 10000);
    pwm_freq(BEEP_PWM, Freq, PERCENT_TO_TICKS(50));
}

void Beep::BeepFreqDelay(uint32_t Freq, uint16_t ms) {
    BeepFreq(Freq);
    beep_timer.Start(ms);
}