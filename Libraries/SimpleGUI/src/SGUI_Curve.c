/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Curve.c												**/
/** Author: Polarix														**/
/** Description: Draw frames on screen.									**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Curve.h"

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//


//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Curve_Initialize								**/
/** Purpose:		Initialize curve data structure.					**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pcstInitParam[in]: Initialize data pointer.						**/
/** Return:			None.												**/
/** Notice:			none.												**/
/*************************************************************************/
void SGUI_Curve_Initialize(SGUI_CURVE_STRUCT* pstObj, const SGUI_CURVE_PARAM* pcstInitParam)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pcstInitParam))
	{
		SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_CURVE_PARAM));
		SGUI_SystemIF_MemorySet(&(pstObj->stData), 0x00, sizeof(SGUI_CURVE_DATA));
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_Repaint									**/
/** Purpose:		Draw a curve on screen order by point data.			**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_Curve_Repaint(SGUI_SCR_DEV* pstDeviceIF, const SGUI_CURVE_STRUCT* pstObj)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CURVE_POINT*		pcstLineStartPoint;
	SGUI_CURVE_POINT*		pcstLineEndPoint;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstDeviceIF) && (NULL != pstObj))
	{
		/* Clean up background. */
		SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX, pstObj->stParam.stLayout.iY, pstObj->stParam.stLayout.iWidth, pstObj->stParam.stLayout.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
		if(pstObj->stData.stPoints.sCount > 1)
		{
#if 1
			pcstLineStartPoint = pstObj->stData.stPoints.pstHead;
			pcstLineEndPoint = pcstLineStartPoint->pstNext;
			while(NULL != pcstLineEndPoint)
			{
				/* Paint visible line. */
				if(SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineStartPoint->stPosition)) || SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineEndPoint->stPosition)))
				{
					SGUI_Basic_DrawLine(pstDeviceIF, pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY,
											pcstLineEndPoint->stPosition.iX, pcstLineEndPoint->stPosition.iY, SGUI_COLOR_FRGCLR);
				}
				pcstLineStartPoint = pcstLineStartPoint->pstNext;
				pcstLineEndPoint = pcstLineStartPoint->pstNext;
			}
#else
			pcstLineStartPoint = pstObj->stData.stPoints.pstEnd;
			pcstLineEndPoint = pcstLineStartPoint->pstPrev;
			while(NULL != pcstLineEndPoint)
			{
				/* Paint visible line. */
				if(SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineStartPoint->stPosition)) || SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineEndPoint->stPosition)))
				{
					SGUI_Basic_DrawLine(pstDeviceIF, pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY,
											pcstLineEndPoint->stPosition.iX, pcstLineEndPoint->stPosition.iY, SGUI_COLOR_FRGCLR);
				}
				pcstLineStartPoint = pcstLineStartPoint->pstPrev;
				pcstLineEndPoint = pcstLineStartPoint->pstPrev;
			}
#endif
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_InitializePoint							**/
/** Purpose:		Set X and Y value(NOT position) to a point.			**/
/** Params:																**/
/**	@ pstNewPoint[in]: Point data pointer.								**/
/**	@ iX[in]:		Point X value.										**/
/**	@ iY[in]:		Point Y value.										**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_Curve_InitializePoint(SGUI_CURVE_POINT* pstNewPoint, SGUI_INT iX, SGUI_INT iY)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstNewPoint)
	{
		SGUI_SystemIF_MemorySet(pstNewPoint, 0x00, sizeof(SGUI_CURVE_POINT));
		pstNewPoint->stPoint.iX = iX;
		pstNewPoint->stPoint.iY = iY;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_AddPoint									**/
/** Purpose:		Add a new point object to end of curve point chain	**/
/**					list.												**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pstNewPoint[in]: Point data will be appended.						**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_Curve_AddPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstNewPoint))
	{
		if(0 == pstObj->stData.stPoints.sCount)
		{
			pstObj->stData.stPoints.pstHead = pstNewPoint;
			pstObj->stData.stPoints.pstEnd = pstNewPoint;
		}
		else
		{
			pstObj->stData.stPoints.pstEnd->pstNext = pstNewPoint;
			pstNewPoint->pstPrev = pstObj->stData.stPoints.pstEnd;
			pstObj->stData.stPoints.pstEnd = pstNewPoint;
		}
		SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
		pstObj->stData.stPoints.sCount++;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_InsertBeforePoint						**/
/** Purpose:		Add a new point before current focused point in		**/
/**					chain.												**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pstNewPoint[in]: Point data will be inserted.						**/
/** Return:																**/
/** @ SGUI_TRUE		Insert success.										**/
/** @ SGUI_FALSE:	Insert failed, might be focused point is head or	**/
/**					no focused point.									**/
/** Notice:			Cannot insert before the first point in chain.		**/
/*************************************************************************/
SGUI_BOOL SGUI_Curve_InsertBeforePoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CURVE_POINT*		pstBasePoint;
	SGUI_BOOL				bReturn;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL == pstObj) || (NULL == pstNewPoint))
	{
		/* Invalid parameter. */
		bReturn = SGUI_FALSE;
	}
	else
	{
		pstBasePoint = pstObj->stData.pstFocused;
		do
		{
			if(NULL == pstBasePoint)
			{
				/* Current focused point must be set. */
				break;
			}
            if(pstBasePoint == pstObj->stData.stPoints.pstHead)
			{
				/* Cannot insert before head. */
				bReturn = SGUI_FALSE;
				break;
			}
			/* Judge new point X value. */
			if(pstNewPoint->stPoint.iX > pstBasePoint->stPoint.iX)
			{
				pstNewPoint->stPoint.iX = pstBasePoint->stPoint.iX;
			}
			if(pstNewPoint->stPoint.iX < pstBasePoint->pstPrev->stPoint.iX)
			{
				pstNewPoint->stPoint.iX = pstBasePoint->pstNext->stPoint.iX;
			}

			pstBasePoint->pstPrev->pstNext = pstNewPoint;
			pstNewPoint->pstPrev = pstBasePoint->pstPrev;
			pstNewPoint->pstNext = pstBasePoint;
			pstBasePoint->pstPrev = pstNewPoint;
			SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
			pstObj->stData.stPoints.sCount++;
			bReturn = SGUI_TRUE;
		}while(0);
	}
	return bReturn;
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_InsertAfterPoint							**/
/** Purpose:		Add a new point after current focused point in		**/
/**					chain.												**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pstNewPoint[in]: Point data will be inserted.						**/
/** Return:																**/
/** @ SGUI_TRUE		Insert success.										**/
/** @ SGUI_FALSE:	Insert failed, might be focused point is end or no	**/
/**					focused point.										**/
/** Notice:			Cannot insert after the last point in chain.		**/
/*************************************************************************/
SGUI_BOOL SGUI_Curve_InsertAfterPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CURVE_POINT*		pstBasePoint;
	SGUI_BOOL				bReturn;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL == pstObj) || (NULL == pstNewPoint))
	{
		/* Invalid parameter. */
		bReturn = SGUI_FALSE;
	}
	else
	{
		pstBasePoint = pstObj->stData.pstFocused;
		do
		{
			if(NULL == pstBasePoint)
			{
				/* Current focused point must be set. */
				break;
			}
            if(pstBasePoint == pstObj->stData.stPoints.pstEnd)
			{
				/* Cannot insert after end. */
				bReturn = SGUI_FALSE;
				break;
			}
			/* Judge new point X value. */
			if(pstNewPoint->stPoint.iX < pstBasePoint->stPoint.iX)
			{
				pstNewPoint->stPoint.iX = pstBasePoint->stPoint.iX;
			}
			if(pstNewPoint->stPoint.iX > pstBasePoint->pstNext->stPoint.iX)
			{
				pstNewPoint->stPoint.iX = pstBasePoint->pstNext->stPoint.iX;
			}
			pstBasePoint->pstNext->pstPrev = pstNewPoint;
			pstNewPoint->pstNext = pstBasePoint->pstNext;
			pstNewPoint->pstPrev = pstBasePoint;
			pstBasePoint->pstNext = pstNewPoint;
			SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
			pstObj->stData.stPoints.sCount++;
			bReturn = SGUI_TRUE;
		}while(0);
	}
	return bReturn;
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_RemovePoint								**/
/** Purpose:		Remove the point object from the curve chain.		**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pstPoint[in]: Point data will be removed.							**/
/** Return:																**/
/** @ SGUI_TRUE		Point removed from curve point list.				**/
/** @ SGUI_FALSE:	Remove failed, might be disable to remove or point	**/
/**					is not in curve point list.							**/
/** Notice:			This function only removed point form curve chain,	**/
/**					memory must release if allocate from heap.			**/
/*************************************************************************/
SGUI_BOOL SGUI_Curve_RemovePoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstPoint)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOOL				bReturn;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstPoint) && (pstObj->stData.stPoints.sCount > 2))
	{
		if(NULL != pstPoint->pstNext)
		{
			pstPoint->pstNext->pstPrev =  pstPoint->pstPrev;
		}
		if(NULL != pstPoint->pstPrev)
		{
			pstPoint->pstPrev->pstNext = pstPoint->pstNext;
		}
		if(pstPoint == pstObj->stData.pstFocused)
		{
			pstObj->stData.pstFocused = NULL;
		}
		pstObj->stData.stPoints.sCount--;
		bReturn = SGUI_TRUE;
	}
	else
	{
		bReturn = SGUI_FALSE;
	}

	return bReturn;
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_CalculatePointPosition					**/
/** Purpose:		Calculate point position according to point X and Y	**/
/**					value and curve display area layout.				**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ pstPoint[in]: Point data will be removed.							**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_Curve_CalculatePointPosition(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstPoint)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstObj) && (NULL != pstPoint))
	{
		pstPoint->stPosition.iX = (RECT_WIDTH(pstObj->stParam.stLayout)-1)*(pstPoint->stPoint.iX-pstObj->stParam.stXRange.iMin)/(RANGE_SIZE(pstObj->stParam.stXRange)-1);
		pstPoint->stPosition.iY = (RECT_HEIGHT(pstObj->stParam.stLayout)-1)*(pstPoint->stPoint.iY-pstObj->stParam.stYRange.iMin)/(RANGE_SIZE(pstObj->stParam.stYRange)-1);
		pstPoint->stPosition.iX += pstObj->stParam.stLayout.iX;
		pstPoint->stPosition.iY = (RECT_HEIGHT(pstObj->stParam.stLayout)-1)-pstPoint->stPosition.iY;
		pstPoint->stPosition.iY += pstObj->stParam.stLayout.iY;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_FocusPoint								**/
/** Purpose:		Set focus on a point according to given index start	**/
/**					at 0.												**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ sIndex[in]:	Index start at 0 form the head point.				**/
/** Return:			Focused point object pointer. NULL pointer will be	**/
/**					returned when index is invalid.						**/
/*************************************************************************/
SGUI_CURVE_POINT* SGUI_Curve_FocusPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_INDEX sIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CURVE_POINT*		pstFocusedPoint;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstFocusedPoint =		NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstObj)
	{
		pstFocusedPoint = pstObj->stData.stPoints.pstHead;
		while((sIndex>0) && (NULL != pstFocusedPoint))
		{
			pstFocusedPoint = pstFocusedPoint->pstNext;
			sIndex--;
		}

		pstObj->stData.pstFocused = pstFocusedPoint;
	}
	return pstFocusedPoint;
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_UpdateFocusPoint							**/
/** Purpose:		Update focused point value.							**/
/** Params:																**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/**	@ iX[in]:		New X value for focused point.						**/
/**	@ iY[in]:		New Y value for focused point.						**/
/** Return:																**/
/** @ SGUI_TRUE		New value set successfully.							**/
/** @ SGUI_FALSE:	Curve object is invalid or no focused point.		**/
/*************************************************************************/
SGUI_BOOL SGUI_Curve_UpdateFocusPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_INT iX, SGUI_INT iY)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOOL				bReturn;
	SGUI_CURVE_POINT*		pstFocusedPoint;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =				SGUI_FALSE;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL == pstObj)
	{
		/* Invalid parameter. */
		bReturn = SGUI_FALSE;
	}
	else
	{
		do
		{
            if(NULL == pstObj->stData.pstFocused)
			{
				bReturn = SGUI_FALSE;
				break;
			}
			pstFocusedPoint = pstObj->stData.pstFocused;
			/* Check point X value. */
			if((pstObj->stData.pstFocused == pstObj->stData.stPoints.pstHead) || (pstObj->stData.pstFocused == pstObj->stData.stPoints.pstEnd))
			{
				/* Cannot change X value when focused point is head or end. */
				iX = pstObj->stData.pstFocused->stPoint.iX;
			}
			else
			{
				if(NULL == pstObj->stData.pstFocused->pstPrev)
				{
					iX = SGUI_MAX_OF(pstObj->stParam.stXRange.iMin, iX);
				}
				else
				{
					iX = SGUI_MAX_OF(pstFocusedPoint->pstPrev->stPoint.iX, iX);
				}
				if(NULL == pstObj->stData.pstFocused->pstNext)
				{
					iX = SGUI_MIN_OF(pstObj->stParam.stXRange.iMax, iX);
				}
				else
				{
					iX = SGUI_MIN_OF(pstFocusedPoint->pstNext->stPoint.iX, iX);
				}
			}
			/* Check point Y value. */
			iY = SGUI_MAX_OF(pstObj->stParam.stYRange.iMin, iY);
			iY = SGUI_MIN_OF(pstObj->stParam.stYRange.iMax, iY);

			pstFocusedPoint->stPoint.iX = iX;
			pstFocusedPoint->stPoint.iY = iY;
			/* Recalculate point position. */
			SGUI_Curve_CalculatePointPosition(pstObj, pstFocusedPoint);
			bReturn = SGUI_TRUE;
		}while(0);
	}
	return bReturn;
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_HighlightFocus							**/
/** Purpose:		Paint a highlight cursor for focused point.			**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstObj[in]:	Curve object pointer.								**/
/** Return:			None.												**/
/*************************************************************************/
void SGUI_Curve_HighlightFocus(SGUI_SCR_DEV* pstDeviceIF, SGUI_CURVE_STRUCT* pstObj)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT				stHighlightArea;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstDeviceIF) && (NULL != pstObj))
	{
		if(NULL != pstObj->stData.pstFocused)
		{
			stHighlightArea.iX = pstObj->stData.pstFocused->stPosition.iX-2;
			stHighlightArea.iY = pstObj->stData.pstFocused->stPosition.iY-2;
			stHighlightArea.iWidth = 5;
			stHighlightArea.iHeight = 5;

			if(RECT_X_START(stHighlightArea) <= RECT_X_START(pstObj->stParam.stLayout))
			{
				stHighlightArea.iWidth -= (RECT_X_START(pstObj->stParam.stLayout)-stHighlightArea.iX);
				stHighlightArea.iX = RECT_X_START(pstObj->stParam.stLayout);
			}

			if(RECT_X_END(stHighlightArea) >= RECT_X_END(pstObj->stParam.stLayout))
			{
				stHighlightArea.iWidth -= (RECT_X_END(stHighlightArea)-RECT_X_END(pstObj->stParam.stLayout));
			}

			if(RECT_Y_START(stHighlightArea) <= RECT_Y_START(pstObj->stParam.stLayout))
			{
				stHighlightArea.iHeight -= (RECT_Y_START(pstObj->stParam.stLayout)-stHighlightArea.iY);
				stHighlightArea.iY = RECT_Y_START(pstObj->stParam.stLayout);
			}

			if(RECT_Y_END(stHighlightArea) >= RECT_Y_END(pstObj->stParam.stLayout))
			{
				stHighlightArea.iHeight -= (RECT_Y_END(stHighlightArea)-RECT_Y_END(pstObj->stParam.stLayout));
			}

			SGUI_Basic_ReverseBlockColor(pstDeviceIF, stHighlightArea.iX, stHighlightArea.iY, stHighlightArea.iWidth, stHighlightArea.iHeight);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Curve_PointIsHighlight							**/
/** Purpose:		Judge curve point is in highlight state.			**/
/** Params:																**/
/**	@ pstDeviceIF[in]: Device driver object pointer.					**/
/**	@ pstPoint[in]:	Point object will be judged.						**/
/** Return:																**/
/** @ SGUI_TRUE		Point is highlight.									**/
/** @ SGUI_FALSE:	Point is not highlight.								**/
/*************************************************************************/
SGUI_BOOL SGUI_Curve_PointIsHighlight(SGUI_SCR_DEV* pstDeviceIF, SGUI_CURVE_POINT* pstPoint)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOOL				bReturn;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstDeviceIF) && (NULL != pstPoint))
	{
		bReturn = (SGUI_COLOR_BKGCLR==SGUI_Basic_GetPoint(pstDeviceIF, pstPoint->stPosition.iX, pstPoint->stPosition.iY))?SGUI_TRUE:SGUI_FALSE;
	}
	else
	{
		bReturn = SGUI_FALSE;
	}

	return bReturn;
}
