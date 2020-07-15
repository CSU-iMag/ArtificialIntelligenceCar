/*********电磁检测*********/
#include "magnet.hpp"
#include "car.hpp"
#include "communication.hpp"
#include "gui.hpp"
#include "peripherals.h"
#include "util.h"
#include <arm_math.h>

std::vector<MagSensor> iMagCar::MagList(ADC_CNT);

bool SaveMaxEnabled;
volatile float MagErrorForPID;

void MagSensor::Sample(float raw) {
    RawData = raw;
    //    RawData = filter_RawData.Moving(RawData);
    if (SaveMaxEnabled)
        MaxRawData = std::max(MaxRawData, RawData);
    Normalize();
}

void MagSensor::Normalize() {
    if (SaveMaxEnabled)
        return;
    CAR_ERROR_CHECK(MaxRawData);
    CRITICAL_REGION_ENTER();
    fValue = RESCALE_VALUE((float)RawData, 255, MaxRawData);
    LIMITING(fValue, 0.001f, 254.999f);
    CRITICAL_REGION_EXIT();
}

static __inline void DerailProtect(void) {
    if (IS_DERAIL) {
        Car.Machine.DetectException();
    }
}

static void UpdateGUI() {
    static int cnt;
    if (++cnt %= 26) // 12分频
        return;
    gui_steering.err_curve.AppendValue(MagErrorForPID * 9);
    gui_magadcDat.UpdateValue();
    // gui_debug.UpdateValue(
    //     1, std::to_string(Car.MagList[MagL_FRONT].GetNormalized() +
    //                       Car.MagList[MagM_FRONT].GetNormalized() +
    //                       Car.MagList[MagR_FRONT].GetNormalized()));
}

void AnalysePackage() {
    if (SLAVE_rxBuffer[0] != SOH && SLAVE_rxBuffer[18] != EOT &&
        AccParity(SLAVE_rxBuffer + 1, 16) != SLAVE_rxBuffer[17])
        return;
    for (int i(0); i < 8; ++i)
        Car.MagList[i].Sample((SLAVE_rxBuffer[i + 1]));
    for (int i(0); i < 8; i += 2)
        Car.MagList[i / 2 + 8].Sample(((uint16_t)SLAVE_rxBuffer[i + 10] << 8) |
                                      SLAVE_rxBuffer[i + 9]);
}

void magnet_sched(sched_event_data_t dat) {
    AnalysePackage();
    DerailProtect();

    CRITICAL_REGION_ENTER();
    MagErrorForPID =
        10 *
        (Car.MagList[MagFrontL].GetNormalized() -
         Car.MagList[MagFrontR].GetNormalized() + Car.Steer3010.steerOffset) /
        (Car.MagList[MagFrontL].GetNormalized() +
         Car.MagList[MagFrontM].GetNormalized() +
         Car.MagList[MagFrontR].GetNormalized());
    CRITICAL_REGION_EXIT();

    UpdateGUI();
}

void SlaveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                   void *userData) {
    if (status != kStatus_LPUART_IdleLineDetected)
        return;
    sched_event_put(magnet_sched);
    LPUART_TransferAbortReceive(SLAVE_PERIPHERAL, &SLAVE_handle);
    LPUART_TransferReceiveNonBlocking(SLAVE_PERIPHERAL, &SLAVE_handle,
                                      (lpuart_transfer_t *)&SLAVE_rxTransfer,
                                      NULL);
}

void magnet_init(void) {
    LPUART_TransferReceiveNonBlocking(SLAVE_PERIPHERAL, &SLAVE_handle,
                                      (lpuart_transfer_t *)&SLAVE_rxTransfer,
                                      NULL);
}
