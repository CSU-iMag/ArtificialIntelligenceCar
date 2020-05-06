/**
 * @file PACK
 * @brief 与上位机通讯的数据包
 */
#ifndef _PACK_H
#define _PACK_H

#include "fsl_dcp.h"
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

/**
 * @brief 将buf[0]~buf[cnt-1]进行crc32校验计算，得parity
 * @param[in]  buf        需要进行crc32校验的字节数组的首地址
 * @param[in]  cnt        需要进行crc32校验的字节数
 * @param[out]  parity    放置crc32校验结果的首地址
 */
__STATIC_INLINE void Crc32Parity(uint8_t *buf, uint8_t cnt, uint8_t *parity) {
    status_t status;
    size_t outLength = 4;

    /* Expected CRC-32 for the message.
     * CRC-32 params:
     * width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false
     * xorout=0x00000000 http://reveng.sourceforge.net/crc-catalogue/
     */
    static const unsigned char crc32[] = {0x7f, 0x04, 0x6a, 0xdd};

    dcp_handle_t m_handle;

    m_handle.channel = kDCP_Channel0;
    m_handle.keySlot = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    memset(&parity, 0, outLength);

    /************************ CRC-32 **************************/
    status = DCP_HASH(DCP, &m_handle, kDCP_Crc32, buf, cnt, parity, &outLength);
    //    TEST_ASSERT(kStatus_Success == status);
    //    TEST_ASSERT(outLength == 4u);
    //    TEST_ASSERT(memcmp(output, crc32, outLength) == 0);

    //    DEBUG_LOG("CRC-32 output:%d,%d,%d,%d\n",
    //    parity[0],parity[1],parity[2],parity[3]);
}

#endif
