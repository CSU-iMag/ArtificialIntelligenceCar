#include "package.hpp"
#include "car.hpp"
#include "communication.hpp"
#include "gui.hpp"
#include "peripherals.h"
#include "timer.hpp"

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

static pid_coefficient_t Speed_PID_PD, Steer_PID_PD;
static constexpr uint8_t SpeedPID_ID = 0x2a, SteerPID_ID = 0x2b;

/** @struct Switch_PackData
 * @brief 开关下行包的结构体
 * 定义存储开关下行包的所有内容
 */
__PACKED_STRUCT Switch_PackData { uint8_t switch_status; }
static Sw_PD;
static constexpr uint8_t Switch_ID = 0x2c;

/** @struct Filter_PackData
 * @brief 滤波器下行包的结构体
 * 定义存储滤波器下行包的所有内容
 */
struct Filter_PackData {
    uint8_t Encoder_Filter_size, Motor_Filter_size, Mag_MiddleFilter_size,
        Mag_MovFilter_size;
} Fil_PD;

/** @} 数据包 */

__STATIC_INLINE void ReceiveSpeedPID(uint8_t *buf) {
    CRITICAL_REGION_ENTER();
    memcpy(&Speed_PID_PD, buf, sizeof Speed_PID_PD);
    Car.MotorL.speedCtrl = Speed_PID_PD;
    CRITICAL_REGION_EXIT();
}

__STATIC_INLINE void ReceiveSteerPID(uint8_t *buf) {
    CRITICAL_REGION_ENTER();
    memcpy(&Steer_PID_PD, buf, sizeof Steer_PID_PD);
    Car.Steer3010.steerCtrl = Steer_PID_PD;
    CRITICAL_REGION_EXIT();
}

void package_receive(std::vector<uint8_t> buffer) {
    gui_debug.UpdateValue(0, "id: " + std::to_string(buffer[0]));
    switch (buffer[0]) {
    case SpeedPID_ID:
        if (buffer.size() - 1 == sizeof Speed_PID_PD)
            ReceiveSpeedPID(&buffer[1]);
        break;

    case SteerPID_ID:
        if (buffer.size() - 1 == sizeof Steer_PID_PD)
            ReceiveSteerPID(&buffer[1]);
        break;
    case Switch_ID:
        CRITICAL_REGION_ENTER();
        Car.Enabled = buffer[1];
        CRITICAL_REGION_EXIT();
        break;
    default:
        break;
    }
}

/**
 * @brief 发送上行包
 */

static void SendSpeed(sched_event_data_t dat) {
    if (!IS_SET(Car.Enabled, SW_SEND_MOTOR))
        return;
    pkgSpeed.speedLeft_H = ((int16_t)Car.EncoderL.GetSpeed() >> 8) & (0x0FF);
    pkgSpeed.speedLeft_L = ((int16_t)Car.EncoderL.GetSpeed()) & (0x0FF);
    pkgSpeed.speedRight_H = ((int16_t)Car.EncoderR.GetSpeed() >> 8) & (0x0FF);
    pkgSpeed.speedRight_L = ((int16_t)Car.EncoderR.GetSpeed()) & (0x0FF);
    pkgSpeed.dutyLeft = (uint8_t)Car.MotorL.duty;
    pkgSpeed.dutyRight = (uint8_t)Car.MotorR.duty;
    com_push(SpeedID, (const uint8_t *)&pkgSpeed, sizeof pkgSpeed);
}

static void SendSteer(sched_event_data_t dat) {
    if (!IS_SET(Car.Enabled, SW_SEND_DIR))
        return;

    pkgDir.magL_Row1_fValue = Car.MagList[MagLeftL].GetNormalized();
    pkgDir.magM_Row1_fValue = Car.MagList[MagMiddleM].GetNormalized();
    pkgDir.magR_Row1_fValue = Car.MagList[MagRightR].GetNormalized();
}

static void SendSwitch(sched_event_data_t dat) {
    uint8_t buf[1] = {Car.Enabled};
    // CRITICAL_REGION_ENTER();
    com_push(0x8d, buf, 1);
    // CRITICAL_REGION_EXIT();
}

/**
 * @brief 上行包的定时发送softtimer对象的声明
 */
SoftTimer SendDirectionTim(SendSteer);
SoftTimer SendSwitchTim(SendSwitch);
SoftTimer SendSpeedTim(SendSpeed);

void package_init() {
    SendDirectionTim.Start(DIR_PD_PERIOD);
    SendSpeedTim.Start(SPEED_PD_PERIOD);
    sched_event_put(SendSwitch);
    // SendAiTim.Start(AI_PD_PERIOD);
    // SendSwitchTim.Start(300);
}

void package_sendDeep(uint8_t steering) {
    if (!IS_SET(Car.Enabled, SW_SEND_AI))
        return;
    pkgAI.steerDutyforAI = steering;
    pkgAI.MagL_Row2 = (uint8_t)Car.MagList[MagLeftL].GetNormalized();
    pkgAI.MagLM_Row2 = (uint8_t)Car.MagList[MagLeftX].GetNormalized();
    pkgAI.MagM_Row2 = (uint8_t)Car.MagList[MagMiddleM].GetNormalized();
    pkgAI.MagRM_Row2 = (uint8_t)Car.MagList[MagRightX].GetNormalized();
    pkgAI.MagR_Row2 = (uint8_t)Car.MagList[MagRightR].GetNormalized();
    pkgAI.MagL_Row3 = (uint8_t)Car.MagList[MagMiddleL].GetNormalized();
    pkgAI.MagR_Row3 = (uint8_t)Car.MagList[MagMiddleR].GetNormalized();
    com_push(AiID, (const uint8_t *)&pkgAI, sizeof pkgAI);
}
