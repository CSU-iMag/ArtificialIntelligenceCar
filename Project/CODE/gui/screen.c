#include "screen.h"
#include "SEEKFREE_OLED.h"
#include <stdbool.h>
#include <string.h>

SGUI_SCR_DEV CarOLED = {.stSize = {128, 64},
                        .fnClear = screen_clear,
                        .fnSetPixel = screen_setPixel,
                        .fnGetPixel = screen_getPixel,
                        .fnInitialize = screen_initialize,
                        .fnSyncBuffer = screen_refresh};

static bool screen_buffer[128][64];

void screen_clear(void) {
    memset(screen_buffer, 0, sizeof screen_buffer);
    oled_fill(0);
}

void screen_refresh(void) {
    for (unsigned i = 0; i < 128; ++i)
        for (unsigned j = 0; j < 8; ++j) {
            unsigned char dat = 0;
            for (unsigned k = 0; k < 8; ++k)
                if (screen_buffer[i][j * 8 + k])
                    dat |= 1 << k;
            oled_putpixel(i, j, dat);
        }
}

int screen_initialize(void) {
    oled_init();
    return 0;
}

SGUI_INT screen_getPixel(SGUI_INT x, SGUI_INT y) {
    if (x > 127 || y > 63)
        return 0;
    return screen_buffer[x][y] ? 1 : 0;
}

void screen_setPixel(SGUI_INT x, SGUI_INT y, SGUI_INT color) {
    screen_buffer[x][y] = color ? true : false;
}
