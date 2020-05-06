#ifndef __INCLUDE_SGUI_GRAPH_H__
#define __INCLUDE_SGUI_GRAPH_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_ScrollBar.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_INT				x;
	SGUI_INT				y;
}SGUI_GRAPH_POINT;

typedef struct
{
	SGUI_INT				Min;
	SGUI_INT				Max;
}SGUI_GRAPH_AXIS;

typedef struct
{
	SGUI_GRAPH_POINT*		Points;
	SGUI_SIZE				Count;
}SGUI_GRAPH_DATA;

typedef struct
{
	SGUI_GRAPH_AXIS			xAxis;
	SGUI_GRAPH_AXIS			yAxis;
	SGUI_SIZE				FocusIndex;
	SGUI_INT				PointRangeX;
	SGUI_INT				PointRangeY;
	SGUI_GRAPH_POINT		LastDrawingPoint;
}SGUI_GRAPH_CONTROL;

typedef struct
{
	SGUI_SCROLLBAR_STRUCT	xScrollBar;
	SGUI_SCROLLBAR_STRUCT	yScrollBar;
}SGUI_GRAPH_SUBELEMENT;

typedef struct
{
	SGUI_GRAPH_DATA*		Data;
	SGUI_GRAPH_CONTROL*		Control;
	SGUI_GRAPH_SUBELEMENT	SubElement;
}SGUI_GRAPH;

typedef struct
{
	SGUI_SIZE				Count;
	SGUI_SIZE				FocusIndex;
	SGUI_GRAPH_POINT*		Points;
}SGUI_GRAPH_INIT_DATA;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_Graph_InitializeGraphData(SGUI_SCR_DEV* pstIFObj, SGUI_GRAPH* pstGraph, SGUI_GRAPH_INIT_DATA* pstInitializeData);
void			SGUI_Graph_Refresh(SGUI_SCR_DEV* pstIFObj, SGUI_GRAPH* pstGraph);
#endif // __INCLUDE_SGUI_GRAPH_H__
