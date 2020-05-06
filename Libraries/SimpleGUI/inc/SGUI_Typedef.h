#ifndef _INCLUDE_GUI_TYPEDEF_H_
#define _INCLUDE_GUI_TYPEDEF_H_
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		SGUI_MAX_OF(A, B)						(A>B?A:B)
#define		SGUI_MIN_OF(A, B)						(A<B?A:B)
#define		RANGE_SIZE(RANGE)						((RANGE).iMax-(RANGE).iMin+1)
#define 	RECT_X_START(ST)						((ST).iX)
#define 	RECT_X_END(RECT)						(((RECT).iX + (RECT).iWidth - 1))
#define 	RECT_Y_START(ST)						((ST).iY)
#define 	RECT_Y_END(RECT)						(((RECT).iY + (RECT).iHeight - 1))
#define 	RECT_WIDTH(ST)							((ST).iWidth)
#define 	RECT_HEIGHT(ST)							((ST).iHeight)
#define 	RECT_VALID_WIDTH(DATA, POS)				((RECT_X_START(POS)>0)?RECT_WIDTH(DATA):(RECT_WIDTH(DATA)+RECT_X_START(POS)))
#define		RECT_VALID_HEIGHT(DATA, POS)			((RECT_Y_START(POS)>0)?RECT_HEIGHT(DATA):(RECT_HEIGHT(DATA)+RECT_Y_START(POS)))

#define		SGUI_DEVPF_IF_DEFINE(R, FN, PARAM)		typedef R(*FN)PARAM
#define		SGUI_BMP_DATA_BUFFER_SIZE				(512)

#define	SGUI_BMP_RESOURCE_DECLARE(NAME)				extern const SGUI_BMP_RES NAME
#define	SGUI_BMP_RESOURCE_DEFINE(NAME, W, H, ...)	const SGUI_BYTE NAME##DATA[] = {__VA_ARGS__};\
													const SGUI_BMP_RES NAME = {W, H, NAME##DATA}
//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef char							SGUI_INT8;
typedef	unsigned char					SGUI_UINT8;
typedef int16_t							SGUI_INT16;
typedef uint16_t						SGUI_UINT16;
typedef int32_t							SGUI_INT32;
typedef	uint32_t						SGUI_UINT32;

typedef	int								SGUI_INT;
typedef unsigned int					SGUI_UINT;
typedef long							SGUI_LONG;
typedef unsigned long					SGUI_ULONG;
typedef unsigned char					SGUI_BYTE;
typedef const unsigned char				SGUI_CBYTE;
typedef size_t							SGUI_SIZE;

typedef char*							SGUI_SZSTR;
typedef const char*						SGUI_CSZSTR;
typedef char							SGUI_CHAR;
typedef const char						SGUI_CCHAR;

typedef void*							SGUI_PTR;

typedef SGUI_SIZE						SGUI_INDEX;
#define	SGUI_INVALID_INDEX				(-1)
typedef	SGUI_UINT32						SGUI_ROM_ADDRESS;

#define	SGUI_BOOL						SGUI_INT
#define SGUI_FALSE						(0)
#define SGUI_TRUE						(!SGUI_FALSE)

typedef struct
{
	SGUI_INT							iX;
	SGUI_INT							iY;
	SGUI_INT							iWidth;
	SGUI_INT							iHeight;
}SGUI_RECT;

typedef struct
{
	SGUI_INT							iWidth;
	SGUI_INT							iHeight;
}SGUI_AREA_SIZE;

typedef struct
{
	SGUI_INT							iX;
	SGUI_INT							iY;
}SGUI_POINT;

typedef struct
{
	SGUI_INT							iMin;
	SGUI_INT							iMax;
}SGUI_RANGE;

typedef enum
{
	SGUI_COLOR_BKGCLR =					0,
	SGUI_COLOR_FRGCLR =					1,
	SGUI_COLOR_TRANS =					2,
}SGUI_COLOR;

typedef enum
{
	SGUI_DRAW_NORMAL =					0,
	SGUI_DRAW_REVERSE =					1,
}SGUI_DRAW_MODE;

typedef enum
{
	SGUI_RIGHT =						0,
	SGUI_CENTER,
	SGUI_LEFT,
}SGUI_ALIG_MODE;

// Screen device operation interface type declare.
SGUI_DEVPF_IF_DEFINE(SGUI_INT,			SGUI_FN_IF_INITIALIZE,				(void));
SGUI_DEVPF_IF_DEFINE(void,				SGUI_FN_IF_CLEAR,					(void));
SGUI_DEVPF_IF_DEFINE(void,				SGUI_FN_IF_SET_POINT,				(SGUI_INT iX, SGUI_INT iY, SGUI_INT iColor));
SGUI_DEVPF_IF_DEFINE(SGUI_INT,			SGUI_FN_IF_GET_POINT,				(SGUI_INT iX, SGUI_INT iY));
SGUI_DEVPF_IF_DEFINE(SGUI_INT,			SGUI_FN_IF_SET_BYTE,				(SGUI_INT iPage, SGUI_INT iColumn));
SGUI_DEVPF_IF_DEFINE(SGUI_INT,			SGUI_FN_IF_GET_BYTE,				(SGUI_INT iPage, SGUI_INT iColumn));
SGUI_DEVPF_IF_DEFINE(void,				SGUI_FN_IF_REFRESH,					(void));

// System function interface type declare.
SGUI_DEVPF_IF_DEFINE(void,				SGUI_FN_IF_GET_RTC,					(SGUI_INT iYear, SGUI_INT iMounth, SGUI_INT iDay, SGUI_INT iWeekDay, SGUI_INT iHour, SGUI_INT iMinute, SGUI_INT iSecond));
SGUI_DEVPF_IF_DEFINE(SGUI_INT,			SGUI_FN_IF_GET_CHAR_INDEX,			(SGUI_UINT32 uiCode));
SGUI_DEVPF_IF_DEFINE(SGUI_CSZSTR,       SGUI_FN_IF_STEP_NEXT,               (SGUI_CSZSTR cszSrc, SGUI_UINT32* puiCode));
SGUI_DEVPF_IF_DEFINE(SGUI_SIZE,         SGUI_FN_IF_GET_DATA,                (SGUI_SIZE sStartAddr, SGUI_BYTE* pDataBuffer, SGUI_SIZE sReadSize));
SGUI_DEVPF_IF_DEFINE(SGUI_BOOL,         SGUI_FN_IF_IS_FULL_WIDTH,           (SGUI_UINT32 uiCode));

typedef struct
{
	//Screen display area size in pixel.
	SGUI_AREA_SIZE						stSize;
	//Bitmap data buffer.
	SGUI_BYTE							arrBmpDataBuffer[SGUI_BMP_DATA_BUFFER_SIZE];
    //Engine & device initialize function.
    SGUI_FN_IF_INITIALIZE				fnInitialize;
    //Clear screen function.
    SGUI_FN_IF_CLEAR					fnClear;
    //Set pixel value function.
    SGUI_FN_IF_SET_POINT				fnSetPixel;
    //Get pixel value function.
    SGUI_FN_IF_GET_POINT				fnGetPixel;
    // Sync display buffer data to screen device.
    SGUI_FN_IF_REFRESH					fnSyncBuffer;
}SGUI_SCR_DEV;

typedef struct
{
    SGUI_INT							iHalfWidth;
    SGUI_INT							iFullWidth;
    SGUI_INT							iHeight;
	SGUI_FN_IF_GET_CHAR_INDEX			fnGetIndex;
	SGUI_FN_IF_GET_DATA                 fnGetData;
	SGUI_FN_IF_STEP_NEXT                fnStepNext;
	SGUI_FN_IF_IS_FULL_WIDTH            fnIsFullWidth;
}SGUI_FONT_RES;

typedef struct
{
	SGUI_INT							iWidth;
    SGUI_INT							iHeight;
    const SGUI_BYTE*					pData;
}SGUI_BMP_RES;

#endif // _INCLUDE_GUI_TYPEDEF_H_
