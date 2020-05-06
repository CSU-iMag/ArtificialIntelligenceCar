#ifndef __INCLUDE_GUI_COMMON_H__
#define __INCLUDE_GUI_COMMON_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Config.h"
#include "SGUI_Typedef.h"
#include "SGUI_Interface.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		NOTICE_ICON_SIZE				(16)
#define		SGUI_NUMBER_STR_LENGTH_MAX		(12)
#define		SGUI_ISDIGIT(C)					(((C>='0')&&(C<='9'))?true:false)
#define		SGUI_ISALPHA(C)					((((C>='A')&&(C<='Z'))||((C>='a')&&(C<='z')))?true:false)
#define		SGUI_ISHEXDIGIT(C)				((((C>='A')&&(C<='F'))||((C>='a')&&(C<='f'))||((C>='0')&&(C<='9')))?true:false)
#define		SGUI_ISUPPER(C)					(((C>='A')&&(C<='Z'))?true:false)
#define		SGUI_TOUPPER(C)					(SGUI_ISUPPER(C)?(C):(C-32))
#define		SGUI_SWAP(A, B)					{A=A^B; B=A^B; A=A^B;}

#define		LAYOUT(POBJ)					((POBJ)->stParam.stLayout)

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void					SGUI_Common_AdaptDisplayInfo(SGUI_RECT* pstDisplayArea, SGUI_POINT* pstInnerPos);
SGUI_SIZE               SGUI_Common_IntegerToStringWithDecimalPoint(SGUI_INT iInteger, SGUI_UINT uiDecimalPlaces, SGUI_SZSTR pszStringBuffer, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter);
SGUI_SIZE               SGUI_Common_IntegerToString(SGUI_INT iInteger, SGUI_SZSTR pszStringBuffer, SGUI_UINT uiBase, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter);
SGUI_UINT               SGUI_Common_ConvertStringToUnsignedInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase);
SGUI_INT                SGUI_Common_ConvertStringToInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase);

#endif // __INCLUDE_GUI_COMMON_H__
