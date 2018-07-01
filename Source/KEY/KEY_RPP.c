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
//Режим Программирования программы
///////////////////////////////////////////////////////////////
void Key_RPP(uint32_t l_Key)
{
int n; //темп переменная для вычислений
static int l_PrConf = 0; //флаг редактирования шага
static struct CProgS CProgCPY = CPROG_INIT; //Копия структуры программы
int l_Required; //темп переменная для вычислений

	if(l_Key >= IDC_BUTTON_0 && l_Key <= IDC_BUTTON_9)
	{
		Number((l_Key - IDC_BUTTON_0), &CInput);

		if (l_PrConf == 1)
		{
			CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
		}
	}
	else
	{
		switch(l_Key)
		{
			case IDC_BUTTON_Pnt:								//.
				if (CInput.Point == 0 && b_NoKeyP == 1)
				{
					if (CInput.Count < NUM_TO_POINT + 1)
					{
						init_CInput(CInput.Numeric, NUM_TO_POINT, 1);
					}
					else
					{
						init_CInput(CInput.Numeric, CInput.Count, 1);
					}

					if (l_PrConf == 1)
					{
						CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
					}
				}
				break;
			case IDC_BUTTON_Ent:							//Enter
				if (l_PrConf == 1)
				{
					l_PrConf = 0;
					if(CInput.Numeric > CUstDef.MinSizeD10 && CInput.Numeric < CUstDef.MaxSizeD10)
					{
						CProg.Step[CProg.NomStep] = CInput.Numeric;

						CProgCPY.NomStep++;
						CProgCPY.NomAllStep++;
						CProgCPY.Step[CProg.NomStep] = CInput.Numeric;

						init_CInputFlick(fNoFlick, 0);
						init_CInput(0, 0, 0);
						//PostMessage(g_hWnd, WM_COMMAND, IDC_BUTTON22, 0);
					}
					else
					{
						Error(1);
					}
				}
				else
				{
					if (CProg.NomAllStep < STEP_IN_PROG)
					{
						CProg.NomStep++;
						CProg.NomAllStep++;
						init_CInput(ACTIVE_10, NUM_TO_POINT + 1, 1);
						init_CInputFlick(fStep, -1);
						l_PrConf = 1;
						CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
					}
					else
					{
						Error(3);
					}
				}
				break;
			case IDC_BUTTON_F:							// -----> (F)
				if (l_PrConf == 0)
					Key = Key_SK;
				break;
			case IDC_BUTTON_Up:                                    //^
				if (CInput.Numeric == 0)
				{
					PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MaxSizeD10);
					g_OffMotion = 1;							//разрешение остановки при отжатии клавиши					
				}
				else                                  //относительный "+"
				{
					if(CInput.Numeric + ACTIVE_10 < CUstDef.MaxSizeD10)
					{
						l_Required = CInput.Numeric + ACTIVE_10;
//              xRunSend(l_Required);
						PostMessage(0, WM_MOTION, IDC_RUN, l_Required);
					}
					else
					{
						Error(1);
					}
				}
				break;
			case IDC_BUTTON_Esc:								//Escape   //отмена введенного, переход в другое состояние
				if (l_PrConf == 1)
				{
					init_CInput(0, 0, 0);        
				}
				break;
			case IDC_BUTTON_Dn:                                    //_
				if (CInput.Numeric == 0)
				{
					PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MinSizeD10);
					g_OffMotion = 1;							//разрешение остановки при отжатии клавиши					
				}
				else                               //относитльный "-"
				{
					if(ACTIVE_10 - CInput.Numeric < CUstDef.MinSizeD10)
					{
						l_Required = ACTIVE_10 - CInput.Numeric;
//              xRunSend(l_Required);
						PostMessage(0, WM_MOTION, IDC_RUN, l_Required);
					}
					else
					{
						Error(1);
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
			case IDC_COPY_PRG:                        //создание копии текущей программы для возможности безболезненного редактирования
				CProgCPY.CurProg = CProg.CurProg;
				CProgCPY.NomAllStep = CProg.NomAllStep = 1;
				CProgCPY.NomStep = CProg.NomStep = 1;
				for(n = 0; n < STEP_IN_PROG + 1; n++)
					CProgCPY.Step[n] = CProg.Step[n] = EMPTY_STEP;

				CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
				l_PrConf = 1;
				break;
			case IDC_PRG_OUT:
				for (n = 0; n < CProg.NomAllStep + 1; n++)
					CProg.Step[n] = CProgCPY.Step[n];

				init_CInputFlick(fNoFlick, 0);
				init_CInput(0, 0, 0);
				l_PrConf = 0;
				g_Regim = 3;
				Key = Key_R3; 
				PostMessage(0, WM_OPERATION, IDC_PROG_OUT, 0);
				break;
			case IDC_KNIFE:
				PostMessage(0, WM_COMMAND, IDC_BUTTON_Ent, 0);
				break;
			default:
				break;
		}
	}
	toDisplay(&CProg, &CInput, &CInputFlick);
}
