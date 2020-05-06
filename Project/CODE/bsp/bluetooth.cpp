#include "Bluetooth.hpp"
#include "car.hpp"
#include "zf_uart.h"
#include "pack.hpp"

static const Baud_t BAUD[12] = {{'1', 1200},   {'2', 2400},   {'3', 4800},
                                {'4', 9600},   {'5', 19200},  {'6', 38400},
                                {'7', 57600},  {'8', 115200}, {'9', 230400},
                                {'A', 460800}, {'B', 921600}, {'C', 1382400}};

const lpuart_config_t bluetooth_config = {
  .baudRate_Bps = 115200,
  .parityMode = kLPUART_ParityDisabled,
  .dataBitsCount = kLPUART_EightDataBits,
  .isMsb = false,
  .stopBitCount = kLPUART_OneStopBit,
  .txFifoWatermark = 0,
  .rxFifoWatermark = 1,//todo
  .enableRxRTS = false,
  .enableTxCTS = false,
  .txCtsSource = kLPUART_CtsSourceMatchResult,
  .txCtsConfig = kLPUART_CtsSampleAtStart,
  .rxIdleType = kLPUART_IdleTypeStopBit,
  .rxIdleConfig = kLPUART_IdleCharacter8,
  .enableTx = true,
  .enableRx = true
};

lpuart_handle_t bluetooth_handle;


#define BLUETOOTH_RX_BUFFER_SIZE 20
uint8_t bluetooth_rxBuffer[BLUETOOTH_RX_BUFFER_SIZE];
const lpuart_transfer_t bluetooth_rxTransfer = {
  .data = bluetooth_rxBuffer,
  .dataSize = BLUETOOTH_RX_BUFFER_SIZE
};

void bluetooth_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                   void *userData) {
    if (kStatus_LPUART_RxIdle == status) {
//			    auto len = LPUART_TransferGetRxRingBufferLength(HC06_LPUART, &bluetooth_handle);
//			    RxPack_Handle(bluetooth_rxBuffer,len);
			
//        static bool newLine(true);
//        if (newLine) {
//            Car.Hc06.rx_buffer = "";
//            newLine = false;
//        }
//        DEBUG_LOG("rx:::%d  len:::%d", Car.Hc06.rx_byte, handle->rxDataSize);
//        if (Car.Hc06.rx_byte == '\r') {
//            DEBUG_LOG("Bluetooth Rx: %s\n", Car.Hc06.rx_buffer.c_str());
//            newLine = true;
//        } else
//            Car.Hc06.rx_buffer += Car.Hc06.rx_byte;
    }
}

void Bluetooth::Init() {
    baud = BAUD + 7; //115200
    password = HC06_PASSWORD;
    parity_bit = 0;
    NVIC_SetPriority(HC06_IRQn, HC06_PRIORITY);
	
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, /* GPIO_AD_B0_12 is configured
                                                      as HC06_LPUART_TX */
                     0U); 
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, /* GPIO_AD_B0_13 is configured
                                                      as HC06_LPUART_RX */
                     0U);
	
	  auto uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
//    LPUART_Init(HC06_LPUART, &bluetooth_config, uartClkSrcFreq);
//	  LPUART_Deinit(HC06_LPUART);
//    LPUART_Init(HC06_LPUART, &bluetooth_config, uartClkSrcFreq);
//	
//    LPUART_TransferCreateHandle(HC06_LPUART, &bluetooth_handle, bluetooth_callback, NULL);

//    LPUART_EnableInterrupts(HC06_LPUART, kLPUART_RxDataRegFullInterruptEnable|kLPUART_IdleLineInterruptEnable);

//    LPUART_TransferStartRingBuffer(HC06_LPUART,&bluetooth_handle,
//                                   bluetooth_rxBuffer,
//                                   BLUETOOTH_RX_BUFFER_SIZE);
//																	 
//		LPUART_TransferReceiveNonBlocking(HC06_LPUART, &bluetooth_handle,
//		(lpuart_transfer_t *)&bluetooth_rxTransfer, NULL);

}

void Bluetooth::TestComunication() { uart_putstr(uartn, "AT\n\r"); }

void Bluetooth::ModifyBaud(int8 varia) {
    baud += varia;
    LIMITING(baud, BAUD, BAUD + 11);
    uart_putstr(uartn, "AT+BAUD");
    uart_putchar(uartn, baud->str);
    uart_init(DEBUG_UART, baud->baudrate, DEBUG_UART_TX_PIN, DEBUG_UART_RX_PIN);
}

void Bluetooth::SendPassword() {
    uart_putstr(uartn, "AT+PIN");
    uart_putstr(uartn, password.c_str());
}

void Bluetooth::SetRole(int8 option) {
    switch (option) {
    case -1:
        isMaster = false;
        break;
    case 1:
        isMaster = true;
        break;
    default:
        break;
    }
    uart_putstr(uartn, isMaster ? "AT+ROLE=S" : "AT+ROLE=M");
}

void Bluetooth::ModifyValid(int8 varia) {
    parity_bit += varia;
    LIMITING(parity_bit, 0, 2);
    switch (parity_bit) {
    case 0:
        uart_putstr(uartn, "AT+PN");
        break;
    case 1:
        uart_putstr(uartn, "AT+PO");
        break;
    case 2:
        uart_putstr(uartn, "AT+PE");
        break;
    default:
        break;
    }
}

void Bluetooth::GetVersion() { uart_putstr(uartn, "AT+VERSION"); }

// void Bluetooth::ConvertLED() {
//    static bool is_LedOn=true;
//    is_LedOn	= !is_LedOn;
//    if (is_LedOn)
//        uart_putstr(uartn, "AT+LED0");
//    else
//        uart_putstr(uartn, "AT+LED1");
//}
