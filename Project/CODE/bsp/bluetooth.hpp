/**
 * @file Bluetooth
 * @brief HC06
 */
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "car_config.h"
#include "route.h"
#include <string>
struct Baud_t {
    char str;
    uint32 baudrate;
};

class Bluetooth {
    UARTN_enum uartn;
    UARTPIN_enum tx_pin;
    UARTPIN_enum rx_pin;

  public:
    uint8 rx_byte;
    std::string rx_buffer;
	const Baud_t *baud;//为啥加const？
    std::string password;
    bool isMaster;
    uint8 parity_bit;

    Bluetooth(UARTN_enum uartn_t, UARTPIN_enum tx_pin_t, UARTPIN_enum rx_pin_t)
        : uartn(uartn_t), tx_pin(tx_pin_t), rx_pin(rx_pin_t){};
    void Init();
    void TestComunication();
    void ModifyBaud(int8 varia);
    void SendPassword();
    void SetRole(int8 option);
    void SendRole();
    void ModifyValid(int8 varia);
    void GetVersion();
    //    void ReceiveByte();
    //    void ConvertLED();
};
void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                   void *userData, uint8 *buffer);

#endif
