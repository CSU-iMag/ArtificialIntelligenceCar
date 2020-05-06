#include "SGUI_Typedef.h"
#include "fsl_cache.h"
#include "zf_flash.h"

//=======================================================================//
//= Static function declaration.
//=//
//=======================================================================//
static SGUI_INT GetCharIndex_GB2312(SGUI_UINT32 uiCode);
static SGUI_CSZSTR StepNext_GB2312(SGUI_CSZSTR cszSrc, SGUI_UINT32 *puiCode);
static SGUI_SIZE GB2312_GetFontData(SGUI_SIZE sStartAddr,
                                    SGUI_BYTE *pDataBuffer,
                                    SGUI_SIZE sReadSize);
static SGUI_BOOL GB2312_IsFullWidth(SGUI_UINT32 uiCode);

const SGUI_FONT_RES GB2312_FZXS12 = {
    /*SGUI_INT                      iHalfWidth*/ 6,
    /*SGUI_INT                      iFullWidth*/ 12,
    /*SGUI_INT                      iHeight*/ 12,
    /*SGUI_FN_IF_GET_CHAR_INDEX     fnGetIndex*/ GetCharIndex_GB2312,
    /*SGUI_FN_IF_GET_DATA           fnGetData*/ GB2312_GetFontData,
    /*SGUI_FN_IF_STEP_NEXT          fnStepNext*/ StepNext_GB2312,
    /*SGUI_FN_IF_IS_FULL_WIDTH      fnIsFullWidth*/ GB2312_IsFullWidth};

volatile static SGUI_CBYTE *GB2312_H12 = (volatile SGUI_CBYTE *)(uint32)(
    FLASH_BASE_ADDR + (FLASH_SECTOR_NUM - 45) * FLASH_SECTOR_SIZE);

//=======================================================================//
//= Function define.
//=//
//=======================================================================//
/*************************************************************************/
/** Function Name:	GetCharIndex_GB2312
 * **/
/** Purpose:		Get character index in font library.
 * **/
/** Resources:		None.
 * **/
/** Params:
 * **/
/**	@ uiCode[in]:	Character code.
 * **/
/** Return:			Character index.
 * **/
/*************************************************************************/
static SGUI_INT GetCharIndex_GB2312(SGUI_UINT32 uiCode) {
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    SGUI_INT iIndex;
    SGUI_UINT8 uiLowByte, uiHighByte;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    // Initialize variable.
    iIndex = SGUI_INVALID_INDEX;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    /* for ASCII characters. */
    if ((uiCode > 0x19) && (uiCode < 0x7F)) {
        iIndex = uiCode - (0x20);
    } else {
        uiLowByte = (SGUI_UINT8)(uiCode & 0xFF);
        uiHighByte = (SGUI_UINT8)((uiCode >> 8) & 0xFF);
        /* For symbol characters. */
        if ((uiCode > 0xA1A0) && (uiCode < 0xA9FF)) {
            iIndex = (((uiHighByte - 0xA1) * 94 + (uiLowByte - 0xA1)) * 2) + 95;
        } else if ((uiCode > 0xB0A0) && (uiCode < 0xF7FF)) {
            iIndex =
                (((uiHighByte - 0xB0) * 94 + (uiLowByte - 0xA1)) * 2) + 1787;
        }
    }
    return iIndex;
}

/*************************************************************************/
/** Function Name:	StepNext_GB2312
 * **/
/** Purpose:		Read current character code order by input pointer  **/
/**                 and step to next character start pointer.
 * **/
/** Resources:		None.
 * **/
/** Params:
 * **/
/**	@ cszSrc[in]:	Current char pointer.
 * **/
/**	@ puiCode[in]:	Character code.
 * **/
/** Return:			Next character start pointer.
 * **/
/*************************************************************************/
static SGUI_CSZSTR StepNext_GB2312(SGUI_CSZSTR cszSrc, SGUI_UINT32 *puiCode) {
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    const SGUI_CHAR *pcNextChar;
    SGUI_UINT32 uiCode;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pcNextChar = cszSrc;
    uiCode = 0;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if (NULL != pcNextChar) {
        do {
            uiCode = (SGUI_BYTE)(*pcNextChar++);
            if (uiCode < 0x7F) {
                break;
            }
            uiCode = uiCode << 8;
            uiCode |= (SGUI_BYTE)(*pcNextChar++);
        } while (0);
    }
    *puiCode = uiCode;

    return pcNextChar;
}

/*************************************************************************/
/** Function Name:	GB2312_GetFontData
 * **/
/** Purpose:		Read character data form font data. **/
/** Resources:		None.
 * **/
/** Params:
 * **/
/**	@ sStartAddr[in]: Read start address in memory.
 * **/
/**	@ pDataBuffer[in]: Character data dump buffer pointer.
 * **/
/**	@ sReadSize[in]: Size of data will be read, always mean the buffer  **/
/**                 size. **/
/** Return:			Data in process was be read.
 * **/
/*************************************************************************/
static SGUI_SIZE GB2312_GetFontData(SGUI_SIZE sStartAddr,
                                    SGUI_BYTE *pDataBuffer,
                                    SGUI_SIZE sReadSize) {
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    SGUI_SIZE sReadCount;
    volatile const SGUI_BYTE *pSrc = GB2312_H12 + sStartAddr;
    SGUI_BYTE *pDest = pDataBuffer;

    /*----------------------------------*/
    /* Initialize						*/
    /*----------------------------------*/
    pSrc = GB2312_H12 + sStartAddr;
    pDest = pDataBuffer;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if (NULL != pDataBuffer) {
        for (sReadCount = 0; sReadCount < sReadSize; sReadCount++) {
            *pDest++ = *pSrc++;
        }
    }
    return sReadCount;
}

/*************************************************************************/
/** Function Name:	GB2312_GetFontData
 * **/
/** Purpose:		Read character data form font data. **/
/** Resources:		None.
 * **/
/** Params:
 * **/
/**	@ sStartAddr[in]: Read start address in memory.
 * **/
/**	@ pDataBuffer[in]: Character data dump buffer pointer.
 * **/
/**	@ sReadSize[in]: Size of data will be read, always mean the buffer  **/
/**                 size. **/
/** Return:			Data in process was be read.
 * **/
/*************************************************************************/
static SGUI_BOOL GB2312_IsFullWidth(SGUI_UINT32 uiCode) {
    /*----------------------------------*/
    /* Variable Declaration				*/
    /*----------------------------------*/
    SGUI_BOOL bReturn;

    /*----------------------------------*/
    /* Process							*/
    /*----------------------------------*/
    if (uiCode < 0x7F) {
        bReturn = SGUI_FALSE;
    } else {
        bReturn = SGUI_TRUE;
    }
    return bReturn;
}
