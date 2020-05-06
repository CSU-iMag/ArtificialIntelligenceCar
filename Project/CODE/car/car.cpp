#include "car.hpp"
#include "MCP4452.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"
#include "bat.hpp"
#include "communication.hpp"
#include "fsl_src.h"
#include "gui.hpp"
#include "pack.hpp"
#include "rit.hpp"
#include "storage.hpp"
#include "timer.hpp"
#include "usage.hpp"
#include "util.h"
#include "deep.hpp"
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
    EncoderL.Init();
    EncoderR.Init();
    MotorL.Init();
    MotorR.Init();
    Steer3010.Init();
    Hc06.Init();
    beep0.Init();
    ActiveLayout->Repaint(); //!< after oled
    bat_init();
    com_init();
    rit_init();
    pack_Init();
    deep_init();
    magnet_init();
    cpu_usage_init();
    MCP4452_all_init(); //!< after i2c
    DEBUG_LOG("MCP4452 Check: %d\n", MCP4452_self_check());
    storage_load(SLN_DEBUG_SECTOR, SLN_DEBUG_PAGE); //!< after MCP4452
    gpio_init(MOTOR_EN_PIN, GPO, 1, GPIO_PIN_CONFIG);

    dcp_config_t dcpConfig;
//    /* Init hardware 例程里有 不知道是否一定需要*/
//    BOARD_InitPins();
//    /* Data cache must be temporarily disabled to be able to use sdram 干啥的*/
//    SCB_DisableDCache();
    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);
//#if DCP_TEST_USE_OTP_KEY
//    /* Set OTP key type in IOMUX registers before initializing DCP. */
//    /* Software reset of DCP must be issued after changing the OTP key type. */
//    DCP_OTPKeySelect(kDCP_OTPMKKeyLow);
//#endif
    /* Reset and initialize DCP */
    DCP_Init(DCP, &dcpConfig);

    EnableGlobalIRQ(0);
    beep0.Mute();
    DEBUG_LOG("Startup Successfully!\n");
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
//    pit_close(STEER_CHANNEL);
}

iMagCar::~iMagCar() {
    Steer3010.~Steer();
    CAR_ERROR_CHECK(0)
}

void LaunchDelaySchedule(sched_event_data_t dat) {
    static uint8_t cnt;
    Car.beep0.BeepFreq(beep_musicFreq[++cnt + 3]);
    if (cnt < 3)
        return;
    Car.beep0.BeepFreqDelay(beep_musicFreq[6], 666);
    cnt = 0;
    Car.Machine.Start();
}
