#include "led.hpp"
#include "car.hpp"
#include "util.h"
#include "timer.hpp"

void HardLED::Init() { gpio_init(Pin, GPO, !ActiveState, GPIO_PIN_CONFIG); }

void HardLED::TurnOn() { gpio_set(Pin, ActiveState); }

void HardLED::TurnOff() { gpio_set(Pin, !ActiveState); }

void HardLED::Toggle() { gpio_toggle(Pin); }

//! @brief 渐变LED指示CPU正在运行
void led_schedule(sched_event_data_t dat) {
    static bool CountDown;
    static uint8_t Duty;
    Duty += CountDown ? -10 : 10;
    if (Duty <= 66)
        CountDown = false;
    if (Duty >= 89)
        CountDown = true;
    // Car.CoreLED.SetBrightness(Duty);
    Car.CoreLED.Toggle();
}

SoftTimer ledTmr(led_schedule);

void led_init() {
    Car.CoreLED.Init();
    ledTmr.Start(69);
}
