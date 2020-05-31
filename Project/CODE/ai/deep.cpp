#include "deep.hpp"
#include "car.hpp"
#include "nncie.h"
#include "scheduler.hpp"
#include "util.h"

static int8_t cie_data[7];
static ModelInfo_t info;
static const CI_OutMetricInfo *pOM;

void deep_init() {
    pOM = CI_GetOutMetricInfo(0);
    CI_GetModelInfoXIP(Car.Model, &info);
    DEBUG_LOG("model info: %d , %s\n", info.quantBits, info.pszString);
}

__STATIC_INLINE void GetData() {
    cie_data[0] = Car.MagList[MagL_ROW2]->GetNormalized() - 128;
    cie_data[1] = Car.MagList[MagLM_ROW2]->GetNormalized() - 128;
    cie_data[2] = Car.MagList[MagM_ROW2]->GetNormalized() - 128;
    cie_data[3] = Car.MagList[MagRM_ROW2]->GetNormalized() - 128;
    cie_data[4] = Car.MagList[MagR_ROW2]->GetNormalized() - 128;
    cie_data[5] = Car.MagList[MagL_ROW3]->GetNormalized() - 128;
    cie_data[6] = Car.MagList[MagR_ROW3]->GetNormalized() - 128;
}

void RunModel(sched_event_data_t dat) {
    GetData();
    int16_t temp;
    CI_RunModelXIP(Car.Model, cie_data, &temp); //调用模型计算转角

    //根据获取到的模型参数对计算结果进行右移位
    int16_t g_servoValue;
    if (info.quantBits > 7) {
        g_servoValue = temp >> (info.quantBits - pOM->fracBits - 1);
    } else {
        g_servoValue = temp;
    }

    float steering =
        RESCALE_VALUE((float)g_servoValue, STEER_MAX - STEER_MIN, 255) +
        STEER_CENTER;
    Car.Steer3010.WidthSet(steering);
}

// <o> 舵机中值 <0-10000>
//  STEER_CENTER 1265    
// <o> 最右边 <0-10000>
//  STEER_MIN 1107
// <o> 最左边 <0-10000>
//  STEER_MAX 1412
//	1107+158=1265		1265+147=1412
//	158+147=305
void RunModel_classify40(sched_event_data_t dat) {
    GetData();
    int16_t temp;
    CI_RunModelXIP(Car.Model, cie_data, &temp); //调用模型计算转角

    //根据获取到的模型参数对计算结果进行右移位
    int16_t g_servoValue;
    if (info.quantBits > 7) {
        g_servoValue = temp >> (info.quantBits - pOM->fracBits - 1);
    } else {
        g_servoValue = temp;
    }

    float steering =
        RESCALE_VALUE((float)g_servoValue, STEER_MAX - STEER_MIN, 255) +
        STEER_CENTER;
    Car.Steer3010.WidthSet(steering);
}

void deep_schedule() {
    if (Car.CtrlMode == ControlMode::AI)
        sched_event_put(RunModel);
}
