/**
 * @file PACK
 * @brief 与上位机通讯的数据包
 */
#ifndef _PACK_H
#define _PACK_H

#include "peripherals.h"
#include "route.h"
#include "util.h"

/**
 * @name 蓝牙数据包
 * @brief 蓝牙数据包相关结构体，以及长度、包头、包尾等参数
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
extern Dir_PD;

/** @struct Speed_PackData
 * @brief 速度上行包的结构体
 * 定义存储速度上行包的所有内容
 */
__PACKED_STRUCT Speed_PackData {
    uint8_t head, size, speedLeft_H, speedLeft_L, speedRight_H, speedRight_L,
        dutyLeft, dutyRight, parity, tail;
}
extern Speed_PD;

/** @struct AI_PackData
 * @brief AI上行包的结构体
 * 定义存储AI上行包的所有内容
 */
__PACKED_STRUCT AI_PackData {
    uint8_t head, size, steerDutyforAI, MagL_Row2, MagLM_Row2, MagM_Row2,
        MagRM_Row2, MagR_Row2, MagL_Row3, MagR_Row3, CRC0, CRC1, CRC2, CRC3,
        tail;
}
extern AI_PD;

/** @} 蓝牙数据包 */

void Unpack();
void pack_Init();
void RxPack_Handle(uint8_t *rxbuf, uint32_t count);
void Crc32Parity(uint8_t *buf, uint8_t cnt, uint8_t *parity);

/**
 * @brief 将buf中的所有字节累加，计算parity
 * @param[in]  buf        需要累加的字节数组的首地址
 * @param[in]  cnt        需要累加的字节数
 * @return  函数执行结果
 */
__STATIC_INLINE uint8_t AccParity(uint8_t *buf, uint8_t cnt) {
    uint8_t sum = 0;
    while (cnt--)
        sum += buf[cnt];
    return sum;
}


#endif
