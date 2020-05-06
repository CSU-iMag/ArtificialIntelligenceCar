#ifndef _INCLUDE_SGUI_ITEMS_BASE_H_
#define _INCLUDE_SGUI_ITEMS_BASE_H_
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		ITEM_EDGE_WIDTH						(1)
#define		ITEM_HEIGHT(FONT)					((FONT)->iHeight+(ITEM_EDGE_WIDTH*2))

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct _st_sgui_items_item_
{
	SGUI_CSZSTR					cszLabelText;					// This pointer points to a character array constant
    //    char szVariableText[19];
    SGUI_SZSTR					szVariableText;					// This pointer points to a character array variable, this member will be used first if existed.
}SGUI_ITEMS_ITEM;

typedef struct
{
	SGUI_RECT					stLayout;
	SGUI_ITEMS_ITEM*			pstItems;						// Read only
	SGUI_INT					iCount;
	SGUI_INT					iSelection;						// Read only
	SGUI_INT					iPageStartIndex;				// Read only
	SGUI_INT					iPageEndIndex;					// Read only
	SGUI_INT					iVisibleItems;
	SGUI_INT					iItemPaintOffset;
	const SGUI_FONT_RES*        pstFontRes;
}SGUI_ITEMS_BASE;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void				SGUI_ItemsBase_Initialize(SGUI_ITEMS_BASE* pstObj, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount);
void				SGUI_ItemsBase_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_ITEMS_BASE* pstObj);
SGUI_ITEMS_ITEM*	SGUI_ItemsBase_GetItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection);
void				SGUI_ItemsBase_GetItemExtent(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection, SGUI_RECT* pstItemExtent);
void				SGUI_ItemsBase_FitLayout(SGUI_ITEMS_BASE* pstObj);

#endif // _INCLUDE_SGUI_ITEMS_BASE_H_
