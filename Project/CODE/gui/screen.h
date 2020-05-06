// Abstract Layer from OLED to GUI
#ifndef SCREEN_H
#define SCREEN_H

#include "SGUI_Typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

static SGUI_POINT screen_innerPos = {.iX = 0, .iY = 0};
static SGUI_RECT screen_full = {.iX = 0, .iY = 0, .iWidth = 128, .iHeight = 64};

extern SGUI_SCR_DEV CarOLED;

void screen_clear(void);
void screen_refresh(void);
SGUI_INT screen_initialize(void);
SGUI_INT screen_getPixel(SGUI_INT x, SGUI_INT y);
void screen_setPixel(SGUI_INT x, SGUI_INT y, SGUI_INT color);

#ifdef __cplusplus
}
#endif

#endif
