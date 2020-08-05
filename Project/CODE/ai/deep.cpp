#include "deep.hpp"
#include "car.hpp"
#include "communication.hpp"
#include "nncie.h"
#include "util.h"

static int8_t cie_data[7];
static ModelInfo_t info;
static const CI_OutMetricInfo *pOM;

void deep_init() {
    pOM = CI_GetOutMetricInfo(0);
    CI_GetModelInfoXIP(Car.Model, &info);
    com_log(info.pszString);
    //    DEBUG_LOG("model info: %d , %s\n", info.quantBits, info.pszString);
}

__STATIC_INLINE void GetData() {
    for (int i(0); i < ARRAY_SIZE(MagForDeep); ++i)
        cie_data[i] = Car.MagList[MagForDeep[i]].GetNormalized() - 128;
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
