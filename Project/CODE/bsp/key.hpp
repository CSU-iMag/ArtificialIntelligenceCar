#ifndef KEY_H
#define KEY_H

#include "fsl_gpio.h"

enum class key_action { key_down, key_up, key_longPush, key_doubleClick };
typedef void (*key_event_handler_t)(key_action sta);

class Button {
    enum class ButtonState {
        idle,
        press_armed,
        press_detected,
        pressed,
        release_detected,
        long_push,
        click
    } state;

    char pin;
    uint8_t pressed_cnt, click_cnt;
    key_event_handler_t event;

  public:
    Button(char PIN, key_event_handler_t Event)
        : pin(PIN), event(Event), state(ButtonState::idle), click_cnt(0),
          pressed_cnt(0) {}
    void Init();
    bool IsPressed();
    void evt_handle();
};

void key_init();

#endif
