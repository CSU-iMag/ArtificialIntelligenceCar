#include "nncie.h"
#include <stdlib.h>

__WEAK void *CIPort_Malloc(size_t size) {
    void *pv = malloc(size);
    if (pv)
        return pv;
    abort();
}

__WEAK void CIPort_Free(void *p) { free(p); }

__WEAK int CIPort_DefaultReadData(uint32_t modelHandle,
                                  const CI_UserModelHeader *p, uint32_t ofs,
                                  size_t bytes, void *pvBuf) {
    return 0;
}

volatile int s_criticalNest, s_primask;
__WEAK int CIPort_EnterCritical(void) {
    int nest;
    int primask = __get_PRIMASK();
    __set_PRIMASK(1);
    nest = s_criticalNest++;
    if (nest == 0) {
        s_primask = primask;
    }
    return nest;
}

__WEAK int CIPort_LeaveCritical(void) {
    if (s_criticalNest == 0)
        return -1L;
    int nest = --s_criticalNest;
    if (0 == nest) {
        __set_PRIMASK(s_primask);
    }
    return nest;
}
