// #include <windows.h>
// #include "KEYBOARD.h"
// #include "CYCLE.h"
// #include "READ_WRITE_DATA.h"
// #include "resource.h"
// #include "REZAK.h"// только для hwnd
#include <stdint.h>
#include "KEYBOARD.h"
#include "rotary_encoder.h"
#include "Queue.h"
#include "DlgProc.h"
#include "PostMessage.h"
///////////////////////////////////////////////////////////////
//Режим Ошибки
///////////////////////////////////////////////////////////////
void Key_Er(uint32_t l_Key)
{
	switch(l_Key)
	{
		case IDC_BUTTON_Esc:								//Escape   //сброс ошибки
			g_Error = 0;
                              //Изменение режима при конце режима ошибки
			if (g_Regim == 0)
				Key = Key_R0;
			else if (g_Regim == 1)
				Key = Key_R1;
			else if (g_Regim == 2)
				Key = Key_R2;
			else if (g_Regim == 3)
				Key = Key_R3;
			else if (g_Regim == 4)
				Key = Key_RD;
			else if (g_Regim == 5)
				Key = Key_RPS;
			else if (g_Regim == 6)
				Key = Key_RPP;
			break;
		default:
			break;
	} 
	CDisp.Error = g_Error;    
}


///////////////////////////////////////////////////////////////
//Обработка Ошибки
///////////////////////////////////////////////////////////////
void Error(unsigned char l_Error)
{
	if (l_Error == 1)
	{
		init_CInputFlick(fStep, FLICK_TIME);
		return;
	}
	else
	if (l_Error == 2)
	{
		init_CInputFlick(fNomStep, FLICK_TIME);           
		return;
	}
//  Key = Key_Er;
	g_Error = l_Error;
	PostMessage(0, WM_OPERATION, IDC_ERROR, 0);   //Для ясности из какой функции идет перемена режима
}
