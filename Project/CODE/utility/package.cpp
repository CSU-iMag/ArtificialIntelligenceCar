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
    uint8_t head,
        size, //这是pack里data的size，不是整个pack的size
        steerDutyforAI, steerPID_Input, magL_Row1_fValue, magM_Row1_fValue,
        magR_Row1_fValue, parity, tail;
}
static Dir_PD;
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
    uint8_t head, size, steerDutyforAI, MagL_Row2, MagLM_Row2, MagM_Row2,
        MagRM_Row2, MagR_Row2, MagL_Row3, MagR_Row3, CRC0, CRC1, CRC2, CRC3,
        tail;
}
static AI_PD;
static const uint8_t AiID = 0x8c;

/** @struct PID_PackData
 * @brief PID下行包的结构体
 * 定义存储PID下行包的所有内容
 */
struct PID_PackData {
    uint8_t head, size;
    word_t KP, KI, KD;
    uint8_t parity, tail;
} __PACKED Speed_PID_PD, Dir_PID_PD;

/** @struct Switch_PackData
 * @brief 开关下行包的结构体
 * 定义存储开关下行包的所有内容
 */
struct Switch_PackData {
    uint8_t head, size, switch_status, parity, tail;
} Sw_PD;

/** @struct Filter_PackData
 * @brief 滤波器下行包的结构体
 * 定义存储滤波器下行包的所有内容
 */
struct Filter_PackData {
    uint8_t head, size, Encoder_Filter_size, Motor_Filter_size,
        Mag_MiddleFilter_size, Mag_MovFilter_size, parity, tail;
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
    Dir_PD.steerDutyforAI = Car.Steer3010.steerDutyforAI;
    //    Dir_PD.steerPID_Input = Car.Steer3010.steerPID_Input;
    Dir_PD.magL_Row1_fValue = Car.MagList[9]->GetNormalized();
    Dir_PD.magM_Row1_fValue = Car.MagList[10]->GetNormalized();
    Dir_PD.magR_Row1_fValue = Car.MagList[11]->GetNormalized();
}

void Send_AI_PackData_sched(sched_event_data_t dat) {
    AI_PD.steerDutyforAI = (uint8_t)Car.Steer3010.steerDutyforAI;
    AI_PD.MagL_Row2 = (uint8_t)Car.MagList[MagL_ROW2]->GetNormalized();
    AI_PD.MagLM_Row2 = (uint8_t)Car.MagList[MagLM_ROW2]->GetNormalized();
    AI_PD.MagM_Row2 = (uint8_t)Car.MagList[MagM_ROW2]->GetNormalized();
    AI_PD.MagRM_Row2 = (uint8_t)Car.MagList[MagRM_ROW2]->GetNormalized();
    AI_PD.MagR_Row2 = (uint8_t)Car.MagList[MagR_ROW2]->GetNormalized();
    AI_PD.MagL_Row3 = (uint8_t)Car.MagList[MagL_ROW3]->GetNormalized();
    AI_PD.MagR_Row3 = (uint8_t)Car.MagList[MagR_ROW3]->GetNormalized();

#ifdef DEPRECATED
    // Crc32Parity((uint8_t *)&AI_PD + 2, AI_PD.size, (uint8_t *)&AI_PD.CRC0);
    lpuart_transfer_t xfer = {.data = (uint8_t *)&AI_PD,
                              .dataSize = (size_t)AI_PD.size + 4};
    LPUART_SendEDMA(COMMUNICATE_PERIPHERAL, &communicate_LPUART_eDMA_Handle,
                    &xfer);
#endif
}

/**
 * @brief 上行包的定时发送softtimer对象的声明
 */
SoftTimer SendDirectionTim(Send_Direction_PackData_sched);
SoftTimer SendSpeedTim(SendSpeed);
SoftTimer SendAiTim(Send_AI_PackData_sched);

#ifdef DEPRECATED
/**
 * @brief 接收下行包时在下行包处理函数中调用的函数
 */
inline void Receive_Speed_PID_PackData(uint8_t *buf) {
    DEBUG_LOG("speed package\n");
    Speed_PID_PD.KP.Bdata[0] = buf[0];
    Speed_PID_PD.KP.Bdata[1] = buf[1];
    Speed_PID_PD.KP.Bdata[2] = buf[2];
    Speed_PID_PD.KP.Bdata[3] = buf[3];
    Speed_PID_PD.KI.Bdata[0] = buf[4];
    Speed_PID_PD.KI.Bdata[1] = buf[5];
    Speed_PID_PD.KI.Bdata[2] = buf[6];
    Speed_PID_PD.KI.Bdata[3] = buf[7];
    Speed_PID_PD.KD.Bdata[0] = buf[8];
    Speed_PID_PD.KD.Bdata[1] = buf[9];
    Speed_PID_PD.KD.Bdata[2] = buf[10];
    Speed_PID_PD.KD.Bdata[3] = buf[11];
    Car.MotorL.speedCtrl.SetPID(Speed_PID_PD.KP.Fdata, Speed_PID_PD.KI.Fdata,
                                Speed_PID_PD.KD.Fdata);
    Car.MotorR.speedCtrl.SetPID(Speed_PID_PD.KP.Fdata, Speed_PID_PD.KI.Fdata,
                                Speed_PID_PD.KD.Fdata);
}

inline void Receive_Dir_PID_PackData(uint8_t *buf) {
    DEBUG_LOG("steering package\n");
    Dir_PID_PD.KP.Bdata[0] = buf[0];
    Dir_PID_PD.KP.Bdata[1] = buf[1];
    Dir_PID_PD.KP.Bdata[2] = buf[2];
    Dir_PID_PD.KP.Bdata[3] = buf[3];
    Dir_PID_PD.KI.Bdata[0] = buf[4];
    Dir_PID_PD.KI.Bdata[1] = buf[5];
    Dir_PID_PD.KI.Bdata[2] = buf[6];
    Dir_PID_PD.KI.Bdata[3] = buf[7];
    Dir_PID_PD.KD.Bdata[0] = buf[8];
    Dir_PID_PD.KD.Bdata[1] = buf[9];
    Dir_PID_PD.KD.Bdata[2] = buf[10];
    Dir_PID_PD.KD.Bdata[3] = buf[11];
    Car.Steer3010.steerCtrl.SetPID(Dir_PID_PD.KP.Fdata, Dir_PID_PD.KI.Fdata,
                                   Dir_PID_PD.KD.Fdata);
}

inline void Receive_Switch_PackData(uint8_t *buf) {
    Sw_PD.switch_status = buf[0];
}

inline void Receive_Filter_PackData(uint8_t *buf) {
    Fil_PD.Encoder_Filter_size = buf[0];
    Fil_PD.Motor_Filter_size = buf[1];
    Fil_PD.Mag_MiddleFilter_size = buf[2];
    Fil_PD.Mag_MovFilter_size = buf[3];
}

/**
 * @brief 接收下行包时在下行包处理函数中调用的函数的函数指针数组
 */
void (*RxPack_FuncPtr[4])(uint8_t *buf) = {
    Receive_Speed_PID_PackData, Receive_Dir_PID_PackData,
    Receive_Switch_PackData, Receive_Filter_PackData};

//我觉得先把rxbuffer放到一个新数组rxbuf，然后再将这个新数组送到这个函数里处理，
//从新数组里接收多个包，依次赋给buf，每种包对应一个处理函数，
//然后判断若有剩下的未完全接收的包，先保留函数里一些状态，将最后的不完全的buf放到
// rxbuf首端，然后新的rxbuffer应该赋进来排在后面。好麻烦，但是应该不大可能，毕竟空闲中断
#endif
