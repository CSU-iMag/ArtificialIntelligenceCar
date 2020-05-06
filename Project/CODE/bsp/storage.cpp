#include "storage.hpp"
#include "car.hpp"
#include "car_config.h"
#include "core_cm7.h"
#include "gui.hpp"
#include "pid.hpp"
#include "util.h"
#include "zf_flash.h"

struct StorageData {
    uint8_t Gain[PGA_CNT];
    uint16_t MagMax[ADC_CNT];
    float32_t TargetSpeed, SteerOffset;
    pid_coefficient_t SteerCoeff, SpeedCoeff;
} __PACKED;

static constexpr size_t storage_size(sizeof(StorageData));
static_assert(storage_size % 4 == 0, "buf must align in word");
static_assert(storage_size <= FLASH_PAGE_SIZE,
              "flash program cannot cross page");

union Saver {
    StorageData data;
    uint32 buffer[storage_size];
} static store;

void storage_save(uint32 sector, uint32 page) {
    CRITICAL_REGION_ENTER();
    for (int i(0); i < PGA_CNT; ++i)
        store.data.Gain[i] = dynamic_cast<MagSensorPGA *>(Car.MagList[i])->Gain;
    for (int i(0); i < ADC_CNT; ++i)
        store.data.MagMax[i] = Car.MagList[i]->MaxRawData;
    store.data.TargetSpeed = Car.TargetSpeed;
    store.data.SteerOffset = Car.Steer3010.steerOffset;
    store.data.SteerCoeff.Kp = Car.Steer3010.steerCtrl.instance.Kp;
    store.data.SteerCoeff.Ki = Car.Steer3010.steerCtrl.instance.Ki;
    store.data.SteerCoeff.Kd = Car.Steer3010.steerCtrl.instance.Kd;
    store.data.SpeedCoeff.Kp = Car.MotorL.speedCtrl.instance.Kp;
    store.data.SpeedCoeff.Ki = Car.MotorL.speedCtrl.instance.Ki;
    store.data.SpeedCoeff.Kd = Car.MotorL.speedCtrl.instance.Kd;
    CRITICAL_REGION_EXIT();

    flash_erase_sector(sector);
    flash_page_program(sector, page, store.buffer, storage_size);
}

void storage_load(uint32 sector, uint32 page) {
    // SCB_InvalidateDCache();
    flash_read_page(sector, page, store.buffer, storage_size / 4);

    CRITICAL_REGION_ENTER();
    for (int i(0); i < PGA_CNT; ++i)
        dynamic_cast<MagSensorPGA *>(Car.MagList[i])->Gain = store.data.Gain[i];
    for (int i(0); i < ADC_CNT; ++i)
        Car.MagList[i]->MaxRawData = store.data.MagMax[i];
    // Car.TargetSpeed = store.data.TargetSpeed;
    Car.Steer3010.steerOffset = store.data.SteerOffset;
    Car.Steer3010.steerCtrl.SetPID(store.data.SteerCoeff.Kp,
                                   store.data.SteerCoeff.Ki,
                                   store.data.SteerCoeff.Kd);
    Car.MotorL.speedCtrl.SetPID(store.data.SpeedCoeff.Kp,
                                store.data.SpeedCoeff.Ki,
                                store.data.SpeedCoeff.Kd);
    Car.MotorR.speedCtrl.SetPID(store.data.SpeedCoeff.Kp,
                                store.data.SpeedCoeff.Ki,
                                store.data.SpeedCoeff.Kd);
    CRITICAL_REGION_EXIT();

    gui_reloadVal();
    DEBUG_LOG("target speed=%f\n", Car.TargetSpeed);
    DEBUG_LOG("steer offset=%f\n", Car.Steer3010.steerOffset);
    DEBUG_LOG("steer pid: %f %f %f\n", Car.Steer3010.steerCtrl.instance.Kp,
              Car.Steer3010.steerCtrl.instance.Ki,
              Car.Steer3010.steerCtrl.instance.Kd);
    DEBUG_LOG("speed pid: %f %f %f\n", Car.MotorL.speedCtrl.instance.Kp,
              Car.MotorL.speedCtrl.instance.Ki,
              Car.MotorL.speedCtrl.instance.Kd);
              PRINTF("\nPGA:");
              for (int i(0); i < PGA_CNT; ++i)
                PRINTF(" %d", dynamic_cast<MagSensorPGA *>(Car.MagList[i])->Gain);
                PRINTF("\n\nRawMax:");
                 for (int i(0); i < ADC_CNT; ++i)
       PRINTF(" %d", Car.MagList[i]->MaxRawData);
}
