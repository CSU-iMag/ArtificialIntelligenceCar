/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_RealtimeGraph.c										**/
/** Author: Polarix														**/
/** Description: Graph adjustment UI interface.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_RealtimeGraph.h"

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_Initialize						**/
/** Purpose:		Initialize a graph control data.					**/
/** Params:																**/
/**	@ pstRTGraph[in]:	Graph map data.									**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_Initialize(SGUI_RTGRAPH* pstRTGraph)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;

		// yAxisMax must be greater then yAxisMin;
		if(pstControl->yAxisMax < pstControl->yAxisMin)
		{
			SGUI_SWAP(pstControl->yAxisMax, pstControl->yAxisMin);
		}
		// X-axis step in pixel must be greater then 2.
		if(pstControl->xAxisStepPixel < 2)
		{
			pstControl->xAxisStepPixel = 2;
		}

		pstControl->ValueArea = pstControl->yAxisMax - pstControl->yAxisMin+1;

		// Zero point value must NOT greater then yAxisMax and NOT less then yAxisMin.
		if(pstData->BaseLineValue > pstControl->yAxisMax)
		{
			pstData->BaseLineValue = pstControl->yAxisMax;
		}
		if(pstData->BaseLineValue < pstControl->yAxisMin)
		{
			pstData->BaseLineValue = pstControl->yAxisMin;
		}
		// Calculate the number of value points that can be used.
		pstData->ValueCount = (RECT_WIDTH(pstRTGraph->stLayout)-2)/pstControl->xAxisStepPixel;

		// Initialize value array.
		for(iValueIndex=0; iValueIndex<pstData->ValueCount; iValueIndex++)
		{
			pstData->PointYCoordinateArray[iValueIndex] = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstData->BaseLineValue);;
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Graph_Refresh									**/
/** Purpose:		Refresh graph map display.							**/
/** Resources:		Graph data.											**/
/** Params:																**/
/**	@ pstDeviceIF[in]:		SimpleGUI object pointer.						**/
/**	@ pstRTGraph[in]:	Graph map data used refreshed.					**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_RTGRAPH* pstRTGraph)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;
	SGUI_INT					iPixelCoordinateStartX, iPixelCoordinateEndX;
	SGUI_INT					iBaseLineCoordinateY;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstRTGraph) && (NULL != pstDeviceIF))
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;

		// Draw frame
		SGUI_Basic_DrawRectangle(pstDeviceIF, RECT_X_START(pstRTGraph->stLayout), RECT_Y_START(pstRTGraph->stLayout),
									RECT_WIDTH(pstRTGraph->stLayout), RECT_HEIGHT(pstRTGraph->stLayout), SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);

		if(SGUI_TRUE == pstControl->EnableBaseline)
		{
			iBaseLineCoordinateY = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstData->BaseLineValue);
			SGUI_Basic_DrawLine(pstDeviceIF, 1, iBaseLineCoordinateY, RECT_WIDTH(pstDeviceIF->stSize)-2, iBaseLineCoordinateY, SGUI_COLOR_FRGCLR);
		}

		if(pstData->ValueCount > 1)
		{
			for(iValueIndex=1; iValueIndex<pstData->ValueCount; iValueIndex++)
			{
				iPixelCoordinateStartX = 1 + ((iValueIndex-1) * pstControl->xAxisStepPixel) + RECT_X_START(pstRTGraph->stLayout);
				iPixelCoordinateEndX = 1 + (iValueIndex * pstControl->xAxisStepPixel) + RECT_X_START(pstRTGraph->stLayout);
				SGUI_Basic_DrawLine(pstDeviceIF, iPixelCoordinateStartX, pstData->PointYCoordinateArray[iValueIndex-1],
										iPixelCoordinateEndX, pstData->PointYCoordinateArray[iValueIndex], SGUI_COLOR_FRGCLR);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_AppendValue						**/
/** Purpose:		Append a new value to graph.						**/
/** Params:																**/
/**	@ pstDeviceIF[in]:		SimpleGUI object pointer.						**/
/**	@ pstRTGraph[in]:	Real-time graph object pointer.					**/
/**	@ iNewValue[in]:	New value will be appended.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_AppendValue(SGUI_SCR_DEV* pstDeviceIF, SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;
	SGUI_INT					iLimitedValue;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;

		// Push value.
		for(iValueIndex=0; iValueIndex<pstData->ValueCount-1; iValueIndex++)
		{
			pstData->ValueArray[iValueIndex] = pstData->ValueArray[iValueIndex+1];
			pstData->LimitedValueArray[iValueIndex] = pstData->LimitedValueArray[iValueIndex+1];
			pstData->PointYCoordinateArray[iValueIndex] = pstData->PointYCoordinateArray[iValueIndex+1];
		}
		// Append new value.
		iValueIndex = pstData->ValueCount-1;
		pstData->ValueArray[iValueIndex] = iNewValue;
		iLimitedValue = iNewValue;
		if(iLimitedValue > pstControl->yAxisMax)
		{
			iLimitedValue = pstControl->yAxisMax;
		}
		if(iLimitedValue < pstControl->yAxisMin)
		{
			iLimitedValue = pstControl->yAxisMin;
		}
		pstData->LimitedValueArray[iValueIndex] = iLimitedValue;
		pstData->PointYCoordinateArray[iValueIndex] = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, iLimitedValue);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_GetValuePointYCoordinate			**/
/** Purpose:		Convert data point to a drawing point in visible 	**/
/**					graph area.											**/
/** Params:																**/
/**	@ pstRTGraph[in]:	Real-time graph object pointer.					**/
/**	@ iValue[in]:		Real value.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iDisplayValuePointAreaHeight;
	SGUI_INT					iAbsoluteValue;
	SGUI_INT					iValuePointCoordinate;
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iValuePointCoordinate =     -1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstControl = pstRTGraph->Control;

		iDisplayValuePointAreaHeight = RECT_HEIGHT(pstRTGraph->stLayout);

		//Make sure the value is within the valid range
		if(iValue > pstControl->yAxisMax)
		{
			iValue = pstControl->yAxisMax;
		}
		if(iValue < pstControl->yAxisMin)
		{
			iValue = pstControl->yAxisMin;
		}
		iAbsoluteValue = iValue - pstControl->yAxisMin;

		iValuePointCoordinate = iAbsoluteValue*iDisplayValuePointAreaHeight/pstControl->ValueArea;

		iValuePointCoordinate = iDisplayValuePointAreaHeight-iValuePointCoordinate;

		iValuePointCoordinate = iValuePointCoordinate + RECT_Y_START(pstRTGraph->stLayout)-1;
	}

	return iValuePointCoordinate;
}
