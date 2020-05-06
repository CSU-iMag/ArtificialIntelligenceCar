/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_ProcessBar.c											**/
/** Author: XuYulin														**/
/** Description: Process bar display interface.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_ProcessBar.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_RefreshProcessBar								**/
/** Purpose:		Display or update a process bar.					**/
/** Resources:		Process bar data structure.							**/
/** Params:																**/
/**	@ pstDeviceIF[in]:	SimpleGUI object pointer.							**/
/**	@ pstProcessBarData[in]: Process bar data whitch will update on		**/
/**					screen.												**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_ProcessBar_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_PROCBAR_STRUCT *pstProcessBarData)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT16					uiProcessBlockStartX, uiProcessBlockStartY;
	SGUI_UINT16					uiProcessBlockWidth, uiProcessBlockHeight;
	SGUI_COLOR					eBackColor, eFillColor;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstProcessBarData)
	{
		if((pstProcessBarData->Parameter.Width > 3) && (pstProcessBarData->Parameter.Height > 3))
		{
			// Check value must be less then PROCBAR_VALUE_MAX.
			if(pstProcessBarData->Data.Value > pstProcessBarData->Parameter.MaxValue)
			{
				pstProcessBarData->Data.Value = pstProcessBarData->Parameter.MaxValue;
			}
			// Update process bar data.
			switch(pstProcessBarData->Parameter.Direction)
			{
				case SGUI_PROCBAR_DOWN:	// Process from up to down.
				{
					uiProcessBlockStartX	= pstProcessBarData->Parameter.PosX + 1;
					uiProcessBlockStartY	= pstProcessBarData->Parameter.PosY + 1;
					uiProcessBlockWidth		= pstProcessBarData->Parameter.Width - 2;
					uiProcessBlockHeight	= (pstProcessBarData->Parameter.Height - 2) * (pstProcessBarData->Data.Value) / pstProcessBarData->Parameter.MaxValue;
					eBackColor = SGUI_COLOR_BKGCLR;
					eFillColor = SGUI_COLOR_FRGCLR;
					break;
				}
				case SGUI_PROCBAR_LEFT:	// Process from right to left.
				{
					uiProcessBlockStartX	= pstProcessBarData->Parameter.PosX + 1;
					uiProcessBlockStartY	= pstProcessBarData->Parameter.PosY + 1;
					uiProcessBlockWidth		= (pstProcessBarData->Parameter.Width - 2) * (pstProcessBarData->Parameter.MaxValue - pstProcessBarData->Data.Value) / pstProcessBarData->Parameter.MaxValue;
					uiProcessBlockHeight	= pstProcessBarData->Parameter.Height - 2;
					eBackColor = SGUI_COLOR_FRGCLR;
					eFillColor = SGUI_COLOR_BKGCLR;
					break;
				}
				case SGUI_PROCBAR_RIGHT:	// Process from left to right.
				{
					uiProcessBlockStartX	= pstProcessBarData->Parameter.PosX + 1;
					uiProcessBlockStartY	= pstProcessBarData->Parameter.PosY + 1;
					uiProcessBlockWidth		= (pstProcessBarData->Parameter.Width - 2) * (pstProcessBarData->Data.Value) / pstProcessBarData->Parameter.MaxValue;
					uiProcessBlockHeight	= pstProcessBarData->Parameter.Height - 2;
					eBackColor = SGUI_COLOR_BKGCLR;
					eFillColor = SGUI_COLOR_FRGCLR;
					break;
				}
				case SGUI_PROCBAR_UP:	// Process from down to up.
				default:
				{
					uiProcessBlockStartX	= pstProcessBarData->Parameter.PosX + 1;
					uiProcessBlockStartY	= pstProcessBarData->Parameter.PosY + 1;
					uiProcessBlockWidth		= pstProcessBarData->Parameter.Width - 2;
					uiProcessBlockHeight	= (pstProcessBarData->Parameter.Height - 2) * (pstProcessBarData->Parameter.MaxValue - pstProcessBarData->Data.Value) / pstProcessBarData->Parameter.MaxValue;
					eBackColor = SGUI_COLOR_FRGCLR;
					eFillColor = SGUI_COLOR_BKGCLR;
				}
			}
			// Redraw edge and clean up area.
			SGUI_Basic_DrawRectangle(pstDeviceIF, pstProcessBarData->Parameter.PosX, pstProcessBarData->Parameter.PosY, pstProcessBarData->Parameter.Width, pstProcessBarData->Parameter.Height, SGUI_COLOR_FRGCLR, eBackColor);
			// Draw process block.
			SGUI_Basic_DrawRectangle(pstDeviceIF, uiProcessBlockStartX, uiProcessBlockStartY, uiProcessBlockWidth, uiProcessBlockHeight, eFillColor, eFillColor);
		}
	}
}
