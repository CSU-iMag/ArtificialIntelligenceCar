/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Interface.c											**/
/** Author: Polarix														**/
/** Description: System platform interface.								**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Interface.h"

// SDK string encode convert library interface
#ifdef _SIMPLE_GUI_ENCODE_TEXT_
 #ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
  #include <iconv.h>
 #endif
#endif

// System RTC interface switch.
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
#include <windows.h>
#include <time.h>
#else
	// Include platform RTC interface declare here.
//#include "RTC.h"
#endif

// Dynamic memory operation switch.
#ifdef _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
 #ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
  #include <malloc.h>
  #include <string.h>
 #endif
#else
	// Include MMU interface header file here.
#include <string.h>
#endif

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
 #ifdef _SIMPLE_GUI_ENCODE_TEXT_
  static SGUI_CHAR	g_arrcEncodeBuffer[SGUI_ENCODE_BUFFER_SIZE];
 #endif
#endif

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
#ifdef _SIMPLE_GUI_ENCODE_TEXT_
/*************************************************************************/
/** Function Name:	SGUI_SystemIF_EncodeConvert							**/
/** Purpose:		Convert string encode.								**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ szSourceEncode[in]: Source encoder name.							**/
/**	@ szDestinationEncode[in]: Destination encoder name.				**/
/**	@ szSource[in]:		String will converted.							**/
/** Return:			String after convert.								**/
/*************************************************************************/
SGUI_SZSTR SGUI_SystemIF_EncodeConvert(SGUI_CSZSTR szSourceEncode, SGUI_SZSTR szDestinationEncode, SGUI_SZSTR szSource)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SZSTR					pszResultPtr;
	SGUI_SIZE					uiSourceLength, uiOutputBufferSize;
	SGUI_SIZE					uiEncoderResult;
	iconv_t						pIconv;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiEncoderResult			= -1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	pIconv = iconv_open(szDestinationEncode, szSourceEncode);
	if((iconv_t)-1 != pIconv)
	{
		uiSourceLength = SGUI_SystemIF_StringLength(szSource) + 1;
		uiOutputBufferSize = SGUI_ENCODE_BUFFER_SIZE;
		pszResultPtr = g_arrcEncodeBuffer;
		uiEncoderResult = iconv(pIconv, &szSource, &uiSourceLength, &pszResultPtr, &uiOutputBufferSize);
		if (uiEncoderResult == -1)
		{
			pszResultPtr = NULL;
		}
		else
		{
			pszResultPtr = g_arrcEncodeBuffer;
		}
	}
	iconv_close(pIconv);
	return pszResultPtr;
}
#endif

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_StringLengthCopy						**/
/** Purpose:		Copy string.										**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ szDest[in]:		Source string.									**/
/**	@ szSrc[in]:		Destination string.								**/
/**	@ uiSize[in]:		String length will be copied.					**/
/** Return:			Destination string pointer.							**/
/*************************************************************************/
SGUI_SZSTR SGUI_SystemIF_StringLengthCopy(SGUI_SZSTR szDest, SGUI_CSZSTR szSrc, SGUI_SIZE uiSize)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
#ifndef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	const SGUI_CHAR*			pcSrcPtr;
	SGUI_CHAR*					pcDestPtr;
	SGUI_SIZE					sCopiedCount;
#endif

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
#ifndef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	pcSrcPtr =					szSrc;
	pcDestPtr =					szDest;
	sCopiedCount =				0;
#endif

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != szDest) && (NULL != szSrc))
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		strncpy(szDest, szSrc, uiSize);
#else
		// Add string(character array) copy with length process here;
		while(('\0' != *pcSrcPtr) && (sCopiedCount < uiSize))
		{
			*pcDestPtr++ = *pcSrcPtr++;
			sCopiedCount++;
		}
#endif
	}

	return szDest;
}

#ifdef _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
/*************************************************************************/
/** Function Name:	SGUI_SystemIF_Allocate								**/
/** Purpose:		Allocate a memory block.							**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ uiSize[in]:		Allocated memory size.							**/
/** Return:			Allocated memory block started address, same as STD	**/
/**					malloc interface.									**/
/*************************************************************************/
void* SGUI_SystemIF_Allocate(SGUI_SIZE uiSize)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	void						*pAllocatedMemory;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pAllocatedMemory =		NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	pAllocatedMemory = malloc(uiSize);
#else
	// Add allocate memory function here;
	pAllocatedMemory = malloc(uiSize);
#endif
	return pAllocatedMemory;
}

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_Free									**/
/** Purpose:		Free a memory block.								**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ pFreePointer[in]:	Free memory pointer 							**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_SystemIF_Free(void* pFreePointer)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pFreePointer)
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		free(pFreePointer);
#else
		// Add allocate memory function here;
		free(pFreePointer);
#endif
	}
}
#endif

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_MemoryCopy							**/
/** Purpose:		Copy memory block to a new address.					**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ pDest[in]:		Memory address will copied to.					**/
/**	@ pSrc[in]:			Memory data source.								**/
/**	@ sSize[in]:		Copied data size(in byte).						**/
/** Return:			Destination memory block pointer.					**/
/*************************************************************************/
SGUI_PTR SGUI_SystemIF_MemoryCopy(SGUI_PTR pDest, const SGUI_PTR pSrc, SGUI_SIZE sSize)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
#ifndef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	SGUI_SIZE                   sIdx;
	SGUI_BYTE*					pSrcPtr;
#endif
	SGUI_BYTE*					pDestPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pDestPtr =					NULL;
#ifndef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	pSrcPtr =					(SGUI_BYTE*)pSrc;
	pDestPtr =					(SGUI_BYTE*)pDest;
#endif

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pDest) && (NULL != pSrc))
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		// Add memory block copy process here;
		pDestPtr = memcpy(pDest, pSrc, sSize);
#else
		for(sIdx=0; sIdx<sSize; sIdx++)
		{
			*pDestPtr++ = *pSrcPtr++;
		}
#endif
	}

	return pDestPtr;
}

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_MemorySet								**/
/** Purpose:		Set memory area data.           					**/
/** Params:																**/
/**	@ pMemoryPtr[in]:	Memory address will filled. 					**/
/**	@ iSetValue[in]:	Memory data byte value.							**/
/**	@ sSize[in]:		Memory area size.  				        		**/
/** Return:			None.                           					**/
/*************************************************************************/
void SGUI_SystemIF_MemorySet(SGUI_PTR pMemoryPtr, SGUI_BYTE iSetValue, SGUI_SIZE sSize)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
#ifndef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
	SGUI_SIZE                   sIdx;
	SGUI_BYTE*					pcbBytePtr;
#endif

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pMemoryPtr) && (0 != sSize))
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		// Add memory area value set process here.
        memset(pMemoryPtr, iSetValue, sSize);
#else
		pcbBytePtr = (SGUI_BYTE*)pMemoryPtr;
        for(sIdx=0; sIdx<sSize; sIdx++)
        {
            *pcbBytePtr++ = iSetValue;
        }
#endif
	}
}

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_StringLength							**/
/** Purpose:		Get string length in bytes.							**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ szString[in]:		String head pointer.							**/
/** Return:			String length in bytes.								**/
/*************************************************************************/
SGUI_SIZE SGUI_SystemIF_StringLength(SGUI_CSZSTR szString)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SIZE					uiStringLength;
	const SGUI_CHAR*			pcCharPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiStringLength =			0;
	pcCharPtr =					szString;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pcCharPtr)
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		// Add string(character array) length process here;
		uiStringLength = strlen(pcCharPtr);
#else
		while('\0' != *pcCharPtr)
		{
			uiStringLength++;
			pcCharPtr++;
		}
#endif
	}

	return uiStringLength;
}

/*************************************************************************/
/** Function Name:	SGUI_SystemIF_StringCopy							**/
/** Purpose:		Copy string.										**/
/** Resources:		None.												**/
/** Params:																**/
/**	@ szDest[in]:		Source string.									**/
/**	@ szSrc[in]:		Destination string	.							**/
/** Return:			Destination string pointer.							**/
/*************************************************************************/
SGUI_SZSTR SGUI_SystemIF_StringCopy(SGUI_SZSTR szDest, SGUI_CSZSTR szSrc)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	const SGUI_CHAR*			pcSrcPtr;
	SGUI_CHAR*					pcDestPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pcSrcPtr =					szSrc;
	pcDestPtr =					szDest;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pcDestPtr) && (NULL != pcSrcPtr))
	{
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
		// Add string(character array) time process here;
		strcpy(pcDestPtr, pcSrcPtr);
#else
		while('\0' != *pcSrcPtr)
		{
			*pcDestPtr++ = *pcSrcPtr++;
		}
		*pcDestPtr = '\0';
#endif
	}

	return pcDestPtr;
}
