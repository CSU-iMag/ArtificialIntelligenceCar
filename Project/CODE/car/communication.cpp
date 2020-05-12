#include "communication.hpp"
#include "SEEKFREE_VIRSCO.h"
#include "car.hpp"
#include "fsl_iomuxc.h"
#include "magnet.hpp"
#include "pack.hpp"
#include "peripherals.h"
#include "timer.hpp"
#include "util.h"

void com_send(sched_event_data_t dat) {
    data_conversion(Car.EncoderL.GetSpeed(), Car.EncoderR.GetSpeed(), 0, 0,
                    communicate_txBuffer);

    LPUART_TransferSendNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_txTransfer);
}

extern "C" void communicate_init(void);
void com_rx_sched(sched_event_data_t dat) {
    // for (int i(0); i < 20; ++i)
    //     DEBUG_LOG("%x ", communicate_rxBuffer[i]);
    // DEBUG_LOG("\n");
    // auto count(communicate_handle.rxDataSizeAll -
    //            communicate_handle.rxDataSize);
    // RxPack_Handle(communicate_rxBuffer, count);
    Unpack();

    CRITICAL_REGION_ENTER();
    LPUART_SoftwareReset(COMMUNICATE_PERIPHERAL);
    communicate_init();
    LPUART_TransferReceiveNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_rxTransfer, NULL);
    CRITICAL_REGION_EXIT();
}

void com_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                  void *userData) {
    if (kStatus_LPUART_IdleLineDetected == status) {
        // communicate_rxBuffer[18] = '\0';
        // DEBUG_LOG("%s\n", communicate_rxBuffer);
        sched_event_put(com_rx_sched);
    }
    // DEBUG_LOG("uart call sta=%d\n", status);
}

/**
 * @brief 发送上行包
 */
void Send_Direction_PackData_sched(sched_event_data_t dat) {
    Dir_PD.steerDutyforAI = Car.Steer3010.steerDutyforAI;
//    Dir_PD.steerPID_Input = Car.Steer3010.steerPID_Input;
    Dir_PD.magL_Row1_fValue = Car.MagList[9]->GetNormalized();
    Dir_PD.magM_Row1_fValue = Car.MagList[10]->GetNormalized();
    Dir_PD.magR_Row1_fValue = Car.MagList[11]->GetNormalized();
    Dir_PD.parity = AccParity((uint8_t *)&Dir_PD + 2, Dir_PD.size);
    memcpy(communicate_txBuffer, (uint8 *)&Dir_PD, Dir_PD.size + 4);
    communicate_txBuffer[Dir_PD.size + 4] = '\n';
    LPUART_TransferSendNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_txTransfer);
}

void Send_Speed_PackData_sched(sched_event_data_t dat) {
    Speed_PD.speedLeft_H = ((int16_t)Car.EncoderL.GetSpeed() >> 8) & (0x0FF);
    Speed_PD.speedLeft_L = ((int16_t)Car.EncoderL.GetSpeed()) & (0x0FF);
    Speed_PD.speedRight_H = ((int16_t)Car.EncoderR.GetSpeed() >> 8) & (0x0FF);
    Speed_PD.speedRight_L = ((int16_t)Car.EncoderR.GetSpeed()) & (0x0FF);
    Speed_PD.dutyLeft = (uint8_t)Car.MotorL.duty;
    Speed_PD.dutyRight = (uint8_t)Car.MotorR.duty;
    Speed_PD.parity = AccParity((uint8_t *)&Speed_PD + 2, Speed_PD.size);
    memcpy(communicate_txBuffer, (uint8 *)&Speed_PD, Speed_PD.size + 4);
    communicate_txBuffer[Speed_PD.size + 4] = '\n';
    LPUART_TransferSendNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_txTransfer);
}

void Send_AI_PackData_sched(sched_event_data_t dat) {
    AI_PD.steerDutyforAI = (uint8_t)Car.Steer3010.steerDutyforAI;
    AI_PD.MagL_Row2 = (uint8_t)Car.MagList[MagL_ROW2]->GetNormalized();
    AI_PD.MagLM_Row2 = (uint8_t)Car.MagList[MagLM_ROW2]->GetNormalized();
    AI_PD.MagM_Row2 = (uint8_t)Car.MagList[MagM_ROW2]->GetNormalized();
    AI_PD.MagRM_Row2 = (uint8_t)Car.MagList[MagRM_ROW2]->GetNormalized();
    AI_PD.MagR_Row2 = (uint8_t)Car.MagList[MagR_ROW2]->GetNormalized();
    AI_PD.MagL_Row3 = (uint8_t)Car.MagList[MagL_ROW3]->GetNormalized();
    AI_PD.MagR_Row3 = (uint8_t)Car.MagList[MagR_ROW3]->GetNormalized();
    Crc32Parity((uint8_t *)&AI_PD + 2, AI_PD.size, (uint8_t *)&AI_PD.CRC0);
    memcpy(communicate_txBuffer, (uint8 *)&AI_PD, AI_PD.size + 7);
    communicate_txBuffer[AI_PD.size + 7] = '\n';
    LPUART_TransferSendNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_txTransfer);
}

/**
 * @brief 上行包的定时发送softtimer对象的声明
 */
SoftTimer Send_Direction_PackData_tim(Send_Direction_PackData_sched);
SoftTimer Send_Speed_PackData_tim(Send_Speed_PackData_sched);
SoftTimer Send_AI_PackData_tim(Send_AI_PackData_sched);

SoftTimer VisualScopeTmr(com_send);

void com_init() {
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TX, /* GPIO_B1_12 is configured
                                                      as LPUART5_TX */
                     0U); /* Software Input On Field: Input Path is determined
                             by functionality */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RX, /* GPIO_B1_13 is configured
                                                      as LPUART5_RX */
                     0U);
    LPUART_TransferStopRingBuffer(COMMUNICATE_PERIPHERAL, &communicate_handle);
    LPUART_TransferReceiveNonBlocking(
        COMMUNICATE_PERIPHERAL, &communicate_handle,
        (lpuart_transfer_t *)&communicate_rxTransfer, NULL);
}
