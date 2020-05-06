/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_ItemsBase.c											**/
/** Author: XuYulin														**/
/** Description: Simple GUI items base interface, used to extend list	**/
/**				components .											**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_ItemsBase.h"

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_ItemsBase_Initialize							**/
/** Purpose:		Initialize a items-base object data.				**/
/** Params:																**/
/** @ pstObj[in]:	Pointer of items-base object will be initialized.	**/
/** @ pstFontRes[in]: Font resource object pointer.						**/
/** @ pstItemsData[in]: Items data array.								**/
/** @ iItemsCount[in]: Number of list item data.						**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_ItemsBase_Initialize(SGUI_ITEMS_BASE* pstObj, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstFontRes))
	{
		pstObj->pstItems = pstItemsData;
		pstObj->iCount = iItemsCount;
		pstObj->pstFontRes = pstFontRes;
		pstObj->iSelection = 0;
		pstObj->iVisibleItems = (pstObj->stLayout.iHeight-1)/ITEM_HEIGHT(pstObj->pstFontRes)+1;
		pstObj->iPageStartIndex = 0;
		pstObj->iPageEndIndex = pstObj->iVisibleItems - 1;
		pstObj->iItemPaintOffset = 0;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_ItemsBase_Repaint								**/
/** Purpose:		Update list display on screen.						**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/** @ pstObj[in]:	Pointer of items-base object will be paint.			**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_ItemsBase_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_ITEMS_BASE* pstObj)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT				stItemPaintArea;
	SGUI_POINT				stItemTextPos;
	SGUI_INT				iItemIndex;
	SGUI_CSZSTR				cszItemText;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stItemTextPos.iX =		1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstDeviceIF) && (NULL != pstObj))
	{
		// Clear background
		SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
		if((pstObj->iSelection > SGUI_INVALID_INDEX) && (pstObj->iSelection < pstObj->iCount))
		{
			stItemPaintArea.iX =	pstObj->stLayout.iX;
			stItemPaintArea.iY = pstObj->stLayout.iY;
			stItemPaintArea.iWidth = pstObj->stLayout.iWidth;
			/* Judge selected item is in visible area. */
			if(pstObj->iSelection < pstObj->iPageStartIndex)
			{
				pstObj->iPageStartIndex = pstObj->iSelection;
				pstObj->iPageEndIndex = pstObj->iPageStartIndex + pstObj->iVisibleItems - 1;
			}
			else if(pstObj->iSelection > pstObj->iPageEndIndex)
			{
				pstObj->iPageEndIndex = pstObj->iSelection;
				pstObj->iPageStartIndex = pstObj->iPageEndIndex - pstObj->iVisibleItems + 1;
			}
			/* Judge selected item is beginning or ending. */
			if(pstObj->iSelection == pstObj->iPageStartIndex)
			{
				pstObj->iItemPaintOffset = 0;
				stItemPaintArea.iHeight = (pstObj->iVisibleItems>1)?(ITEM_HEIGHT(pstObj->pstFontRes)):(pstObj->stLayout.iHeight);
			}
			else if(pstObj->iSelection == pstObj->iPageEndIndex)
			{
				pstObj->iItemPaintOffset = (((pstObj->stLayout.iHeight)%ITEM_HEIGHT(pstObj->pstFontRes))-ITEM_HEIGHT(pstObj->pstFontRes))%ITEM_HEIGHT(pstObj->pstFontRes);
				stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes)+pstObj->iItemPaintOffset;
			}
			else
			{
				stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
			}

			/* Paint first visible item. */
			iItemIndex=pstObj->iPageStartIndex;
			stItemTextPos.iY = pstObj->iItemPaintOffset+1;
			cszItemText = (NULL==pstObj->pstItems[iItemIndex].szVariableText)?pstObj->pstItems[iItemIndex].cszLabelText:pstObj->pstItems[iItemIndex].szVariableText;
			SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, iItemIndex==pstObj->iSelection?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
			iItemIndex++;
			stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
			if(pstObj->iVisibleItems > 1)
			{
				stItemPaintArea.iY += ITEM_HEIGHT(pstObj->pstFontRes)+(pstObj->iItemPaintOffset);
				stItemTextPos.iY = 1;
				while((iItemIndex != pstObj->iPageEndIndex) && (iItemIndex < pstObj->iCount))
				{
					cszItemText = (NULL==pstObj->pstItems[iItemIndex].szVariableText)?pstObj->pstItems[iItemIndex].cszLabelText:pstObj->pstItems[iItemIndex].szVariableText;
					SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, iItemIndex==pstObj->iSelection?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
					stItemPaintArea.iY += ITEM_HEIGHT(pstObj->pstFontRes);
					iItemIndex++;
				}
				if(iItemIndex < pstObj->iCount)
				{
					/* Paint last visible item. */
					stItemPaintArea.iHeight = (0==pstObj->iItemPaintOffset)?(pstObj->stLayout.iHeight%ITEM_HEIGHT(pstObj->pstFontRes)):(ITEM_HEIGHT(pstObj->pstFontRes));
					/* Correct last visible item height when items area height is an integer multiple of item height. */
					stItemPaintArea.iHeight = (0==stItemPaintArea.iHeight)?ITEM_HEIGHT(pstObj->pstFontRes):stItemPaintArea.iHeight;
					cszItemText = (NULL==pstObj->pstItems[iItemIndex].szVariableText)?pstObj->pstItems[iItemIndex].cszLabelText:pstObj->pstItems[iItemIndex].szVariableText;
					SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, iItemIndex==pstObj->iSelection?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
				}
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_ItemsBase_GetItem								**/
/** Purpose:		Get item object pointer by index.					**/
/** Params:																**/
/** @ pstObj[in]:	Pointer of items-base object will be paint.			**/
/** @ iIndex[in]:	Index of wanted item.								**/
/** Return:			Item object pointer.								**/
/*************************************************************************/
SGUI_ITEMS_ITEM* SGUI_ItemsBase_GetItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_ITEMS_ITEM*			pstSelectedItem;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstSelectedItem =			NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (iIndex > SGUI_INVALID_INDEX) && (iIndex < pstObj->iCount))
	{
		pstSelectedItem = pstObj->pstItems+iIndex;
	}

	return pstSelectedItem;
}

/*************************************************************************/
/** Function Name:	SGUI_ItemsBase_GetItemExtent						**/
/** Purpose:		Get item object display area position and size.		**/
/** Params:																**/
/** @ pstObj[in]:	Pointer of items-base object will be paint.			**/
/** @ iSelection[in]: Selected item index.								**/
/** @ pstItemExtent[out]: Item position and display area size.			**/
/** Return:			None.												**/
/** Notice:			This function only used for the visible item, if	**/
/**					selection is invisible, output size will be (0, 0).	**/
/*************************************************************************/
void SGUI_ItemsBase_GetItemExtent(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection, SGUI_RECT* pstItemExtent)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (iSelection > SGUI_INVALID_INDEX) && (iSelection < pstObj->iCount) && (NULL != pstItemExtent))
	{
		pstItemExtent->iX = pstObj->stLayout.iX;
		pstItemExtent->iWidth = pstObj->stLayout.iWidth;
		/* Item is not visible. */
        if((iSelection < pstObj->iPageStartIndex) || (iSelection > pstObj->iPageEndIndex))
		{
			pstItemExtent->iY = 0;
			pstItemExtent->iHeight = 0;
		}
		else
		{
			if((0 == pstObj->iItemPaintOffset) && (iSelection == pstObj->iPageEndIndex))
			{
				pstItemExtent->iY = pstObj->stLayout.iY+(ITEM_HEIGHT(pstObj->pstFontRes)*(pstObj->iVisibleItems-1));
				pstItemExtent->iHeight = (0==pstObj->iItemPaintOffset)?(pstObj->stLayout.iHeight%ITEM_HEIGHT(pstObj->pstFontRes)):(ITEM_HEIGHT(pstObj->pstFontRes));
				/* Correct last visible item height when items area height is an integer multiple of item height. */
				pstItemExtent->iHeight = (0==pstItemExtent->iHeight)?ITEM_HEIGHT(pstObj->pstFontRes):pstItemExtent->iHeight;
			}
			else if((0 != pstObj->iItemPaintOffset) && (iSelection == pstObj->iPageStartIndex))
			{
				pstItemExtent->iY = pstObj->stLayout.iY;
				pstItemExtent->iHeight = ITEM_HEIGHT(pstObj->pstFontRes)+pstObj->iItemPaintOffset;
			}
			else
			{
				pstItemExtent->iY = pstObj->stLayout.iY+((iSelection - pstObj->iPageStartIndex)*ITEM_HEIGHT(pstObj->pstFontRes))+pstObj->iItemPaintOffset;
				pstItemExtent->iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_ItemsBase_FitLayout							**/
/** Purpose:		Recalculate layout data when display area size is 	**/
/**					changed.											**/
/** Params:																**/
/** @ pstObj[in]:	Pointer of items-base object will be paint.			**/
/** Return:			None.												**/
/** Notice:			This function only used for the visible item, if	**/
/**					selection is invisible, output size will be (0, 0).	**/
/*************************************************************************/
void SGUI_ItemsBase_FitLayout(SGUI_ITEMS_BASE* pstObj)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstObj)
	{
		pstObj->iVisibleItems = (pstObj->stLayout.iHeight-1)/ITEM_HEIGHT(pstObj->pstFontRes)+1;
		pstObj->iPageEndIndex = pstObj->iPageStartIndex + pstObj->iVisibleItems - 1;
	}
}
