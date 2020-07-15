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
    //    LPUART_SendEDMA(COM_PERIPHERAL, &COM_LPUART_eDMA_Handle, &xfer);
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
    if (LPUART_GetStatusFlags(COM_PERIPHERAL) &
        kLPUART_TxDataRegEmptyFlag)
        SendOne(pack);
    else
        sendQ.push(pack);
    CRITICAL_REGION_EXIT();
}

static void AnalysePackages() {


}

// extern "C" void com_callback(LPUART_Type *, lpuart_edma_handle_t *, status_t status,
//                              void *) {
//     volatile int i;
//     ++i;
//                                 i%=2;
//                                 if (i)
//                                     Car.CoreLED.SetBrightness(99);
//                                 else
//                                      Car.CoreLED.SetBrightness(1);
//     if (kStatus_LPUART_IdleLineDetected == status) {
//         Car.CoreLED.SetBrightness(50);
//     }
//     if (kStatus_LPUART_TxIdle == status)
//         SendPop();
// }
