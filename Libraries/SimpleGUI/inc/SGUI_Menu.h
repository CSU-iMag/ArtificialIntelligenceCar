#ifndef _INCLUDE_SGUI_MENU_H_
#define _INCLUDE_SGUI_MENU_H_
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_ItemsBase.h"
//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_RECT					stLayout;
	SGUI_ITEMS_BASE				stItems;
	const SGUI_FONT_RES*        pstFontRes;
}SGUI_MENU_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void SGUI_Menu_Initialize(SGUI_MENU_STRUCT* pstObj, const SGUI_FONT_RES* pstFontRes, const SGUI_RECT* cpstLayout, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount);
void SGUI_Menu_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU_STRUCT* pstObj);
void SGUI_Menu_Layout(SGUI_MENU_STRUCT* pstObj, const SGUI_RECT* pstNewLayout);
void SGUI_Menu_PopupSubMenu(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU_STRUCT* pstObj, const SGUI_RECT* cpstParentLayout);

#endif // _INCLUDE_SGUI_MENU_H_
