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

static std::queue<std::vector<uint8_t>> sendQ;
SECTION_NONCACHE static uint8_t buf[66] = {0}; // max size within ESC
SECTION_NONCACHE static lpuart_transfer_t xfer = {.data = buf};

static void SendOne(std::vector<uint8_t> &pack) {
    xfer.dataSize = pack.size() + 2;
    memcpy(buf + 1, &pack[0], pack.size());
    buf[0] = SOH;
    buf[pack.size() + 1] = EOT;
    LPUART_TransferSendNonBlocking(COM_PERIPHERAL, &COM_handle, &xfer);
}

static void SendPop() {
    if (sendQ.empty())
        return;
    SendOne(sendQ.front());
    sendQ.pop();
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
    if (LPUART_GetStatusFlags(COM_PERIPHERAL) & kLPUART_TxDataRegEmptyFlag)
        SendOne(pack);
    else
        sendQ.push(pack);
    CRITICAL_REGION_EXIT();
}

__STATIC_INLINE bool CheckParity(int i) {
    return AccParity(COM_rxBuffer + 2, i - 2) == COM_rxBuffer[i];
}

static void AnalysePackages(sched_event_data_t dat) {
    if (COM_rxBuffer[0] != SOH)
        return;

    bool esc = false;
    std::vector<uint8_t> buf;
    for (int i(1); i < COM_RX_BUFFER_SIZE; ++i) {
        if (!esc && COM_rxBuffer[i] == EOT &&
            CheckParity(i - 1)) { // 检测到帧尾
            buf.erase(buf.cend() - 1);
            package_receive(buf);
            return;
        } else if (!esc && COM_rxBuffer[i] == ESC)
            esc = true; // 转义下一个字节
        else {
            esc = false;
            buf.push_back(COM_rxBuffer[i]);
        }
    }
}

#define START_RX                                                               \
    LPUART_TransferReceiveNonBlocking(COM_PERIPHERAL, &COM_handle,             \
                                      (lpuart_transfer_t *)&COM_rxTransfer,    \
                                      NULL)

void com_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status,
                  void *userData) {
    if (kStatus_LPUART_TxIdle == status)
        SendPop();
    if (kStatus_LPUART_IdleLineDetected == status) {
        sched_event_put(AnalysePackages);
        LPUART_TransferAbortReceive(COM_PERIPHERAL, &COM_handle);
        START_RX;
    }
}

void com_init() { START_RX; }
