#include "car.hpp"
#include "MCP4452.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"
#include "bat.hpp"
#include "communication.hpp"
#include "deep.hpp"
#include "fsl_src.h"
#include "gui.hpp"
#include "rit.hpp"
#include "storage.hpp"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"
#include "zf_flash.h"

//! @warning å…¨å±€å”¯ä¸€
AT_SDRAM_SECTION(iMagCar Car);

void iMagCar::Startup() {
    DisableGlobalIRQ();
    board_init(); //!< before all
    DEBUG_LOG("\n\r SRC: %d \n", SRC_GetResetStatusFlags(SRC));
    CAR_ERROR_CHECK(flash_init() == 0);
    led_init();
    key_init();
    oled_init(); //!< after key
    simiic_init();
    // mpu6050_init();
    // EncoderL.Init();
    // EncoderR.Init();
    // Hc06.Init();
    MotorL.Init();
    MotorR.Init();
    Steer3010.Init();
    ActiveLayout->Repaint(); //!< after oled
    bat_init();
    com_init();
    rit_init();
    deep_init();
    magnet_init();
    cpu_usage_init();
    MCP4452_all_init(); //!< after i2c
    DEBUG_LOG("MCP4452 Check: %d\n", MCP4452_self_check());

    
    EnableGlobalIRQ(0);
    storage_load(SLN_DEBUG_SECTOR, SLN_DEBUG_PAGE); //!< after MCP4452
    gui_motor.upd_tmr.Start(62);
    beep0.Mute();
    com_log("Startup Successfully!\n", LogGreen);
}

void iMagCar::Run() {
    cpu_usage_count();
    sched_execute();
}

void iMagCar::Launch() {
    pit_interrupt_ms(MOTOR_CHANNEL, MOTOR_PERIOD);
    pit_interrupt_ms(STEER_CHANNEL, STEER_PERIOD);
}

void iMagCar::Pause() {
    Car.MotorL.Stop();
    Car.MotorR.Stop();
    pit_close(MOTOR_CHANNEL);
    pit_close(STEER_CHANNEL);
}

iMagCar::~iMagCar() {
    Steer3010.~Steer();
    CAR_ERROR_CHECK(0)
}

void LaunchDelaySchedule(sched_event_data_t dat) {
    // static uint8_t cnt;
    // Car.beep0.BeepFreq(++cnt * 1000);
    // if (cnt < 3)
    //     return;
    // SongOfJoy.Start();
    // cnt = 0;
    Car.beep0.Mute();
    Car.Machine.Start();
}
