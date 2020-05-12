#include "encoder.hpp"
#include "car.hpp"
#include "car_config.h"
#include "filter.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"

#define TMR_CNTR                                                               \
    (PULSEENCODER_PERIPHERAL->CHANNEL[static_cast<int>(Channel)].CNTR)

//-------------------------------------------------------------------------------------------------------------------
//  @name     编码器初始化
//  @sample   Motor
//  MotorL(MOTOR_L,encoderL_QTIMER,encoderL_LSB_QTIMER,encoderL_DIR_QTIMER,
//                         0.5,0.2,0.2,MOTOR_MIN,MOTOR_MAX,MOTOR_MIN);
//-------------------------------------------------------------------------------------------------------------------
Encoder::Encoder(qtmr_channel_selection_t QTIMER_Channel)
    : Channel(QTIMER_Channel), filter_moving(3) {}

//-------------------------------------------------------------------------------------------------------------------
//  @name     获取速度（单位cm/s）
//-------------------------------------------------------------------------------------------------------------------
__INLINE void Encoder::CalSpeed() {
    //读取编码器计数值
    speed = PULSE_TO_CM_S(static_cast<int16_t>(TMR_CNTR));
    TMR_CNTR = 0;
    speed = filter_moving.Moving(speed);
}

//-------------------------------------------------------------------------------------------------------------------
//  @name     获取距离（单位cm）
//  @Sample   MotorL.GetSpeed();
//-------------------------------------------------------------------------------------------------------------------
inline void Encoder::CalDistance() {
    distance += speed * ENCODER_PERIOD / 1000;
}

/** -------------------------------------------------------------------------------------------------------------------
 *  @name     速度pid控制左右电机
 */
void pulse_encoder_schedule() {
    CRITICAL_REGION_ENTER();
    Car.EncoderL.CalSpeed();
    Car.EncoderR.CalSpeed();
    CRITICAL_REGION_EXIT();

    Car.EncoderL.speed = -Car.EncoderL.speed;

    Car.EncoderL.CalDistance();
    Car.EncoderR.CalDistance();
    if (IS_STEADY)
        Car.Machine.Steady();
    if (IS_STOPPED)
        Car.Machine.Stopped();
}

void tmr_init() {
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_00_QTIMER1_TIMER0, /* GPIO_B0_00 is configured as
                                             QTIMER1_TIMER0 */
        0U); /* Software Input On Field: Input Path is determined by
                functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_01_QTIMER1_TIMER1, /* GPIO_B0_01 is configured as
                                             QTIMER1_TIMER1 */
        0U); /* Software Input On Field: Input Path is determined by
                functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_02_QTIMER1_TIMER2, /* GPIO_B0_02 is configured as
                                             QTIMER1_TIMER2 */
        0U); /* Software Input On Field: Input Path is determined by
                functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_08_QTIMER1_TIMER3, /* GPIO_B1_08 is configured as
                                             QTIMER1_TIMER3 */
        0U);
    IOMUXC_GPR->GPR6 =
        ((IOMUXC_GPR->GPR6 &
          (~(IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL_MASK |
             IOMUXC_GPR_GPR6_QTIMER1_TRM1_INPUT_SEL_MASK |
             IOMUXC_GPR_GPR6_QTIMER1_TRM2_INPUT_SEL_MASK |
             IOMUXC_GPR_GPR6_QTIMER1_TRM3_INPUT_SEL_MASK |
             IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero
                                                               which are setting
                                                             */
         | IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL(
               0x00U) /* QTIMER1 TMR0 input select: input from IOMUX */
         | IOMUXC_GPR_GPR6_QTIMER1_TRM1_INPUT_SEL(
               0x00U) /* QTIMER1 TMR1 input select: input from IOMUX */
         | IOMUXC_GPR_GPR6_QTIMER1_TRM2_INPUT_SEL(
               0x00U) /* QTIMER1 TMR2 input select: input from IOMUX */
         | IOMUXC_GPR_GPR6_QTIMER1_TRM3_INPUT_SEL(
               0x00U) /* QTIMER1 TMR3 input select: input from IOMUX */
         | IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL(
               0x00U) /* QTIMER2 TMR3 input select: input from IOMUX */
        );
}
