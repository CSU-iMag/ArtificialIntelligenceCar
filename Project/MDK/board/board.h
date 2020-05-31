#ifndef _BOARD_H_
#define _BOARD_H_

#include "RT106X_config.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_INSTANCE 1

#if     (BOARD_DEBUG_UART_INSTANCE == 1)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_UART_IRQ LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 2)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART2
#define BOARD_UART_IRQ LPUART2_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART2_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 3)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART3
#define BOARD_UART_IRQ LPUART3_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART3_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 4)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART4
#define BOARD_UART_IRQ LPUART4_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART4_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 5)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART5
#define BOARD_UART_IRQ LPUART5_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART5_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 6)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART6
#define BOARD_UART_IRQ LPUART6_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART6_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 7)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART7
#define BOARD_UART_IRQ LPUART7_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART7_IRQHandler

#elif   (BOARD_DEBUG_UART_INSTANCE == 8)
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART8
#define BOARD_UART_IRQ LPUART8_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART8_IRQHandler

#endif

#define BOARD_DEBUG_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (DEBUG_UART_BAUD)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The board flash size */
#define BOARD_FLASH_SIZE (0x400000U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

uint32_t BOARD_DebugConsoleSrcFreq(void);
void BOARD_InitDebugConsole(void);
void BOARD_ConfigMPU(void);

void board_init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
