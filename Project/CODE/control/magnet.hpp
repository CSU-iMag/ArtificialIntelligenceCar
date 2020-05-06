/**
 * @file magnet
 * @brief 电磁传感器
 */
#ifndef MAGNET_H
#define MAGNET_H

#include "filter.hpp"
#include "pga.hpp"
#include "util.h"
#include "zf_adc.h"

//! @brief 平移电感记录归一化用的最大值
extern bool SaveMaxEnabled;

class MagSensor {
    friend void storage_save(uint32 sector, uint32 page);
    friend void storage_load(uint32 sector, uint32 page);
 
  protected:
    filter_movAve_t filter_RawData;
    ADCCH_enum Channel;
    float fValue;

  public:
    uint16_t RawData, MaxRawData;
    MagSensor(const ADCCH_enum &adc);
    virtual ~MagSensor() = default;
    void Init();
    void Sample();
    void NormalizeForPID();
    void NormalizeForAI();
    //! @brief access private readonly
    __inline uint16_t GetRaw() { return RawData; }
    __inline float GetNormalized() { return fValue; }
    __inline float GetVoltage() { return ADC12b_TO_mV(RawData); }
};

struct MagSensorPGA : MagSensor {
    pga_t Gain; //!< Resistance value for **Programmable Gain Amplifier**

    MagSensorPGA(const ADCCH_enum &adc, const pga_channels &pga);
    //! @brief 自动调整增益防止饱和
    void Calibrate();
    bool Check(uint8_t);
};

//! @brief 通过前瞻测到的三个电磁值,一定处理后得到的偏差值,作为舵机pid中的error.
extern volatile float MagErrorForPID; 

void magnet_init();

#endif
