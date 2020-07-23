#include "bat.hpp"
#include "filter.hpp"
#include "fsl_tempmon.h"
#include "gui.hpp"
#include "peripherals.h"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"


static void bat_schedule(sched_event_data_t dat) {
    ADC_SetChannelConfig(BAT_PERIPHERAL, 0U,
                         (adc_channel_config_t *)&BAT_channels_config);

    gui_control.UpdateValue(5, std::to_string(cpu_usage_get()));
    gui_control.UpdateValue(
        6, std::to_string(TEMPMON_GetCurrentTemperature(TEMPMON)));
}

static SoftTimer bat_tim(bat_schedule);

void bat_init() {
    bat_tim.Start(999);
}

extern "C" void BAT_IRQHANDLER(void) {
    uint32_t bat_val = ADC_GetChannelConversionValue(BAT_PERIPHERAL, 0U);
    CAR_ERROR_CHECK(bat_val < (1 << 13));  // 12bit ADC

    //    static filter_movAve_t filter_battery(3);
    //    bat_val = filter_battery.Moving(bat_val);
     bat_val = battery_level_in_percent(ADC12b_TO_mV(bat_val));
    //    CAR_ERROR_CHECK(bat_val != lim(bat_val, 3, 9));

    gui_background.UpdateValue(bat_val);
    gui_control.UpdateValue(4, std::to_string(bat_val));
    gui_control.battery_curve.AppendValue(bat_val);
    // gui_debug.UpdateValue(1, "Enabled:" + std::to_string(Car.Enabled));
}
