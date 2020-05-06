#include "led.hpp"
#include "car.hpp"
#include "util.h"
#include "timer.hpp"

void SoftLED::SetBrightness(float percent) {
    pwm_duty(channel, PERCENT_TO_TICKS(percent)); 
}

void SoftLED::Init() {
    pwm_init(channel, LED_PWM_FREQ, 0);
}

void HardLED::Init() { gpio_init(Pin, GPO, !ActiveState, GPIO_PIN_CONFIG); }

void HardLED::TurnOn() { gpio_set(Pin, ActiveState); }

void HardLED::TurnOff() { gpio_set(Pin, !ActiveState); }

void HardLED::Toggle() { gpio_toggle(Pin); }

//! @brief 渐变LED指示CPU正在运行
void led_schedule(sched_event_data_t dat) {
    static bool CountDown;
    static uint8_t Duty;
    Duty += CountDown ? -1 : 1;
    if (Duty == 66)
        CountDown = false;
    if (Duty >= 99)
        CountDown = true;

    Car.ledCore.Toggle();
    Car.ledGreen.SetBrightness(Duty);
}

SoftTimer ledTmr(led_schedule);

void led_init() {
    Car.ledCore.Init();
    Car.ledGreen.Init();
    Car.ledWhite.Init();
    Car.ledBlue.Init();
    ledTmr.Start(LED_SOFT_BLINK_INTERVAL);
}
