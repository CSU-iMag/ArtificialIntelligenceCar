#ifndef __INCLUDE_GUI_SCROLLBAR__
#define __INCLUDE_GUI_SCROLLBAR__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Basic.h"
#include "SGUI_Common.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	SGUI_SCROLLBAR_VERTICAL = 0,						//Vertical scroll bar.
	SGUI_SCROLLBAR_HORIZONTAL,						//Horizontal scroll bar.
}SGUI_SCROLLBAR_DIRECTION;

typedef struct
{
	SGUI_RECT					stLayout;
    SGUI_SIZE					sMaxValue;
    SGUI_SCROLLBAR_DIRECTION	eDirection;
}SGUI_SCROLLBAR_PARAM;

typedef struct
{
	SGUI_SIZE					sValue;
}SGUI_SCROLLBAR_DATA;

typedef struct
{
	SGUI_SCROLLBAR_PARAM		stParam;
	SGUI_SCROLLBAR_DATA			stData;
}SGUI_SCROLLBAR_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void		SGUI_ScrollBar_Initialize(SGUI_SCROLLBAR_STRUCT* pstObj, const SGUI_SCROLLBAR_PARAM* pcstInitParam);
void		SGUI_ScrollBar_SetValue(SGUI_SCROLLBAR_STRUCT* pstObj, SGUI_SIZE sNewValue);
SGUI_SIZE	SGUI_ScrollBar_GetValue(const SGUI_SCROLLBAR_STRUCT* pstObj);
void		SGUI_ScrollBar_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_SCROLLBAR_STRUCT* pstObj);

#endif // __INCLUDE_GUI_SCROLLBAR_STRUCT__
