/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Menu.c												**/
/** Author: Polarix														**/
/** Description: Menu component interface.								**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Menu.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define	SGUI_MENU_ICON_DECLARE(NAME)				extern const SGUI_BMP_RES NAME
#define	SGUI_MENU_ICON_DEFINE(NAME, W, H, ...)		const SGUI_BYTE NAME##DATA[] = {__VA_ARGS__};\
													const SGUI_BMP_RES NAME = {W, H, NAME##DATA}
//=======================================================================//
//= Static variable define.												=//
//=======================================================================//
static SGUI_MENU_ICON_DEFINE(SGUI_MENU_ICON_MOVEUP, 5, 3,
0x04,0x06,0x07,0x06,0x04);
static SGUI_MENU_ICON_DEFINE(SGUI_MENU_ICON_MOVEDOWN, 5, 3,
0x01,0x03,0x07,0x03,0x01);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Menu_Initialize								**/
/** Purpose:		Initialize a menu structure data.					**/
/** Params:																**/
/**	@ pstObj[in]:	Pointer of menu data will be initialized.			**/
/** @ pstFontRes[in]: Font resource object pointer.						**/
/** @ pstItemsData[in]: List items data array.							**/
/** @ iItemsCount[in]: Number of list item data.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Menu_Initialize(SGUI_MENU_STRUCT* pstObj, const SGUI_FONT_RES* pstFontRes, const SGUI_RECT* cpstLayout, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstFontRes))
	{
		// Initialize member object pointer.
		pstObj->pstFontRes = pstFontRes;
		pstObj->stItems.pstItems = NULL;
		SGUI_Menu_Layout(pstObj, cpstLayout);
		SGUI_ItemsBase_Initialize(&(pstObj->stItems), pstObj->pstFontRes, pstItemsData, iItemsCount);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Menu_Repaint									**/
/** Purpose:		Refresh list display.								**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Pointer of menu structure will be refreshed.		**/
/** Return:			None.												**/
/** Notice:			This function will refresh all list display on		**/
/**					screen, include edge, items, title and scrollbar.	**/
/*************************************************************************/
void SGUI_Menu_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU_STRUCT* pstObj)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT				stIconArea;
	SGUI_POINT				stIconInnerPos;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stIconInnerPos.iX =		0;
	stIconInnerPos.iY =		0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstObj)
	{
		/* Clear list item display area. */
		SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
		// Paint items.
		SGUI_ItemsBase_FitLayout(&(pstObj->stItems));
		SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
		/* Paint arrow icon. */
		stIconArea.iWidth = 5;
		stIconArea.iHeight = 3;
		stIconArea.iX = pstObj->stLayout.iX+((pstObj->stLayout.iWidth-stIconArea.iWidth)/2);
		if(pstObj->stItems.iPageStartIndex != 0)
		{
			stIconArea.iY = pstObj->stLayout.iY+1;
            SGUI_Basic_DrawBitMap(pstDeviceIF, &stIconArea, &stIconInnerPos, &SGUI_MENU_ICON_MOVEUP, SGUI_DRAW_NORMAL);
		}
		if(pstObj->stItems.iPageEndIndex < (pstObj->stItems.iCount-1))
		{
			stIconArea.iY = RECT_Y_END(pstObj->stItems.stLayout)+1;
			SGUI_Basic_DrawBitMap(pstDeviceIF, &stIconArea, &stIconInnerPos, &SGUI_MENU_ICON_MOVEDOWN, SGUI_DRAW_NORMAL);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Menu_FitLayout									**/
/** Purpose:		Resize menu to best fit size order by position and	**/
/**					screen size.										**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Pointer of menu structure will be refreshed.		**/
/** Return:			None.												**/
/** Notice:			This function must called after initialize.			**/
/*************************************************************************/
void SGUI_Menu_Layout(SGUI_MENU_STRUCT* pstObj, const SGUI_RECT* cpstNewLayout)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != cpstNewLayout))
	{
		pstObj->stLayout.iX = cpstNewLayout->iX;
		pstObj->stLayout.iY = cpstNewLayout->iY;
		pstObj->stLayout.iWidth = cpstNewLayout->iWidth;
		pstObj->stLayout.iHeight = cpstNewLayout->iHeight;
		pstObj->stItems.stLayout.iX = pstObj->stLayout.iX+1;
		pstObj->stItems.stLayout.iY = pstObj->stLayout.iY+SGUI_MENU_ICON_MOVEUP.iHeight+1;
		pstObj->stItems.stLayout.iWidth = pstObj->stLayout.iWidth-2;
		pstObj->stItems.stLayout.iHeight = pstObj->stLayout.iHeight-(SGUI_MENU_ICON_MOVEUP.iHeight+SGUI_MENU_ICON_MOVEDOWN.iHeight+2);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Menu_PopupSubMenu								**/
/** Purpose:		Resize menu to best fit size order by position and	**/
/**					screen size.										**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Pointer of menu structure will be refreshed.		**/
/**	@ cpstParentItemLayout[in]:	Pointer of parent menu item, sub menu	**/
/** 				layout will update order by parent item layout.		**/
/** Return:			None.												**/
/** Notice:			For 		**/
/**					screen, include edge, items, title and scrollbar.	**/
/*************************************************************************/
void SGUI_Menu_PopupSubMenu(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU_STRUCT* pstObj, const SGUI_RECT* cpstParentItemLayout)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_INT				iFullHeight;
	SGUI_INT				iVisibleHeight;
	SGUI_RECT				stLayout;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (pstObj->stItems.iCount >0))
	{
		/* Calculate a virtual height when display all items. */
		iFullHeight = pstObj->stItems.iCount*ITEM_HEIGHT(pstObj->pstFontRes);
		iFullHeight = iFullHeight+SGUI_MENU_ICON_MOVEUP.iHeight+SGUI_MENU_ICON_MOVEDOWN.iHeight+2;

		stLayout.iX = RECT_X_END(*cpstParentItemLayout)+1;
		iVisibleHeight = pstDeviceIF->stSize.iHeight-cpstParentItemLayout->iY-1;
		if((iVisibleHeight < iFullHeight) && (cpstParentItemLayout->iY > (pstDeviceIF->stSize.iHeight/2)))
		{
			iVisibleHeight = cpstParentItemLayout->iY+cpstParentItemLayout->iHeight+1;
			stLayout.iY = iVisibleHeight-iFullHeight-1;
			if(stLayout.iY<0)
			{
				stLayout.iY = 0;
			}
		}
		else
		{
			stLayout.iY = cpstParentItemLayout->iY;
		}
		stLayout.iWidth = cpstParentItemLayout->iWidth+2;
		if(iFullHeight > iVisibleHeight)
		{
			stLayout.iHeight = iVisibleHeight;
		}
		else
		{
			stLayout.iHeight = iFullHeight;
		}
		SGUI_Menu_Layout(pstObj, &stLayout);
	}
}
