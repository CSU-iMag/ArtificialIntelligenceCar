#ifndef __INCLUDE_GUI_PROCESSBAR_H__
#define __INCLUDE_GUI_PROCESSBAR_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	SGUI_PROCBAR_UP = 0,							//Process bar direction to up.
	SGUI_PROCBAR_DOWN,							//Process bar direction to down.
	SGUI_PROCBAR_LEFT,							//Process bar direction to left.
	SGUI_PROCBAR_RIGHT,							//Process bar direction to right.
}SGUI_PROCBAR_DIRECTION;

typedef struct
{
	SGUI_INT				PosX;
	SGUI_INT				PosY;
	SGUI_SIZE				Width;
	SGUI_SIZE				Height;
	SGUI_PROCBAR_DIRECTION	Direction;
	SGUI_UINT32				MaxValue;

}SGUI_PROCBAR_PARAMETER;

typedef struct
{
	SGUI_SIZE				Value;
}SGUI_PROCBAR_DATA;

typedef struct
{
	SGUI_PROCBAR_PARAMETER	Parameter;
	SGUI_PROCBAR_DATA		Data;
}SGUI_PROCBAR_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void SGUI_ProcessBar_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_PROCBAR_STRUCT *pProcessBarData);

#endif // __INCLUDE_GUI_PROCESSBAR_H__
