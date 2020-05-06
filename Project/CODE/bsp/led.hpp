#ifndef LED_H
#define LED_H

#include "common.h"
#include "zf_gpio.h"
#include "zf_pwm.h"

class SoftLED {
    PWMCH_enum channel;

  public:
    void Init();
    void SetBrightness(float percent);
    SoftLED(PWMCH_enum ch) : channel(ch) {}
};

class HardLED {
    PIN_enum Pin;
    bool ActiveState;

  public:
    void Init();
    void TurnOn();
    void TurnOff();
    void Toggle();
    HardLED(PIN_enum pin, bool active_state = false)
        : Pin(pin), ActiveState(active_state) {}
};

void led_init();

#endif
