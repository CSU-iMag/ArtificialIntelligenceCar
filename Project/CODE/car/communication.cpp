#include "communication.hpp"
#include "SEEKFREE_VIRSCO.h"
#include "car.hpp"
#include "fsl_iomuxc.h"
#include "package.hpp"
#include "peripherals.h"
#include "util.h"
#include <algorithm>
#include <queue>
#include <vector>

static const uint8_t SOH = 0x01, EOT = 0x04, ESC = 0x5c;
static std::queue<std::vector<uint8_t>> sendQ;
SECTION_NONCACHE static uint8_t buf[66] = {0}; // max size within ESC
SECTION_NONCACHE static lpuart_transfer_t xfer = {.data = buf};

static void SendOne(std::vector<uint8_t> &pack) {
    xfer.dataSize = pack.size() + 2;
    memcpy(buf + 1, &pack[0], pack.size());
    buf[0] = SOH;
    buf[pack.size() + 1] = EOT;
    LPUART_SendEDMA(COM_PERIPHERAL, &COM_LPUART_eDMA_Handle, &xfer);
}

static void SendPop() {
    if (sendQ.empty())
        return;
    SendOne(sendQ.front());
    sendQ.pop();
}

/**
 * @brief 将buf中的所有字节累加，计算parity
 * @param[in]  buf        需要累加的字节数组的首地址
 * @param[in]  cnt        需要累加的字节数
 * @return  函数执行结果
 */
__STATIC_INLINE uint8_t AccParity(const uint8_t *buf, uint8_t cnt) {
    uint8_t sum = 0;
    while (cnt--)
        sum += buf[cnt];
    return sum;
}

__STATIC_INLINE bool IsConflict(const uint8_t &b) {
    return b == ESC || b == SOH || b == EOT;
}

void com_push(uint8_t packID, const uint8_t *payload, uint8_t length) {
    std::vector<uint8_t> pack{packID};
    for (int i(0); i < length; pack.push_back(payload[i++]))
        if (IsConflict(payload[i]))
            pack.push_back(ESC);

    uint8_t parity(AccParity(payload, length));
    if (IsConflict(parity))
        pack.push_back(ESC);
    pack.push_back(parity);

    CRITICAL_REGION_ENTER();
    if (LPUART_GetStatusFlags(COM_PERIPHERAL) &
        kLPUART_TxDataRegEmptyFlag)
        SendOne(pack);
    else
        sendQ.push(pack);
    CRITICAL_REGION_EXIT();
}

void com_callback(LPUART_Type *base, lpuart_edma_handle_t *handle,
                  status_t status, void *userData) {
    if (kStatus_LPUART_IdleLineDetected == status) {
    }
    if (kStatus_LPUART_TxIdle == status)
        SendPop();
    // DEBUG_LOG("uart call sta=%d\n", status);
}

void com_init() {
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TX, /* GPIO_B1_12 is configured
                                                      as LPUART5_TX */
                     0U); /* Software Input On Field: Input Path is determined
                             by functionality */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RX, /* GPIO_B1_13 is configured
                                                      as LPUART5_RX */
                     0U);
}

#ifdef DEPRECATED

/**
 * @brief 将buf中的所有字节异或，计算parity
 * @param[in]  buf        需要异或的字节数组的首地址
 * @param[in]  cnt        需要异或的字节数
 * @return  函数执行结果
 */
__STATIC_INLINE uint8_t XorParity(uint8_t *buf, uint8_t cnt) {
    uint8_t ret = 0;
    std::for_each(buf, buf + cnt, [&ret](uint8_t d) { ret ^= d; });
    return ret;
}

/**
 * @brief 将buf[0]~buf[cnt-1]进行crc32校验计算，得parity
 * @param[in]  buf        需要进行crc32校验的字节数组的首地址
 * @param[in]  cnt        需要进行crc32校验的字节数
 * @param[out]  parity    放置crc32校验结果的首地址
 */
void Crc32Parity(uint8_t *buf, uint8_t cnt, uint8_t *parity) {
    status_t status;
    size_t outLength = 4;

    /* Expected CRC-32 for the message.
     * CRC-32 params:
     * width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false
     * xorout=0x00000000 http://reveng.sourceforge.net/crc-catalogue/
     */
    memset(parity, 0, outLength);

    /************************ CRC-32 **************************/
    status = DCP_HASH(DCP, const_cast<dcp_handle_t *>(&DCP_handle_0),
                      kDCP_Crc32, buf, cnt, parity, &outLength);
    DEBUG_LOG("CRC-32 status=%d,output:%d,%d,%d,%d\n", status, parity[0],
              parity[1], parity[2], parity[3]);
    CAR_ERROR_CHECK(kStatus_Success == status);
}

#endif
