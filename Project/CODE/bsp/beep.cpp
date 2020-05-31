#include "beep.hpp"
#include "car.hpp"
#include "fsl_iomuxc.h"
#include "peripherals.h"
#include "timer.hpp"

static void beep_onTime(sched_event_data_t dat) { Car.beep0.Mute(); }

static SoftTimer beep_timer(beep_onTime, 1);

void Beep::Mute() { QTMR_StopTimer(TMR2, kQTMR_Channel_3); }

void Beep::BeepFreq(uint32_t Freq) {
    CAR_ERROR_CHECK(Freq < 10000);
    QTMR_SetupPwm(TMR2, kQTMR_Channel_3, Freq, 50, false,
                  TMR2_CHANNEL_3_CLOCK_SOURCE);
    QTMR_StartTimer(TMR2, kQTMR_Channel_3, kQTMR_PriSrcRiseEdge);
}

void Beep::BeepFreqDelay(uint32_t Freq, uint16_t ms) {
    BeepFreq(Freq);
    beep_timer.Start(ms);
}
