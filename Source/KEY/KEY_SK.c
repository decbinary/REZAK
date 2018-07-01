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
//Режим Дополнительной клавиши
///////////////////////////////////////////////////////////////
void Key_SK(uint32_t l_Key)
{
	switch(l_Key)
	{
		case IDC_BUTTON_P:
			if(g_Regim == 2)
			{
				g_Regim = 3;
				Key = Key_R3; 
			}
			else
			if(g_Regim == 3)    //!!!!!!!!!!!!!!!!!!!
			{
				Key = Key_R3;
				PostMessage(0, WM_OPERATION, IDC_PROG_OUT, 0 );
			}
			else
			if(g_Regim == 6)
			{
				Key = Key_RPP;
				PostMessage(0, WM_OPERATION, IDC_PRG_OUT, 0 );
			}
			break;
		case IDC_BUTTON_Del:                           //Del программы
			if (g_Regim == 3)
			{
				Key = Key_R3;
				PostMessage(0, WM_OPERATION, IDC_PROG_DEL, 0 );
			}
			break;
		case IDC_BUTTON_Dn:
		case IDC_BUTTON_Up:			//абсолютный размер
			if(CInput.Numeric > CUstDef.MinSizeD10 && CInput.Numeric < CUstDef.MaxSizeD10)
			{
//				xRunSend(CInput.Numeric);
				PostMessage(0, WM_MOTION, IDC_RUN, CInput.Numeric);				
			}
			else
			{
				Error(1);
			}        
			break;
		case IDC_BUTTON_F_OFF:                            //Реакция на отжатие клавиши (F)
			if (g_Regim == 0)
				Key = Key_R0;
				else if (g_Regim == 1)
					Key = Key_R1;
				else if (g_Regim == 2)
					Key = Key_R2;
				else if (g_Regim == 3)
					Key = Key_R3;
				break;
		case IDC_BUTTON_Esc:								//Escape   //остановка при нажатии
//			xRunSend(STOP);
			PostMessage(0, WM_MOTION, IDC_STP, 0);		
			break;
		case IDC_RUN_BLOCK:                            //Изменение режима при движении
			Key = Key_Cl;
			break;				
		case IDC_ERROR:
			Key = Key_Er;
			break;
		case IDC_BUTTON_Pnt:
			if (g_Regim == 1 && CInput.Numeric == PASSWORD)
			{
					Key = Key_R1;
					PostMessage(0, WM_OPERATION, IDC_USTR, 0);				
			}
			break;		
		default:
			break;
	}   
	toDisplay(&CProg, &CInput, &CInputFlick);
}
