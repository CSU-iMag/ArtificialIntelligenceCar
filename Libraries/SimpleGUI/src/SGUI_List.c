/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_List.c												**/
/** Author: XuYulin														**/
/** Description: Draw list interface.									**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_List.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
// User settings
#define		LIST_SCROLLBAR_WIDTH						(5)
// Automatic calculation
#define		LIST_EDGE_SIZE								1
#define		LIST_TITLE_HEIGHT(FONT)               		((FONT)->iHeight+2)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_List_InitializeListData						**/
/** Purpose:		Initialize a list control data.						**/
/** Params:																**/
/**	@ pstObj[in]:		Pointer of list data will be initialized.		**/
/** @ pstFontRes[in]: Font resource object pointer.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_List_Initialize(SGUI_LIST_STRUCT* pstObj, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SCROLLBAR_PARAM	stScrollBarParam;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstFontRes))
	{
		// Initialize member object pointer.
		pstObj->pstFontRes = pstFontRes;
		pstObj->stItems.pstItems = NULL;
		pstObj->stItems.stLayout.iX = pstObj->stLayout.iX+2;
		pstObj->stItems.stLayout.iWidth = pstObj->stLayout.iWidth-4-LIST_SCROLLBAR_WIDTH;
		if(NULL == pstObj->szTitle)
		{
			pstObj->stItems.stLayout.iY = pstObj->stLayout.iY+2;
			pstObj->stItems.stLayout.iHeight = pstObj->stLayout.iHeight-4;
		}
		else
		{
			pstObj->stItems.stLayout.iY = pstObj->stLayout.iY+LIST_TITLE_HEIGHT(pstObj->pstFontRes)+2;
			pstObj->stItems.stLayout.iHeight = pstObj->stLayout.iHeight-LIST_TITLE_HEIGHT(pstObj->pstFontRes)-3;
		}
		SGUI_ItemsBase_Initialize(&(pstObj->stItems), pstObj->pstFontRes, pstItemsData, iItemsCount);
		// Initialize scroll bar.
		stScrollBarParam.eDirection = SGUI_SCROLLBAR_VERTICAL;
		stScrollBarParam.stLayout.iX = pstObj->stItems.stLayout.iX+pstObj->stItems.stLayout.iWidth+1;
		stScrollBarParam.stLayout.iY = pstObj->stItems.stLayout.iY;
		stScrollBarParam.stLayout.iWidth = LIST_SCROLLBAR_WIDTH;
		stScrollBarParam.stLayout.iHeight = pstObj->stItems.stLayout.iHeight;
		stScrollBarParam.sMaxValue = (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0;
		SGUI_ScrollBar_Initialize(&(pstObj->stScrollBar), &stScrollBarParam);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_List_Repaint									**/
/** Purpose:		Refresh list display.								**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Pointer of list data will be refreshed.				**/
/** Return:			None.												**/
/** Notice:			This function will refresh all list display on		**/
/**					screen, include edge, items, title and scrollbar.	**/
/*************************************************************************/
void SGUI_List_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_LIST_STRUCT* pstObj)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT				stTitleTextDisplayArea;
	SGUI_POINT				stInnerPos;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstObj)
	{
		// Clear list item display area.
		SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
		// Paint title.
		if(NULL != pstObj->szTitle)
		{
			stTitleTextDisplayArea.iX = pstObj->stLayout.iX+2;
			stTitleTextDisplayArea.iY = pstObj->stLayout.iY+2;
			stTitleTextDisplayArea.iWidth = pstObj->stLayout.iWidth-4;
			stTitleTextDisplayArea.iHeight = pstObj->pstFontRes->iHeight;
			stInnerPos.iX = 0;
			stInnerPos.iY = 0;
			SGUI_Text_DrawText(pstDeviceIF, pstObj->szTitle, pstObj->pstFontRes, &stTitleTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
			SGUI_Basic_DrawLine(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, pstObj->stLayout.iX+pstObj->stLayout.iWidth-1, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, SGUI_COLOR_FRGCLR);
		}
		// Paint items.
		SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
		// Paint scroll bar.
		SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.iPageStartIndex);
		SGUI_ScrollBar_Repaint(pstDeviceIF, &(pstObj->stScrollBar));
	}
}
