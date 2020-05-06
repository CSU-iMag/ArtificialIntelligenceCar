/*********电磁检测*********/
#include "magnet.hpp"
#include "beep.hpp"
#include "car.hpp"
#include "filter.hpp"
#include "fsl_gpt.h"
#include "gui.hpp"
#include "pid.hpp"
#include "route.h"
#include "timer.hpp"
#include "util.h"
#include "zf_adc.h"
#include <arm_math.h>

SECTION_SDRAM MagSensorPGA PGA_A1(ADC1_CH7_B18, pga_channels::A1);
SECTION_SDRAM MagSensorPGA PGA_A2(ADC1_CH8_B19, pga_channels::A2);
SECTION_SDRAM MagSensorPGA PGA_A3(ADC1_CH10_B21, pga_channels::A3);
SECTION_SDRAM MagSensorPGA PGA_A4(ADC1_CH9_B20, pga_channels::A4);
SECTION_SDRAM MagSensorPGA PGA_B1(ADC1_CH14_B25, pga_channels::B1);
SECTION_SDRAM MagSensorPGA PGA_B2(ADC1_CH13_B24, pga_channels::B2);
SECTION_SDRAM MagSensorPGA PGA_B3(ADC1_CH0_B27, pga_channels::B3);
SECTION_SDRAM MagSensorPGA PGA_B4(ADC1_CH11_B22, pga_channels::B4);

SECTION_SDRAM MagSensor OPA_1(ADC1_CH6_B17);
SECTION_SDRAM MagSensor OPA_2(ADC1_CH5_B16);
SECTION_SDRAM MagSensor OPA_3(ADC1_CH3_B14);
SECTION_SDRAM MagSensor OPA_4(ADC1_CH4_B15);

MagSensor *iMagCar::MagList[ADC_CNT] = {&PGA_A1, &PGA_A2, &PGA_A3, &PGA_A4,
                                        &PGA_B1, &PGA_B2, &PGA_B3, &PGA_B4,
                                        &OPA_1,  &OPA_2,  &OPA_3,  &OPA_4};

bool SaveMaxEnabled;
volatile float MagErrorForPID;

MagSensor::MagSensor(const ADCCH_enum &adc)
    : filter_RawData(FILTER_MAGNET_SIZE), Channel(adc), MaxRawData(1) {}

void MagSensor::Init() { adc_init(MAGNET_ADC, Channel, MAG_SAMPLE_RESOLUTION); }

void MagSensor::Sample() {
    RawData = adc_mean_filter(MAGNET_ADC, Channel, 10);
    // RawData = filter_RawData.filter_movingAverage(RawData);
    if (SaveMaxEnabled)
        MaxRawData = std::max(MaxRawData, RawData);
}

void MagSensor::NormalizeForPID() {
    if (SaveMaxEnabled)
        return;
    CAR_ERROR_CHECK(MaxRawData);
    CRITICAL_REGION_ENTER();
    fValue = RawData * 100 / MaxRawData;
    LIMITING(fValue, 0.001f, 99.999f);
    CRITICAL_REGION_EXIT();
}

void MagSensor::NormalizeForAI() {
    if (SaveMaxEnabled)
        return;
    CAR_ERROR_CHECK(MaxRawData);
    CRITICAL_REGION_ENTER();
    fValue = RESCALE_VALUE((float)RawData, 255, MaxRawData);
    LIMITING(fValue, 0.001f, 254.999f);
    CRITICAL_REGION_EXIT();
}

MagSensorPGA::MagSensorPGA(const ADCCH_enum &adc, const pga_channels &res)
    : MagSensor(adc), Gain(res) {}

bool MagSensorPGA::Check(uint8_t val) {
    Gain = val;
    RawData = adc_mean_filter(MAGNET_ADC, Channel, 9);
    return RawData < MaxRawData - 12;
}

void MagSensorPGA::Calibrate() {
    GPT_StopTimer(GPT2);
    Gain = 255;
    MaxRawData = adc_mean_filter(MAGNET_ADC, Channel, 10);

    for (int_fast16_t left(0), right(255), middle(128); left < right;
         middle = left + ((left - right) >> 1))
        if (Check(middle))
            left = middle + 1;
        else
            right = middle;

    DEBUG_LOG("PGA_Calibrate GAIN:%d\n", Gain);
    GPT_StartTimer(GPT2);
    Car.beep0.BeepFreqDelay(2333, 666);
}

static __inline void DerailProtect(void) {
    if (IS_DERAIL) {
        Car.Machine.DetectException();
    }
}

void mag_sample_sched(sched_event_data_t dat) {
    uint8 i = 0;
    for (; i < 9; i++) {
        Car.MagList[i]->Sample();
        Car.MagList[i]->NormalizeForAI();
    }
    for (; i < ADC_CNT; i++) {
        Car.MagList[i]->Sample();
        Car.MagList[i]->NormalizeForPID();
    }
    DerailProtect();

    float sL, sR;
    arm_sqrt_f32(Car.MagList[MagL_FRONT]->GetNormalized(), &sL);
    arm_sqrt_f32(Car.MagList[MagR_FRONT]->GetNormalized(), &sR);

    CRITICAL_REGION_ENTER();
    //    sL += Car.Steer3010.steerOffset;
    //    sR -= Car.Steer3010.steerOffset;

    //前瞻左右电感间距12cm，距离地面高度15cm
    // first 原本的，先试试有木有错，我怕了我自己
    // MagErrorForPID = 1000 * (sL - sR + Car.Steer3010.steerOffset) /
    //                  (Car.MagList[MagL_FRONT]->GetNormalized() +
    //                   Car.MagList[MagM_FRONT]->GetNormalized() +
    //                   Car.MagList[MagR_FRONT]->GetNormalized())/
    //                  (Car.MagList[MagM_FRONT]->GetNormalized());
    // second
    //距离小的时候给的err比first大，距离大的时候比first小，
    //下一个公式同理，只是各种程度不同
    //                     ↓这个参数可能需要稍微改下
    MagErrorForPID =
        10 *
        (Car.MagList[MagL_FRONT]->GetNormalized() -
         Car.MagList[MagR_FRONT]->GetNormalized() + Car.Steer3010.steerOffset) /
        (Car.MagList[MagL_FRONT]->GetNormalized() +
         Car.MagList[MagM_FRONT]->GetNormalized() +
         Car.MagList[MagR_FRONT]->GetNormalized());
    // third
    //    MagErrorForPID = 20 * (Car.MagList[MagL_FRONT]->GetNormalized() -
    //		                  Car.MagList[MagR_FRONT]->GetNormalized() +
    //											Car.Steer3010.steerOffset)
    ///
    //                     (Car.MagList[MagL_FRONT]->GetNormalized() +
    //                      Car.MagList[MagM_FRONT]->GetNormalized() +
    //                      Car.MagList[MagR_FRONT]->GetNormalized())/
    //                     (Car.MagList[MagM_FRONT]->GetNormalized());

    CRITICAL_REGION_EXIT();

    gui_steering.err_curve.AppendValue(MagErrorForPID * 9);
    gui_magadcDat.UpdateValue();
}

static SoftTimer mag_sample_tim(mag_sample_sched);

void magnet_init(void) {
    for (int i(0); i < ADC_CNT; ++i)
        Car.MagList[i]->Init();
    mag_sample_tim.Start(MAG_SAMPLE_PERIOD);
}
