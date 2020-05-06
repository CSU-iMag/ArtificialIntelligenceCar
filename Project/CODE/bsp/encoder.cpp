#include "encoder.hpp"
#include "car.hpp"
#include "car_config.h"
#include "filter.hpp"
#include "route.h"
#include "util.h"
#include "zf_qtimer.h"

//-------------------------------------------------------------------------------------------------------------------
//  @name     编码器初始化
//  @sample   Motor
//  MotorL(MOTOR_L,encoderL_QTIMER,encoderL_LSB_QTIMER,encoderL_DIR_QTIMER,
//                         0.5,0.2,0.2,MOTOR_MIN,MOTOR_MAX,MOTOR_MIN);
//-------------------------------------------------------------------------------------------------------------------
Encoder::Encoder(QTIMERN_enum QTIMER_Channel_temp,
                 QTIMER_PIN_enum QTIMER_PinChannel_LSB_temp,
                 QTIMER_PIN_enum QTIMER_PinChannel_DIR_temp)
    : Channel(QTIMER_Channel_temp), LSB(QTIMER_PinChannel_LSB_temp),
      DIR(QTIMER_PinChannel_DIR_temp) {}

void Encoder::Init() { qtimer_quad_init(Channel, LSB, DIR); }

//-------------------------------------------------------------------------------------------------------------------
//  @name     获取速度（单位cm/s）
//-------------------------------------------------------------------------------------------------------------------
void Encoder::CalSpeed() {
    //读取编码器计数值
    // int32 speed = qtimer_quad_get(Channel,
    //                             LSB); //这里需要注意第二个参数务必填写A相引脚
    // qtimer_quad_clear(Channel, LSB);
    // DEBUG_LOG("%f\n", encoder1);
    // TODO: dir??? A:I guess it will calculate automatically..because demo said
    // we could use this kind of encoder and that's how demo is written. Of
    // course, practice is the criterion of truth. Use the GUI to test.
    //    speed = PULSE_TO_CM_S(res);
    //	  speed=filter_speed.Moving(speed);
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
    auto templ(Car.EncoderL.speed), tempr(Car.EncoderR.speed);

    CRITICAL_REGION_ENTER();
    Car.EncoderL.speed = qtimer_quad_get(QTIMER_1, Car.EncoderL.LSB);
    Car.EncoderR.speed = -qtimer_quad_get(QTIMER_1, Car.EncoderR.LSB);
    qtimer_quad_clear(QTIMER_1, Car.EncoderL.LSB);
    qtimer_quad_clear(QTIMER_1, Car.EncoderR.LSB);
    CRITICAL_REGION_EXIT();

    Car.EncoderL.speed = PULSE_TO_CM_S(Car.EncoderL.speed);
    Car.EncoderR.speed = PULSE_TO_CM_S(Car.EncoderR.speed);

    if (templ - Car.EncoderL.speed > 50)
        Car.EncoderL.speed = templ - 9;
    if (tempr - Car.EncoderR.speed > 30)
        Car.EncoderR.speed = tempr - 9;
    // CAR_ERROR_CHECK(temp > Car.EncoderL.speed-60);
    if (IS_STEADY)
        Car.Machine.Steady();
    if (IS_STOPPED)
        Car.Machine.Halt();
}
