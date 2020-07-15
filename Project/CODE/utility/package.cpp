#include "package.hpp"
#include "car.hpp"
#include "communication.hpp"
#include "peripherals.h"
#include <algorithm>

/**
 * @name 数据包
 * @brief 数据包相关结构体，以及长度、包头、包尾等参数
 * @{
 */

/** @struct Direction_PackData
 * @brief 方向上行包的结构体
 * 定义存储方向上行包的所有内容
 */
__PACKED_STRUCT Direction_PackData {
    uint8_t steerDutyforAI, steerPID_Input, magL_Row1_fValue, magM_Row1_fValue,
        magR_Row1_fValue;
}
static pkgDir;
static const uint8_t DirectionID = 0x8a;

/** @struct Speed_PackData
 * @brief 速度上行包的结构体
 * 定义存储速度上行包的所有内容
 */
__PACKED_STRUCT Speed_PackData {
    uint8_t speedLeft_H, speedLeft_L, speedRight_H, speedRight_L, dutyLeft,
        dutyRight;
}
static pkgSpeed;
static const uint8_t SpeedID = 0x8b;

/** @struct AI_PackData
 * @brief AI上行包的结构体
 * 定义存储AI上行包的所有内容
 */
__PACKED_STRUCT AI_PackData {
    uint8_t steerDutyforAI, MagL_Row2, MagLM_Row2, MagM_Row2, MagRM_Row2,
        MagR_Row2, MagL_Row3, MagR_Row3;
}
static pkgAI;
static const uint8_t AiID = 0x8c;

/** @struct PID_PackData
 * @brief PID下行包的结构体
 * 定义存储PID下行包的所有内容
 */
__PACKED_STRUCT PID_PackData { word_t KP, KI, KD; }
static Speed_PID_PD, Dir_PID_PD;

/** @struct Switch_PackData
 * @brief 开关下行包的结构体
 * 定义存储开关下行包的所有内容
 */
__PACKED_STRUCT Switch_PackData { uint8_t switch_status; }
static Sw_PD;

/** @struct Filter_PackData
 * @brief 滤波器下行包的结构体
 * 定义存储滤波器下行包的所有内容
 */
struct Filter_PackData {
    uint8_t Encoder_Filter_size, Motor_Filter_size, Mag_MiddleFilter_size,
        Mag_MovFilter_size;
} Fil_PD;

/** @} 数据包 */

void SendSpeed(sched_event_data_t dat) {
    pkgSpeed.speedLeft_H = ((int16_t)Car.EncoderL.GetSpeed() >> 8) & (0x0FF);
    pkgSpeed.speedLeft_L = ((int16_t)Car.EncoderL.GetSpeed()) & (0x0FF);
    pkgSpeed.speedRight_H = ((int16_t)Car.EncoderR.GetSpeed() >> 8) & (0x0FF);
    pkgSpeed.speedRight_L = ((int16_t)Car.EncoderR.GetSpeed()) & (0x0FF);
    pkgSpeed.dutyLeft = (uint8_t)Car.MotorL.duty;
    pkgSpeed.dutyRight = (uint8_t)Car.MotorR.duty;
    com_push(SpeedID, (const uint8_t *)&pkgSpeed, sizeof pkgSpeed);
}

void SendSteer(sched_event_data_t dat) {}

/**
 * @brief 发送上行包
 */
void Send_Direction_PackData_sched(sched_event_data_t dat) {
    pkgDir.steerDutyforAI = Car.Steer3010.steerDutyforAI;
    //    pkgDir.steerPID_Input = Car.Steer3010.steerPID_Input;
    pkgDir.magL_Row1_fValue = Car.MagList[9].GetNormalized();
    pkgDir.magM_Row1_fValue = Car.MagList[10].GetNormalized();
    pkgDir.magR_Row1_fValue = Car.MagList[11].GetNormalized();
}

void Send_AI_PackData_sched(sched_event_data_t dat) {
    pkgAI.steerDutyforAI = (uint8_t)Car.Steer3010.steerDutyforAI;
    // pkgAI.MagL_Row2 = (uint8_t)Car.MagList[MagL_ROW2].GetNormalized();
    // pkgAI.MagLM_Row2 = (uint8_t)Car.MagList[MagLM_ROW2].GetNormalized();
    // pkgAI.MagM_Row2 = (uint8_t)Car.MagList[MagM_ROW2].GetNormalized();
    // pkgAI.MagRM_Row2 = (uint8_t)Car.MagList[MagRM_ROW2].GetNormalized();
    // pkgAI.MagR_Row2 = (uint8_t)Car.MagList[MagR_ROW2].GetNormalized();
    // pkgAI.MagL_Row3 = (uint8_t)Car.MagList[MagL_ROW3].GetNormalized();
    // pkgAI.MagR_Row3 = (uint8_t)Car.MagList[MagR_ROW3].GetNormalized();
}

/**
 * @brief 上行包的定时发送softtimer对象的声明
 */
SoftTimer SendDirectionTim(Send_Direction_PackData_sched);
SoftTimer SendSpeedTim(SendSpeed);
SoftTimer SendAiTim(Send_AI_PackData_sched);
