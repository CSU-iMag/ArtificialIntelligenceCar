#include "car.hpp"
#include "MCP4452.h"
#include "SEEKFREE_IIC.h"
#include "bat.hpp"
#include "communication.hpp"
#include "deep.hpp"
#include "fsl_src.h"
#include "gui.hpp"
#include "storage.hpp"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"
#include "zf_flash.h"
#include "zf_pit.h"
#include "zf_uart.h"

//! @warning 全局唯一！
AT_SDRAM_SECTION(iMagCar Car);

void iMagCar::Startup() {
    DisableGlobalIRQ();
    board_init(); //!< before all
    CAR_ERROR_CHECK(flash_init() == 0);
    led_init();
    key_init();
    CarOLED.fnInitialize();  //!< after key
    ActiveLayout->Repaint(); //!< after oled
    MotorL.Init();
    MotorR.Init();
    Steer3010.Init();
    bat_init();
    deep_init();
       magnet_init();
    cpu_usage_init();
    EnableGlobalIRQ(0);
    storage_load(SLN_DEBUG_SECTOR, SLN_DEBUG_PAGE); //!< after MCP4452
    gui_motor.upd_tmr.Start(62);
    com_log("Startup Successfully!\n", LogGreen);
}

void iMagCar::Run() {
    cpu_usage_count();
    sched_execute();
}

void iMagCar::Launch() {
    PIT_StartTimer(PIT, PIT_MOTOR_CH);
    PIT_StartTimer(PIT, PIT_STEER_CH);
}

void iMagCar::Pause() {
    Car.MotorL.Stop();
    Car.MotorR.Stop();
    pit_close(PIT_MOTOR_CH);
    pit_close(PIT_STEER_CH);
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
    Car.Machine.Start();
}
