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
#include "READ_WRITE_DATA.h"
///////////////////////////////////////////////////////////////
//Режим №1 Движение вручную
///////////////////////////////////////////////////////////////
void Key_R1(uint32_t l_Key)
{
static int  l_Required;         //темп переменная для вычислений

	if(l_Key >= IDC_BUTTON_0 && l_Key <= IDC_BUTTON_9)
	{
//    g_Numeric = Number_R1 (l_Key, &l_Count, g_Numeric, &b_Point);
		Number((l_Key - IDC_BUTTON_0), &CInput);
//    l_Count++;
	}
	else
	{
		switch(l_Key)
		{
			case IDC_BUTTON_Pnt:								//.
				if (CInput.Point == 0)
				{
					if (CInput.Count < NUM_TO_POINT + 1)
					{
						init_CInput(CInput.Numeric, NUM_TO_POINT, 1);
					}
					CInput.Point = 1;
				}
				break;
			case IDC_BUTTON_P:							//P
				if (g_Regim == 1)
				{
					b_NoKeyP = 1;
					toReadProgram(&CProg, 1);
//					Error(5);
					init_CInput(0, 0, 0);

					g_Regim = 2;
					Key = Key_R2;
				}
				break;
			case IDC_BUTTON_F:							//F (second function mod)
				if (g_Regim == 1)
				{
					b_NoKeyP = 0;
					Key = Key_SK;
				}
				break;
			case IDC_BUTTON_Up:                                    //^  //относительный размер "+"
				if (CInput.Numeric == 0)
				{
					PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MaxSizeD10); //!!!!! изменение режима для определения момента отпускания кнопки
					g_OffMotion = 1;
				}
				else
				{
					if(CInput.Numeric + ACTIVE_10 < CUstDef.MaxSizeD10)
					{
						l_Required = CInput.Numeric + ACTIVE_10;
			
//          xRunSend(l_Required);
						PostMessage(0, WM_MOTION, IDC_RUN, l_Required);
					}
					else
					{
						Error(1);//g_Error = 1;
					}
				}
				break;
			case IDC_BUTTON_Esc:								//Escape   //остановка при нажатии
				init_CInput(0, 0, 0);
//        b_NoKeyP = 1;
//        xRunSend(STOP);
				PostMessage(0, WM_MOTION, IDC_STP, 0);	
				break;
			case IDC_BUTTON_Dn:                                    //_  //относительный размер "-"
				if (CInput.Numeric == 0)
				{
					PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MinSizeD10); //!!!!! изменение режима для определения момента отпускания кнопки					
					g_OffMotion = 1;
				}
				else
				{
					if(ACTIVE_10 - CInput.Numeric > CUstDef.MinSizeD10)
					{
						l_Required = ACTIVE_10 - CInput.Numeric;
//              xRunSend(l_Required);  
						PostMessage(0, WM_MOTION, IDC_RUN, l_Required);							
//              init_CInput(0, 0, 0);
					}
					else
					{
						Error(1);//g_Error = 1;
					}
				}
				break;
			case IDC_BUTTON_o:									//Изменение состояния муфты
				PostMessage(0, WM_MOTION, IDC_CLUTCH, 0);
				break;	
			case IDC_RUN_BLOCK:                            //Изменение режима при движении
				Key = Key_Cl;
				break;
			case IDC_ERROR:
				init_CInput(0, 0, 0);
				Key = Key_Er;
				break;
			case IDC_USTR:											//Чтение программы юстировочных коэффициентов
				b_NoKeyP = 1;
				CProg.CurProg = 0;
				CProg.NomStep = 1;
				CUstDef.MinSizeD10 = -1;
				CUstDef.MaxSizeD10 = 100000;
				if (ReadCurProg(&CProg, CProg.CurProg) == 1)
				{
					Error(5);
				}
				g_Regim = 3;
				Key = Key_R3;
				PostMessage(0, WM_COMMAND, IDC_BUTTON_Ent, 0);			
				break;			
			default:
				break;
		}
	}
	toDisplay(&CProg, &CInput, &CInputFlick);
}
