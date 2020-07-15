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
//  @name     ��������ʼ��
//  @sample   Motor
//  MotorL(MOTOR_L,encoderL_QTIMER,encoderL_LSB_QTIMER,encoderL_DIR_QTIMER,
//                         0.5,0.2,0.2,MOTOR_MIN,MOTOR_MAX,MOTOR_MIN);
//-------------------------------------------------------------------------------------------------------------------
Encoder::Encoder(qtmr_channel_selection_t QTIMER_Channel)
    : Channel(QTIMER_Channel), filter_moving(3), distance(0) {}

//-------------------------------------------------------------------------------------------------------------------
//  @name     ��ȡ�ٶȣ���λcm/s��
//-------------------------------------------------------------------------------------------------------------------
__INLINE void Encoder::CalSpeed() {
    //��ȡ����������ֵ
    speed = PULSE_TO_CM_S(static_cast<int16_t>(TMR_CNTR));
    TMR_CNTR = 0;
//    speed = filter_moving.Moving(speed);
}

//-------------------------------------------------------------------------------------------------------------------
//  @name     ��ȡ���루��λcm��
//  @Sample   MotorL.GetSpeed();
//-------------------------------------------------------------------------------------------------------------------
inline void Encoder::CalDistance() {
    distance += speed * TICKS_TO_MS(PIT_ENCODER_CH_TICKS, PIT_CLK_FREQ) / 1000;
}

/** -------------------------------------------------------------------------------------------------------------------
 *  @name     �ٶ�pid�������ҵ��
 */
void pulse_encoder_schedule() {
    CRITICAL_REGION_ENTER();
    Car.EncoderL.CalSpeed();
    Car.EncoderR.CalSpeed();
	
//    filter_fir_speedL.filter_fir();
//    filter_fir_speedR.filter_fir();
    CRITICAL_REGION_EXIT();

    Car.EncoderL.speed = -Car.EncoderL.speed;
    Car.EncoderL.CalDistance();
    Car.EncoderR.CalDistance();
    if (IS_STEADY)
        Car.Machine.Steady();
    if (IS_STOPPED)
        Car.Machine.Stopped();
}
