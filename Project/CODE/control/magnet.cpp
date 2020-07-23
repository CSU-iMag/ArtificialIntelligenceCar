#include "car.hpp"
#include "communication.hpp"
#include "gui.hpp"
#include "peripherals.h"
#include "steer.hpp"
#include "util.h"
#include <arm_math.h>

std::vector<MagSensor> iMagCar::MagList(ADC_CNT);

bool SaveMaxEnabled;
volatile float MagErrorForPID;
static uint8_t RingStatus; // 0在环外，1在入环，2在环内，3在出环

void MagSensor::Sample(float raw) {
    RawData = raw;
    //    RawData = filter_RawData.Moving(RawData);
    if (SaveMaxEnabled) {
        MaxRaw = std::max(MaxRaw, RawData);
        MinRaw = std::min(MinRaw, RawData);
    }
    Normalize();
}

void MagSensor::Normalize() {
    if (SaveMaxEnabled)
        return;
    //    CAR_ERROR_CHECK(MaxRaw > MinRaw);
    CRITICAL_REGION_ENTER();
    fValue = RESCALE_VALUE((float)RawData, 255, MaxRaw - MinRaw);
    LIMITING(fValue, 0, 255);
    CRITICAL_REGION_EXIT();
}

__STATIC_INLINE void DerailProtect() {
    if (IS_DERAIL) {
        Car.Machine.DetectException();
    }
}

__STATIC_INLINE bool IsRing() {
    static bool status;
    if (Car.MagList[MagFrontM].GetNormalized() > Car.RingLoader.island)
        status = true;
    if (Car.MagList[MagFrontM].GetNormalized() < Car.RingLoader.island - 5)
        status = false;
    return status;
}

static void UpdateGUI() {
    static int cnt;
    if (++cnt %= 26) // 12分频
        return;
    gui_steering.err_curve.AppendValue(MagErrorForPID * 9);
    gui_magadcDat.UpdateValue();
    gui_ring.UpdateValue(0, RingStatus);
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

void CalcErr() {
    float left, middle(Car.MagList[MagMiddleM].GetNormalized()), right;
    switch (RingStatus) {
    case 1: //第一种情况为即将进入环岛时的偏差计算
        left = Car.MagList[MagLeftL].GetNormalized() - Car.RingLoader.straightL;
        right =
            Car.MagList[MagRightR].GetNormalized() - Car.RingLoader.straightR;
        middle -= Car.RingLoader.island;
        break;
    case 3: //第二种出环岛时的偏差计算，即让车按照直线行驶
        left = Car.RingLoader.straightL;
        right = Car.RingLoader.straightR;
        break;
    default:
        left = Car.MagList[MagLeftL].GetNormalized();
        right = Car.MagList[MagRightR].GetNormalized();
        break;
    }
    MagErrorForPID = 10 * (left - right + Car.Steer3010.steerOffset) /
                     (left + middle + right);
}

__STATIC_INLINE void DetectRingIsland() {
    static bool flag;
    CRITICAL_REGION_ENTER();
    if (flag != IsRing()) {
        flag = IsRing();
        ++RingStatus %= 4;
    }
    CRITICAL_REGION_EXIT();
}

void magnet_sched(sched_event_data_t dat) {
    AnalysePackage();
    DerailProtect();
    DetectRingIsland();
    CalcErr();
    UpdateGUI();
}

#define START_RX                                                               \
    LPUART_TransferReceiveNonBlocking(SLAVE_PERIPHERAL, &SLAVE_handle,         \
                                      (lpuart_transfer_t *)&SLAVE_rxTransfer,  \
                                      NULL)

void SlaveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                   void *userData) {
    if (status != kStatus_LPUART_IdleLineDetected)
        return;
    sched_event_put(magnet_sched);
    LPUART_TransferAbortReceive(SLAVE_PERIPHERAL, &SLAVE_handle);
    START_RX;
}

void magnet_init() { START_RX; }
