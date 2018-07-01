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
//Режим Блокировки клавиатуры
///////////////////////////////////////////////////////////////
void Key_Cl(uint32_t l_Key)
{
	switch(l_Key)
	{
		case IDC_BUTTON_Esc:								//Escape   //остановка при нажатии
			//xRunSend(STOP);
			PostMessage(0, WM_MOTION, IDC_STP, 0);
			break;
		case IDC_STP_UNLK:                            //Изменение режима при конце выполнения шага
			if (g_Regim == 0)
				Key = Key_R0;
			else if (g_Regim == 1)
				Key = Key_R1;
			else if (g_Regim == 2)
				Key = Key_R2;
			else if (g_Regim == 3)
				Key = Key_R3;
			else if (g_Regim == 6)
				Key = Key_RPP;			
			break;
		case IDC_LINK:                            //Изменение режима при наезде на концевик привязки
			if (g_Regim == 0)
			{
				//xRunSend(STOP);
				PostMessage(0, WM_MOTION, IDC_STP, 0);
				Key = Key_R1;
				g_Active = CUstDef.Mark;
				g_Regim = 1;
			}
			break;
		case IDC_BUTTON_Up_OFF:
		case IDC_BUTTON_Dn_OFF:			
			if (g_OffMotion == 1)
			{
				g_OffMotion = 0;
				PostMessage(0, WM_MOTION, IDC_STP, 0);
			}
			break;
		default:
			break;
	}   
}
