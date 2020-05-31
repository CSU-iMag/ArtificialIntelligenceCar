#include "bat.hpp"
#include "car_config.h"
#include "filter.hpp"
#include "fsl_tempmon.h"
#include "gui.hpp"
#include "route.h"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"
#include "zf_adc.h"

filter_movAve_t filter_battery(3);
filter_kalman kk(10, 100);

static void bat_schedule(sched_event_data_t dat) {
    static constexpr int buf_size(16);
    static q15_t bat_val, bat_buf[buf_size];
    for (int i(0); i < buf_size; ++i)
        bat_buf[i] = adc_convert(BAT_ADC, BAT_ADC_CH);

    arm_rms_q15(bat_buf, buf_size, &bat_val);
    // bat_val = filter_battery.Moving(bat_val);
   bat_val= kk.Filte(bat_val);
   CAR_ERROR_CHECK(bat_val < (1 << 13));
   bat_val = battery_level_in_percent(ADC12b_TO_mV(bat_val));
//    CAR_ERROR_CHECK(bat_val != lim(bat_val, 3, 9));

   // DEBUG_LOG("Bat: %d\n", bat_val);
   gui_background.UpdateValue(bat_val);
   gui_control.UpdateValue(4, std::to_string(bat_val));
   gui_control.UpdateValue(5, std::to_string(cpu_usage_get()));
   gui_control.UpdateValue(
       6, std::to_string(TEMPMON_GetCurrentTemperature(TEMPMON)));
   gui_control.battery_curve.AppendValue(bat_val);
}

static SoftTimer bat_tim(bat_schedule);

void bat_init() {
    adc_init(BAT_ADC, BAT_ADC_CH, ADC_12BIT);
    bat_tim.Start(999);
}
