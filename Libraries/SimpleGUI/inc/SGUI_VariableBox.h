#ifndef __INCLUDE_GUI_VARIABLEBOX_H__
#define __INCLUDE_GUI_VARIABLEBOX_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
// Data type of number variable box.
typedef struct
{
	SGUI_RECT				stLayout;
	SGUI_INT				iMin;
	SGUI_INT				iMax;
	const SGUI_FONT_RES*	pstFontRes;
	SGUI_ALIG_MODE			eAlignment;
}SGUI_NUM_VARBOX_PARAM;

typedef struct
{
	SGUI_INT				iValue;
}SGUI_NUM_VARBOX_DATA;

typedef struct
{
	SGUI_NUM_VARBOX_PARAM	stParam;
	SGUI_NUM_VARBOX_DATA	stData;
}SGUI_NUM_VARBOX_STRUCT;

// Data type of text variable box.
typedef struct
{
	SGUI_RECT				stLayout;
	const SGUI_FONT_RES*	pstFontRes;
	SGUI_SIZE				sTextLengthMax;
}SGUI_TEXT_VARBOX_PARAM;

typedef struct
{
	SGUI_SIZE				sFocusIndex;
	SGUI_SZSTR				szValue;
}SGUI_TEXT_VARBOX_DATA;

typedef struct
{
	SGUI_TEXT_VARBOX_PARAM	stParam;
	SGUI_TEXT_VARBOX_DATA	stData;
}SGUI_TEXT_VARBOX_STRUCT;

enum
{
	SGUI_TEXT_NUMBER		= 0x0100,
	SGUI_TEXT_ALPHA			= 0x0200,
	SGUI_TEXT_PUNCTUATION	= 0x0400,
	SGUI_TEXT_DATE			= 0x0800,
	SGUI_TEXT_TIME			= 0x1000,
};

typedef enum
{
    SGUI_TXT_VARBOX_OPT_PREV = -1,
    SGUI_TXT_VARBOX_OPT_NONE = 0,
    SGUI_TXT_VARBOX_OPT_NEXT = 1,
}SGUI_TEXT_VARBOX_OPT;

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define VARBOX_WIDTH(FONT_SIZE, L)		(L*(g_stFontSize[FONT_SIZE].Width)-4)
#define VARBOX_HEIGHT(FONT_SIZE)		(g_stFontSize[FONT_SIZE].Height+2)
#define VARBOX_TEXT_AREA_WIDTH(W)		((W>2)?(W-2):0)
#define VARBOX_TEXT_AREA_HEIGHT(FONT_SIZE) (g_stFontSize[FONT_SIZE].Height)

#define VARBOX_TEXT_LENGTH_MAX			(11)
#define VARBOX_TEXT_BUFFER_SIZE			(VARBOX_TEXT_LENGTH_MAX+1)
#define VARBOX_TEXT_WIDTH(FONT_SIZE, L)	(L*(g_stFontSize[FONT_SIZE].Width))

#define SGUI_TEXT_ASCII					(SGUI_TEXT_NUMBER|SGUI_TEXT_ALPHA|SGUI_TEXT_PUNCTUATION)

#define GUI_TEXT_ISLOWER(C)				((C>='a' && C<='z')?true:false)
#define GUI_TEXT_ISALPHA(C)				(SGUI_IS_UPPER(C) || GUI_TEXT_ISLOWER(C))
#define GUI_TEXT_ISASCII(C)				((C>=0x20 && C<=0x7E)?true:false)

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#define		SGUI_NumberVariableBox_Increase(POBJ)			SGUI_NumberVariableBox_SetValue((POBJ), (POBJ)->stData.iValue+1)
#define		SGUI_NumberVariableBox_Decrease(POBJ)			SGUI_NumberVariableBox_SetValue((POBJ), (POBJ)->stData.iValue-1)
#define		SGUI_NumberVariableBox_SetValue(POBJ, VAL)		if(((VAL) <= (POBJ)->stParam.iMax) && ((VAL) >= (POBJ)->stParam.iMin))\
															{\
																(POBJ)->stData.iValue = (VAL);\
															}
#define		SGUI_NumberVariableBox_GetValue(POBJ)			((POBJ)->stData.iValue)
void		SGUI_NumberVariableBox_Initialize(SGUI_NUM_VARBOX_STRUCT* pstObj, const SGUI_NUM_VARBOX_PARAM* pcstInitParam);
void		SGUI_NumberVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_NUM_VARBOX_STRUCT* pstValue, SGUI_DRAW_MODE eMode);
void		SGUI_TextVariableBox_Initialize(SGUI_TEXT_VARBOX_STRUCT* pstObj, const SGUI_TEXT_VARBOX_PARAM* pcstInitParam, SGUI_SZSTR szTextBuffer);
#define		SGUI_TextVariableBox_GetFocusIndex(POBJ)		((POBJ)->stData.sFocusIndex)
#define		SGUI_TextVariableBox_SetFocusIndex(POBJ, IDX)	if((IDX) < (POBJ)->stParam.sTextLengthMax)\
															{\
																(POBJ)->stData.sFocusIndex = (IDX);\
															}
#define		SGUI_TextVariableBox_IncreaseIndex(POBJ)		{SGUI_TextVariableBox_SetFocusIndex((POBJ), (POBJ)->stData.sFocusIndex+1);}
#define		SGUI_TextVariableBox_DecreaseIndex(POBJ)		{SGUI_TextVariableBox_SetFocusIndex((POBJ), (POBJ)->stData.sFocusIndex-1);}
#define		SGUI_TextVariableBox_FocusedChar(POBJ)          ((POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex])
#define		SGUI_TextVariableBox_IncreaseChar(POBJ)			if((POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex] == 0x7E)\
															{\
																(POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex] = 0x20;\
															}\
															else\
															{\
																(POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex]++;\
															}
#define		SGUI_TextVariableBox_DecreaseChar(POBJ)			if((POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex] == 0x20)\
															{\
																(POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex] = 0x7E;\
															}\
															else\
															{\
																(POBJ)->stData.szValue[(POBJ)->stData.sFocusIndex]--;\
															}
#define 	SGUI_TextVariableBox_GetText(POBJ)				((POBJ)->stData.szValue)
#define		SGUI_TextVariableBox_GetChar(POBJ, IDX)			(IDX<(POBJ)->stParam.sMaxTextLength?(POBJ)->stData.szValue[IDX]:'\0')
void		SGUI_TextVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_TEXT_VARBOX_STRUCT* pstObj, SGUI_DRAW_MODE eMode);

#endif // __INCLUDE_GUI_VARIABLEBOX_H__
