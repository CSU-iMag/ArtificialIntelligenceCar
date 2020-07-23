#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "fsl_common.h"
#include <string>

enum log_color_enum { LogBlack, LogRed, LogGreen, LogBlue, LogOrange, LogLime };

static const uint8_t SOH = 0x01, EOT = 0x04, ESC = 0x5c;

void com_init();
void com_push(uint8_t packID, const uint8_t *payload, uint8_t length);

/**
 * @brief 向上位机发送彩色日志
 * @param str [in]
 * @param color [in] @ref log_color_enum
 * @code
 *  com_log("Hello, World!\n", LogLime);
 * @endcode
 */
__STATIC_INLINE void com_log(std::string str, log_color_enum color = LogBlack) {
    str.insert(str.cbegin(), (char)color);
    com_push(0x8f, (const uint8_t *)&str[0], str.size());
}

#endif
