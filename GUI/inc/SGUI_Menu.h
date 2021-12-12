#ifndef _INCLUDE_SGUI_MENU_H_
#define _INCLUDE_SGUI_MENU_H_
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_ItemsBase.h"
//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_RECT_AREA				stLayout;
	SGUI_ITEMS_BASE				stItems;
	const SGUI_FONT_RES*        pstFontRes;
}SGUI_MENU_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void SGUI_Menu_Initialize(SGUI_MENU_STRUCT* pstObj, const SGUI_FONT_RES* pstFontRes);
void SGUI_Menu_BindItemsData(SGUI_MENU_STRUCT* pstObj, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount);
void SGUI_Menu_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_MENU_STRUCT* pstObj);


#endif // _INCLUDE_SGUI_MENU_H_
