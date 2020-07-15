/**
 * @file magnet
 * @brief 电磁传感器
 */
#ifndef MAGNET_H
#define MAGNET_H

#include "filter.hpp"
#include "util.h"

class MagSensor {
    friend void storage_load(uint32 sector, uint32 page);

  private:
    //    filter_movAve_t filter_RawData;
    float fValue;
    float RawData, MaxRawData = 1;
    void Normalize();

  public:
    void Sample(float raw);

    //! @brief access private readonly
    __inline float GetRaw() { return RawData; }
    __inline float GetNormalized() { return fValue; }
    __inline float GetVoltage() { return ADC12b_TO_mV(RawData); }
    __inline float GetMax() { return MaxRawData; }
    __inline void ClearMax() { MaxRawData = 0; }
};

//! @brief 平移电感记录归一化用的最大值
extern bool SaveMaxEnabled;

//! @brief 通过前瞻测到的三个电磁值,一定处理后得到的偏差值,作为舵机pid中的error.
extern volatile float MagErrorForPID;

void magnet_init();

#endif
