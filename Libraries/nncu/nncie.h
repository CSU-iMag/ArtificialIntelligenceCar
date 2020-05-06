#pragma once
#if defined(__cplusplus)
extern "C" {
#endif

#include "aiann/aia_cmsisnn_ext.h"

#ifdef __CC_ARM
#pragma anon_unions
#endif

// if weight is too large that overwhelms DCache, configure this to a value larger than DCache size
// so your performance can gain, otherwise it just eat RAM but has hardly improvement.
#define CI_CFG_WEIT_CACHE 	0
// may consider to align to cache line, but seems has hardly effect.
// however, better to align to 4 or 8 byte. If you are sure your heap allocator always return aligned 
// addresses, then set this to 0.
#define CI_CFG_TENSOR_ALIGN		8
#define CI_CFG_TENSOR_FORMAT	0	// 0=[H][W][C], 1=[C][H][W]
// below "CFG" items should not be changed as they must match model data file's header structure length 
#define CI_MAX_ALLOC_BUF		16
#define CI_MAX_BUF_PER_CMD		8
/*
NNCIE is a very simple "inference engine" that interprets the model.
Currently only supports models burned in NVM, and model itself provides how to allocate activation buffers
*/

#define CI_ASSERT(x) do { \
	if (!(x)) while(1){} \
	}while(0)

#define CI_COMPLETE		32767L
#define CI_OK			0
#define CI_ERR_GNRC		-1L

typedef struct _CI_UserCtx {
	uint64_t dat64[(32 + 192 + CI_MAX_ALLOC_BUF * 16 + 7) / 8];
}CI_UserContext;
typedef struct _CI_UserModelHeader {
	uint64_t dat64[160 / 8];
}CI_UserModelHeader;

typedef struct _ModelInfo_t
{
	int quantBits;
	const char* pszString;
}ModelInfo_t;

typedef struct _CI_OutMetricInfo {
	int h, w, c, fracBits;
}CI_OutMetricInfo;

int CI_GetModelInfoXIP(const void* pvModel, ModelInfo_t* pInf);
typedef int (*pfnReadData_t)(uint32_t modelHandle, const CI_UserModelHeader* p, uint32_t ofs, size_t bytes, void* pvBuf);
int CI_RunModelXIP(const void *pvModel, const void *pcvIn, void *pvOutBuf);
int CI_RunModelXIP_NoCopyOutput(const void* pvModel, const void* pcvIn, void** ppvOutBuf);
int CI_RunModelXIP_MTSafe(const void *pvModel, const void *pcvIn, void *pvOutBuf);
const CI_OutMetricInfo* CI_GetOutMetricInfo(const CI_UserContext* pUserCtx);
const char* CI_GetModelStringXIP(const void* pvModel);

// modelHandle: A user-interpreted value to identify the model to run, usually useful for non-xip models to identify which model.
// pUserCtx: pointer to context of this CI engine. If zero, CI will try to use its default context, but only OK if the context is not inuse
// pfnReadData: pointer to a function that read data from non-xip models. If zero, will use the default reader in the port.
int CI_zzCreateContextNonXIP(uint32_t modelHandle, CI_UserContext* pUserCtx, void *pvOutBuf, pfnReadData_t pfnReadData);

// modelHandle: A user-interpreted value to identify the model to run, usually useful for non-xip models to identify which model.
// pCtx: pointer to context of this CI engine. If zero, CI will try to use its default context, but only OK if the context is not inuse
// pvModel: pointer to the model data, always starts with the model header structure
int CI_zzCreateContextXIP(uint32_t modelHandle, CI_UserContext*pUserCtx, void *pvOutBuf, const void *pvModel);

int CI_zzStep(CI_UserContext* pUserCtx);
int CI_zzRunWithContext(CI_UserContext*pCtx, const void *pcvIn);

// declare default names of model and test vectors in C array form.
extern const unsigned char test1[];
extern const unsigned char model1[];

// Below functions need to port. They all have default weak implementations
// However, for non-XIP model storage, usually you need to implement "CIPort_DefaultReadData"
extern void* CIPort_Malloc(size_t size);
extern void CIPort_Free(void *p);
extern int CIPort_DefaultReadData(uint32_t modelHandle, const CI_UserModelHeader *p, uint32_t ofs, size_t bytes, void *pvBuf);
extern int CIPort_EnterCritical(void);
extern int CIPort_LeaveCritical(void);

//extern int CIPort_Printf(const char *fmt_s, ...);
#define CIPort_Printf PRINTF

#if defined(__cplusplus)
}
#endif
