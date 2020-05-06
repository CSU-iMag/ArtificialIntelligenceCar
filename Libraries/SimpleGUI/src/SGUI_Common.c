/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Common.c												**/
/** Author: Polarix														**/
/** Description: Simple GUI common process.								**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Common_AdaptDisplayInfo						**/
/** Purpose:		Adapt display area data and internal position data	**/
/**					order by display area position and screen size.		**/
/** Params:																**/
/**	@ pstDisplayArea[in/out]: Display area info.						**/
/**	@ pstInnerPos[in/out]: Display position info in display rectangular	**/
/**					area .												**/
/** Return:			None.												**/
/** Notice:			This function will reset display information on		**/
/**					given data, make sure the display area is within	**/
/**					the screen area.									**/
/*************************************************************************/
void SGUI_Common_AdaptDisplayInfo(SGUI_RECT* pstDisplayArea, SGUI_POINT* pstInnerPos)
{
	if((NULL != pstDisplayArea) && (NULL != pstInnerPos))
	{
		if(RECT_X_START(*pstDisplayArea) < 0)
		{
			RECT_X_START(*pstInnerPos) += RECT_X_START(*pstDisplayArea);
			RECT_WIDTH(*pstDisplayArea) += RECT_X_START(*pstDisplayArea);
			RECT_X_START(*pstDisplayArea) = 0;
		}
		if(RECT_Y_START(*pstDisplayArea) < 0)
		{
			RECT_Y_START(*pstInnerPos) += RECT_Y_START(*pstDisplayArea);
			RECT_HEIGHT(*pstDisplayArea) += RECT_Y_START(*pstDisplayArea);
			RECT_Y_START(*pstDisplayArea) = 0;
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Common_IntegerToStringWithDecimalPoint			**/
/** Purpose:		Convert number to a string and insert decimal point.**/
/** Params:																**/
/**	@iInteger[in]:		Source number.									**/
/**	@uiDecimalPlaces[in]: Decimal point places.							**/
/**	@pszStringBuffer[out]: Bit map data buffer.							**/
/**	@iAlignment[in]:	Alignment of number string, positive numbers 	**/
/**						means right aligned and negative means left 	**/
/**						alignment.										**/
/**	@cFillCharacter[in]:When string length less then align width, use 	**/
/**						this character fill the space.					**/
/** Return:			Converted string length(include space).				**/
/** Notice:			Only applies to decimal numbers.					**/
/*************************************************************************/
SGUI_SIZE SGUI_Common_IntegerToStringWithDecimalPoint(SGUI_INT iInteger, SGUI_UINT uiDecimalPlaces, SGUI_SZSTR pszStringBuffer, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CHAR					szNumberString[SGUI_NUMBER_STR_LENGTH_MAX] = {0x00};
	SGUI_UINT					uiSourceNumber;
	SGUI_UINT					uiSignBit;
	SGUI_SIZE					uiNumberStringLength;
	SGUI_SIZE					uiDecimalLength;
	SGUI_SIZE					uiOutputLength;
	SGUI_SZSTR					pcSrcPtr;
	SGUI_SZSTR					pcDestPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	// Judged sign
	if(iInteger < 0)
	{
		uiSignBit = 1;
		uiSourceNumber = -iInteger;
	}
	else
	{
		uiSignBit = 0;
		uiSourceNumber = iInteger;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Insert dot to string.
	if(uiDecimalPlaces > 0)
	{
		uiDecimalLength = uiDecimalPlaces;
		// Convert number to string, ignore sign.
		uiNumberStringLength = SGUI_Common_IntegerToString(uiSourceNumber, szNumberString, 10, 0, cFillCharacter);
        // Set pointer.
		pcSrcPtr = szNumberString + uiNumberStringLength - 1;
		if(uiDecimalLength < uiNumberStringLength)
		{
			uiOutputLength = uiNumberStringLength + 1;
		}
		else
		{
			uiOutputLength = uiDecimalLength + 2;
		}
		pcDestPtr = pszStringBuffer + uiOutputLength + uiSignBit;
		// Process decimal string character.
		*pcDestPtr = '\0';
		pcDestPtr--;
		while((pcSrcPtr >= szNumberString) && (uiDecimalLength > 0))
		{
			*pcDestPtr = *pcSrcPtr;
			pcDestPtr--;
			pcSrcPtr--;
			uiDecimalLength--;
		}
		while(uiDecimalLength > 0)
		{
			*pcDestPtr = '0';
			pcDestPtr--;
			uiDecimalLength --;
		}
		*pcDestPtr = '.';
		pcDestPtr--;
		// Process integer string character.
		if(uiDecimalPlaces >= uiNumberStringLength)
		{
			*pcDestPtr = '0';
			pcDestPtr--;
		}
		else
		{
            while(pcSrcPtr >= szNumberString)
			{
				*pcDestPtr = *pcSrcPtr;
				pcDestPtr--;
				pcSrcPtr--;
			}
		}
		if(uiSignBit > 0)
		{
			*pcDestPtr = '-';
		}
		// Alignment
		uiOutputLength = uiOutputLength + uiSignBit;
		if(iAlignment > 0)
		{
			pcSrcPtr = pszStringBuffer + uiOutputLength - 1;
			if(uiOutputLength > iAlignment)
			{
				iAlignment = uiOutputLength;
			}
			else
			{
				uiOutputLength = iAlignment;
			}
			pcDestPtr = pszStringBuffer + iAlignment;
			*pcDestPtr = '\0';
			pcDestPtr--;

			while(pcDestPtr >= pszStringBuffer)
			{
				if(pcSrcPtr >= pszStringBuffer)
				{
					*pcDestPtr = *pcSrcPtr;
					pcSrcPtr--;
				}
				else
				{
					*pcDestPtr = cFillCharacter;
				}
				pcDestPtr--;
			}
		}
		else
		{
			iAlignment = -iAlignment;
			pcSrcPtr = pszStringBuffer;
			pcDestPtr = pszStringBuffer + uiOutputLength;
			if(uiOutputLength > iAlignment)
			{
				iAlignment = 0;
			}
			else
			{
				iAlignment = iAlignment - uiOutputLength;
				uiOutputLength = uiOutputLength + iAlignment;
			}
			while(iAlignment --)
			{
				*pcDestPtr = cFillCharacter;
				pcDestPtr ++;
			}
			*pcDestPtr = '\0';
		}

	}
	else
	{
		// Convert number to string.
		uiOutputLength = SGUI_Common_IntegerToString(iInteger, pszStringBuffer, 10, iAlignment, cFillCharacter);
	}

	return uiOutputLength;
}

/*************************************************************************/
/** Function Name:	SGUI_Common_IntegerToString							**/
/** Purpose:		Convert number to a string.							**/
/** Params:																**/
/**	@iInteger[in]:		Source number.									**/
/**	@pszStringBuffer[out]: Bit map data buffer.							**/
/**	@uiBase[in]:		Conversion base.								**/
/**	@iAlignment[in]:	Alignment of number string, positive numbers 	**/
/**						means right aligned and negative means left 	**/
/**						alignment.										**/
/**	@cFillCharacter[in]:When string length less then align width, use 	**/
/**						this character fill the space.					**/
/** Return:			Converted string length.							**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_SIZE SGUI_Common_IntegerToString(SGUI_INT iInteger, SGUI_SZSTR pszStringBuffer, SGUI_UINT uiBase, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CHAR					szNumberString[SGUI_NUMBER_STR_LENGTH_MAX] = {0x00};
	SGUI_UINT					uiSourceNumber;
	SGUI_UINT					uiSignBit;
	SGUI_SIZE					uiNumberStringLength;
    SGUI_SZSTR					pcSrcPtr;
    SGUI_SZSTR					pcDestPtr;
    SGUI_SIZE					uiDigitBit;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pcSrcPtr = szNumberString + SGUI_NUMBER_STR_LENGTH_MAX - 1;
	pcDestPtr = szNumberString;
	*pcSrcPtr = '\0';
	pcSrcPtr--;
	uiNumberStringLength = 0;
	// Judged sign
    if(iInteger < 0)
	{
		if(uiBase == 16)
		{
			uiSignBit = 0;
			uiSourceNumber = (SGUI_UINT)iInteger;
		}
		else
		{
			uiSignBit = 1;
			uiSourceNumber = -iInteger;
		}
	}
	else
	{
		uiSignBit = 0;
		uiSourceNumber = (SGUI_UINT)iInteger;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Convert number to string.
    while(uiSourceNumber != 0)
	{
		uiDigitBit = uiSourceNumber % uiBase;
		if(uiDigitBit < 10)
		{
			*pcSrcPtr = uiDigitBit + '0';
		}
		else
		{
			*pcSrcPtr = uiDigitBit - 10 + 'A';
		}
		uiSourceNumber = uiSourceNumber / uiBase;
		pcSrcPtr--;
		uiNumberStringLength++;
	}
	if(uiSignBit == 1)
	{
		*pcSrcPtr = '-';
		uiNumberStringLength++;
	}
	else
	{
		pcSrcPtr++;
	}

	while((pcDestPtr - uiNumberStringLength) != szNumberString)
	{
		*pcDestPtr = *pcSrcPtr;
		pcSrcPtr++;
		pcDestPtr++;
	}
	if(uiNumberStringLength == 0)
	{
		pcDestPtr = szNumberString;
		*pcDestPtr = '0';
		pcDestPtr++;
		uiNumberStringLength++;
	}
	*pcDestPtr = '\0';
	// Move to out put buffer
	if(iAlignment > 0)
	{
		pcSrcPtr = szNumberString + uiNumberStringLength-1;
		if(uiNumberStringLength > iAlignment)
		{
			iAlignment = uiNumberStringLength;
		}
		else
		{
			uiNumberStringLength = iAlignment;
		}
		pcDestPtr = pszStringBuffer + iAlignment;
		*pcDestPtr = '\0';
		pcDestPtr--;

		while(pcDestPtr >= pszStringBuffer)
		{
			if(pcSrcPtr >= szNumberString)
			{
				*pcDestPtr = *pcSrcPtr;
				pcSrcPtr--;
			}
			else
			{
				*pcDestPtr = cFillCharacter;
			}
			pcDestPtr--;
		}
	}
	else
	{
		iAlignment = -iAlignment;
		if(uiNumberStringLength > iAlignment)
		{
			iAlignment = uiNumberStringLength;
		}
		else
		{
			uiNumberStringLength = iAlignment;
		}
		pcDestPtr = pszStringBuffer;
		pcSrcPtr = szNumberString;
		while(iAlignment > 0)
		{
			if(*pcSrcPtr != '\0')
			{
				*pcDestPtr = *pcSrcPtr;
				pcSrcPtr++;
			}
			else
			{
				*pcDestPtr = cFillCharacter;
			}
			pcDestPtr++;
			iAlignment--;
		}
		*pcDestPtr = '\0';
	}
	return uiNumberStringLength;
}

/*************************************************************************/
/** Function Name:	SGUI_Common_ConvertStringToUnsignedInteger			**/
/** Purpose:		Convert a string to a unsigned integer.				**/
/** Params:																**/
/**	@szString[in]:		Source number.									**/
/**	@ppcEndPointer[out]: Convert end pointer output in string.			**/
/**	@uiBase[in]:		Conversion base.								**/
/** Return:			Converted number.									**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_UINT SGUI_Common_ConvertStringToUnsignedInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    SGUI_UINT32					uiResult;
	SGUI_UINT					uiBitValue;
	SGUI_SZSTR					pcCurPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiResult = 				0;
	pcCurPtr =				szString;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pcCurPtr)
	{
		// Skip header space.
		while(' ' == *pcCurPtr)
		{
			pcCurPtr++;
		}
		// Convert string.
		if(uiBase == 16)
		{
			if((*(pcCurPtr) == '0') && ((*(pcCurPtr+1) == 'x') || (*(pcCurPtr+1) == 'X')))
			{
				pcCurPtr += 2;
			}
			while(('\0' != (*pcCurPtr)) && (true == SGUI_ISHEXDIGIT(*pcCurPtr)))
			{
				uiBitValue = SGUI_ISDIGIT(*pcCurPtr)?((*pcCurPtr)-'0'):(10+SGUI_TOUPPER(*pcCurPtr)-'A');
				uiResult = uiResult*uiBase + uiBitValue;
				pcCurPtr++;
			}
		}
		else if((uiBase == 8) || (uiBase == 10))
		{
			while(('\0' != (*pcCurPtr)) && (true == SGUI_ISDIGIT(*pcCurPtr)))
			{
				uiBitValue =(*pcCurPtr)-'0';
				uiResult = uiResult*uiBase + uiBitValue;
				pcCurPtr++;
			}
		}
	}
	if(NULL != ppcEndPointer)
	{
		*ppcEndPointer = pcCurPtr;
	}
    return uiResult;
}

/*************************************************************************/
/** Function Name:	SGUI_Common_ConvertStringToInteger					**/
/** Purpose:		Convert a string to a signed integer.				**/
/** Params:																**/
/**	@szString[in]:		Source number.									**/
/**	@ppcEndPointer[in]:	Convert end pointer output in string.			**/
/**	@uiBase[in]:		Conversion base.								**/
/** Return:			Converted number.									**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_INT SGUI_Common_ConvertStringToInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    SGUI_INT					iResult;
    SGUI_INT					iSign;
	SGUI_SZSTR					pcCurPtr;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iResult = 				0;
	iSign =					1;
	pcCurPtr =				szString;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pcCurPtr)
	{
		// Skip header space.
		while(' ' == *pcCurPtr)
		{
			pcCurPtr++;
		}
		// Check sign.
		if((10 == uiBase) || (8 == uiBase))
		{
			if('+' == *pcCurPtr)
			{
				pcCurPtr++;
			}
			else if('-' == *pcCurPtr)
			{
				pcCurPtr++;
				iSign = -1;
			}
		}
		// Convert string without sign.
		iResult = SGUI_Common_ConvertStringToUnsignedInteger(pcCurPtr, ppcEndPointer, uiBase);
		if((iResult > 0) && (-1 == iSign))
		{
			iResult = -iResult;
		}
	}
	return iResult;
}

