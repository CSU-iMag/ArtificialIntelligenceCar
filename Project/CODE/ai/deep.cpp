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

float deep_predict() {
    GetData();
    int16_t g_servoValue;
    CI_RunModelXIP(Car.Model, cie_data, &g_servoValue); // 调用模型计算转角

    // 根据获取到的模型参数对计算结果进行右移位
    if (info.quantBits > 7) 
        g_servoValue >>= (info.quantBits - pOM->fracBits - 1);

    return RESCALE_VALUE((float)g_servoValue, STEER_MAX - STEER_MIN, 255) +
           STEER_CENTER;
}
