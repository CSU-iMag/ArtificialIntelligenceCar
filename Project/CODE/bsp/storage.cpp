#include "storage.hpp"
#include "car.hpp"
#include "car_config.h"
#include "communication.hpp"
#include "core_cm7.h"
#include "gui.hpp"
#include "pid.hpp"
#include "util.h"
#include "zf_flash.h"

using std::to_string;
struct StorageData {
    uint16_t MagMax[ADC_CNT], MagMin[ADC_CNT];
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
    for (int i(0); i < ADC_CNT; ++i) {
        store.data.MagMax[i] = Car.MagList[i].GetMax();
        store.data.MagMin[i] = Car.MagList[i].GetMin();
    }
    store.data.TargetSpeed = Car.TargetSpeed;
    store.data.SteerOffset = Car.Steer3010.steerOffset;
    store.data.SteerCoeff.Kp = Car.Steer3010.steerCtrl.coeff.Kp;
    store.data.SteerCoeff.Ki = Car.Steer3010.steerCtrl.coeff.Ki;
    store.data.SteerCoeff.Kd = Car.Steer3010.steerCtrl.coeff.Kd;
    store.data.SpeedCoeff.Kp = Car.MotorL.speedCtrl.instance.Kp;
    store.data.SpeedCoeff.Ki = Car.MotorL.speedCtrl.instance.Ki;
    store.data.SpeedCoeff.Kd = Car.MotorL.speedCtrl.instance.Kd;
    CRITICAL_REGION_EXIT();

    flash_erase_sector(sector);
    flash_page_program(sector, page, store.buffer, storage_size / 4);
}

void storage_load(uint32 sector, uint32 page) {
    flash_read_page(sector, page, store.buffer, storage_size / 4);

    CRITICAL_REGION_ENTER();
    for (int i(0); i < ADC_CNT; ++i) {
        Car.MagList[i].MaxRaw = store.data.MagMax[i];
        Car.MagList[i].MinRaw = store.data.MagMin[i];
    }
    Car.TargetSpeed = store.data.TargetSpeed;
    Car.Steer3010.steerOffset = store.data.SteerOffset;
    Car.Steer3010.steerCtrl.SetPID(store.data.SteerCoeff.Kp,
                                   store.data.SteerCoeff.Ki,
                                   //    0.01,
                                   store.data.SteerCoeff.Kd);
    Car.MotorL.speedCtrl.SetPID(store.data.SpeedCoeff.Kp,
                                store.data.SpeedCoeff.Ki,
                                store.data.SpeedCoeff.Kd);
    Car.MotorR.speedCtrl.SetPID(store.data.SpeedCoeff.Kp,
                                store.data.SpeedCoeff.Ki,
                                store.data.SpeedCoeff.Kd);
    CRITICAL_REGION_EXIT();

    gui_reloadVal();
    com_log("\nLoadParam fromFlash\n", LogBlue);
    com_log("\ntarget speed=");
    com_log(to_string(Car.TargetSpeed));
    com_log("\nsteer offset=");
    com_log(to_string(Car.Steer3010.steerOffset));
    com_log("\nsteer pid:");
//    com_log(to_string(STEER_K(p)) + ' ');
//    com_log(to_string(STEER_K(i)) + ' ');
//    com_log(to_string(STEER_K(d)) + ' ');
    com_log("\nspeed pid:");
    com_log(to_string(SPEED_K(L, p)) + ' ');
    com_log(to_string(SPEED_K(L, i)) + ' ');
    com_log(to_string(SPEED_K(L, d)) + ' ');
    // com_log("\nRawMax:");
    // for (int i(0); i < ADC_CNT; ++i)
    //     com_log(to_string(Car.MagList[i].MaxRaw) + ' ');
    com_log("\n==============\n", LogBlue);
}
