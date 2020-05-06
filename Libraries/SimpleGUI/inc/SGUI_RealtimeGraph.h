#ifndef __INCLUDE_GUI_REAL_GRAPH_H__
#define __INCLUDE_GUI_REAL_GRAPH_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		SGUI_RTGRAPH_VALUE_ARRAY_SIZE		(128)

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_INT				yAxisMax; // Max value on screen display.
	SGUI_INT				yAxisMin; // Min value on screen display.
	SGUI_BOOL				EnableBaseline; // Enable to show zero-point line.
	SGUI_INT				xAxisStepPixel;
	SGUI_INT				ValueArea; // Display value range, auto calculate when initialize.
}SGUI_RTGRAPH_CONTROL;

typedef struct
{

	SGUI_INT				ValueArray[SGUI_RTGRAPH_VALUE_ARRAY_SIZE];
	SGUI_INT				LimitedValueArray[SGUI_RTGRAPH_VALUE_ARRAY_SIZE];
	SGUI_INT				PointYCoordinateArray[SGUI_RTGRAPH_VALUE_ARRAY_SIZE];
	SGUI_INT				BaseLineValue; // Base line value on graph map.
	SGUI_INT				ValueCount; // Display value point number, auto calculate when initialize.
}SGUI_RTGRAPH_DATA;

typedef struct
{
	SGUI_RECT				stLayout;
	SGUI_RTGRAPH_DATA*		Data;
	SGUI_RTGRAPH_CONTROL*	Control;
}SGUI_RTGRAPH;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_RealtimeGraph_Initialize(SGUI_RTGRAPH* pstRTGraph);
void			SGUI_RealtimeGraph_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_RTGRAPH* pstRTGraph);
void			SGUI_RealtimeGraph_AppendValue(SGUI_SCR_DEV* pstDeviceIF, SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue);
#endif // __INCLUDE_GUI_REAL_GRAPH_H__
