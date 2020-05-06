/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_VariableBox.c										**/
/** Author: XuYulin														**/
/** Description: Show and change variable box.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_VariableBox.h"

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_NumberVariableBox_Initialize					**/
/** Purpose:		Initialize a integer value edit box structure.		**/
/** Params:																**/
/**	@ pstObj[in]:	Number variable box object pointer.					**/
/**	@ pcstInitParam[in]: Initialize parameter for initialize.			**/
/** Return:			None.												**/
/** Notice:			iValue will be changed when more then max value or	**/
/**					less then minimum value.							**/
/*************************************************************************/
void SGUI_NumberVariableBox_Initialize(SGUI_NUM_VARBOX_STRUCT* pstObj, const SGUI_NUM_VARBOX_PARAM* pcstInitParam)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pcstInitParam))
	{
		SGUI_SystemIF_MemorySet(pstObj, 0x00, sizeof(SGUI_NUM_VARBOX_STRUCT));
		SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_NUM_VARBOX_PARAM));
	}
}

/*************************************************************************/
/** Function Name:	SGUI_NumberVariableBox_Repaint						**/
/** Purpose:		Display or refresh a integer value edit box.		**/
/** Params:																**/
/**	@ pstDeviceIF[in]:	Device driver object pointer.					**/
/**	@ pstObj[in]:	Value structure, include max value, min value and	**/
/**					current value.										**/
/**	@ eMode[in]:	Display mode, normal or reveres.					**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_NumberVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_NUM_VARBOX_STRUCT* pstObj, SGUI_DRAW_MODE eMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_AREA_SIZE				stTextExtentSize;
	SGUI_POINT					stTextInnerPos;
	SGUI_COLOR					eBackColor;
	SGUI_CHAR					szTextBuffer[VARBOX_TEXT_BUFFER_SIZE];

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	SGUI_SystemIF_MemorySet(szTextBuffer, 0x00, VARBOX_TEXT_BUFFER_SIZE);
	eBackColor =				((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/

	if(NULL != pstObj)
	{
		// Draw edge
		SGUI_Basic_DrawRectangle(pstDeviceIF, LAYOUT(pstObj).iX, LAYOUT(pstObj).iY, LAYOUT(pstObj).iWidth, LAYOUT(pstObj).iHeight, eBackColor, eBackColor);

		// Convert number to string
		(void)SGUI_Common_IntegerToString(pstObj->stData.iValue, szTextBuffer, 10, -1, ' ');
		SGUI_Text_GetTextExtent(szTextBuffer, pstObj->stParam.pstFontRes, &stTextExtentSize);
		switch(pstObj->stParam.eAlignment)
		{
			case SGUI_RIGHT:
			{
				stTextInnerPos.iX = LAYOUT(pstObj).iWidth - stTextExtentSize.iWidth;
				break;
			}
			case SGUI_CENTER:
			{
				stTextInnerPos.iX = (LAYOUT(pstObj).iWidth - stTextExtentSize.iWidth) / 2;
				break;
			}
			default:
			{
				stTextInnerPos.iX = 0;
			}
		}
		stTextInnerPos.iY = 0;
		SGUI_Text_DrawText(pstDeviceIF, szTextBuffer, pstObj->stParam.pstFontRes, &(LAYOUT(pstObj)), &stTextInnerPos, eMode);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_TextVariableBox_Initialize						**/
/** Purpose:		Initialize a text value edit box structure.			**/
/** Params:																**/
/**	@ pstObj[in]:	Text variable box object pointer.					**/
/**	@ pcstInitParam[in]: Parameter data for initialize.					**/
/**	@ szTextBuffer[in]: Text buffer for text variable.					**/
/** Return:			None.												**/
/** Notice:			TextBuffer length cannot less then sMaxTextLength.	**/
/*************************************************************************/
void SGUI_TextVariableBox_Initialize(SGUI_TEXT_VARBOX_STRUCT* pstObj, const SGUI_TEXT_VARBOX_PARAM* pcstInitParam, SGUI_SZSTR szTextBuffer)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pcstInitParam))
	{
		SGUI_SystemIF_MemorySet(pstObj, 0x00, sizeof(SGUI_TEXT_VARBOX_STRUCT));
		SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_TEXT_VARBOX_PARAM));
		pstObj->stData.szValue = szTextBuffer;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_TextVariableBox_Paint							**/
/** Purpose:		Display or refresh a integer value edit box.		**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/** @ pstObj[in]:	Text value edit box pointer.						**/
/** @ cNewCharacters[in]: New character of value.						**/
/**	@ eMode[in]:			Display mode, normal or reveres.			**/
/** Return:			None.												**/
/** Notice:			Static function, call by others interface.			**/
/*************************************************************************/
void SGUI_TextVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_TEXT_VARBOX_STRUCT* pstObj, SGUI_DRAW_MODE eMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_COLOR				eBackColor;
	SGUI_POINT				stTextInnerPos;
	SGUI_RECT				stFocusArea;
	SGUI_UINT16				uiFontWidth, uiFontHeight;
	SGUI_SIZE				uiTextLength, uiFocusIndexMax;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eBackColor =			((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstObj->stData.szValue))
	{
		// Clear background.
		SGUI_Basic_DrawRectangle(pstDeviceIF, LAYOUT(pstObj).iX, LAYOUT(pstObj).iY, LAYOUT(pstObj).iWidth, LAYOUT(pstObj).iHeight, eBackColor, eBackColor);
		// Get font graphics size.
		uiFontWidth =		pstObj->stParam.pstFontRes->iHalfWidth;
		uiFontHeight =		pstObj->stParam.pstFontRes->iHeight;

		// Get max text length and graphics width.
		uiFocusIndexMax = pstObj->stParam.sTextLengthMax-1;
		// Ignore too long text string.
		uiTextLength = SGUI_SystemIF_StringLength(pstObj->stData.szValue);
		if(uiTextLength > pstObj->stParam.sTextLengthMax)
		{
			uiTextLength = pstObj->stParam.sTextLengthMax;
			*(pstObj->stData.szValue+uiTextLength) = '\0';
            // Point at to last character position if index is more then string length.
			if(pstObj->stData.sFocusIndex > uiFocusIndexMax)
			{
				pstObj->stData.sFocusIndex = uiFocusIndexMax;
			}
		}
		// Set text display area.
		stTextInnerPos.iX = 0;
		stTextInnerPos.iY = 0;
		// Set focus character area.
		stFocusArea.iX = LAYOUT(pstObj).iX+pstObj->stData.sFocusIndex*uiFontWidth;
		stFocusArea.iY = LAYOUT(pstObj).iY;
		stFocusArea.iWidth = uiFontWidth;
		stFocusArea.iHeight = uiFontHeight;
		if(RECT_X_END(stFocusArea) > RECT_X_END(LAYOUT(pstObj)))
		{
			stTextInnerPos.iX = RECT_X_END(LAYOUT(pstObj)) - RECT_X_END(stFocusArea);
			stFocusArea.iX = stFocusArea.iX + stTextInnerPos.iX;
		}
		// Display text.
		SGUI_Text_DrawText(pstDeviceIF, pstObj->stData.szValue, pstObj->stParam.pstFontRes, &LAYOUT(pstObj), &stTextInnerPos, eMode);
		// Focus first character.
        SGUI_Basic_ReverseBlockColor(pstDeviceIF, stFocusArea.iX, stFocusArea.iY, stFocusArea.iWidth, stFocusArea.iHeight);
	}
}
