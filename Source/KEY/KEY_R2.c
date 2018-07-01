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
//Режим №2 Выполнение программы
///////////////////////////////////////////////////////////////
void Key_R2(uint32_t l_Key)
{
static int  l_Required;     //темп переменная для вычислений

	if(l_Key >= IDC_BUTTON_0 && l_Key <= IDC_BUTTON_9)
	{
		Number((l_Key - IDC_BUTTON_0), &CInput);

		if(b_NoKeyP == 0)       //обновление номера программы синхронно с вводом числа
		{
			if (CInput.NumericInt > 0 && CInput.NumericInt < PROG_IN_MEMORY - 1) //Переход на заданную программу
			{
				CProg.CurProg = CInput.NumericInt;
			}
			else                  //В случае "неправильный номер ?шага? программы" сбросить число
			{
				init_CInput((l_Key - IDC_BUTTON_0), 1, 0);    //если введено более 2 цифр сбросить счетчик				
				CProg.CurProg = CInput.NumericInt;
			}    
			toReadProgram(&CProg, CProg.CurProg);
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
				}
				break;
			case IDC_BUTTON_Ent:							//Enter
				if (b_NoKeyP == 0)             //Подтвердили выбор программы (1 - программа выбрана)
				{
					b_NoKeyP = 1;

					if (CInput.NumericInt > 0 && CInput.NumericInt < PROG_IN_MEMORY - 1) //Переход на заданную программу
					{
						CProg.CurProg = CInput.NumericInt;
						toReadProgram(&CProg, CProg.CurProg);
					}
					else                  //Показать ошибку "неправильный номер ?шага? программы"
					{
						if (CInput.NumericInt != 0)
							Error(2);
					}
					init_CInput(0, 0, 0);
					init_CInputFlick(fNoFlick, 0);
				}
				else
				{
					b_NoKeyP = 0;
 
//          toReadProgram(&CProg, CInput.NumericInt);
					init_CInput(0, 0, 0);
					init_CInputFlick(fCurProg, -1);
//          init_CInput(0, 0, 0);
				}
				break;
			case IDC_BUTTON_F:                                //F (second function mod)
				if (g_Regim == 2 && b_NoKeyP == 1)
				{
					Key = Key_SK;
				}
				break;
			case IDC_BUTTON_P:							//P
				if (g_Regim == 2)
				{
					b_NoKeyP = 1;
					init_CInput(0, 0, 0);
					init_CInputFlick(fNoFlick, 0);					
					g_Regim = 1;
					Key = Key_R1;
				}
				break;
			case IDC_BUTTON_Lft:								//<-
				if (b_NoKeyP == 0)					//Декримент номера рограммы
				{
					CProg.CurProg--;
					if(CProg.CurProg < 1)
						CProg.CurProg = PROG_IN_MEMORY - 1;

					toReadProgram(&CProg, CProg.CurProg);
				}
				else if (b_NoKeyP == 1)				//Переход на определенный шаг
				{
					if (CInput.NumericInt > 0 && CInput.NumericInt < (CProg.NomAllStep + 1)) //Переход на заданный шаг
					{
						CProg.NomStep = CInput.NumericInt;
					}
					else if(CInput.NumericInt == 0)                      //Переход на предидущий шаг
					{
						if (CProg.NomStep != 0)
						{
							CProg.NomStep--;
							if(CProg.NomStep < 1)
								CProg.NomStep = CProg.NomAllStep;
						}
					}
					else										//Показать ошибку "неправильный номер шага"
					{
						Error(2);//g_Error = 2;
						init_CInput(0, 0, 0);
						break;
					}

					l_Required = CProg.Step[CProg.NomStep];   //Выполнить движение подователем
//					xRunSend(l_Required);
					PostMessage(0, WM_MOTION, IDC_RUN, l_Required);
					init_CInput(0, 0, 0);
				}
				break;
			case IDC_BUTTON_Rht:								//->
				if (b_NoKeyP == 0)					//Инкремент номера рограммы
				{
					CProg.CurProg++;
					if(CProg.CurProg > PROG_IN_MEMORY - 1)
						CProg.CurProg = 1;

					toReadProgram(&CProg, CProg.CurProg);
				}
				else if (b_NoKeyP == 1)				//Переход на определенный шаг
				{
					if (CInput.NumericInt > 0 && CInput.NumericInt < (CProg.NomAllStep + 1)) //Переход на заданный шаг
					{
						CProg.NomStep = CInput.NumericInt;
					}
					else if(CInput.NumericInt == 0)                      //Переход на следующий шаг
					{
						if (CProg.NomStep != 0)
						{
							CProg.NomStep++;
							if(CProg.NomStep > CProg.NomAllStep)
								CProg.NomStep = 1;
						}
					}
					else										//Показать ошибку "неправильный номер шага"
					{
						Error(2);//g_Error = 2;
						init_CInput(0, 0, 0);
						break;
					}

					l_Required = CProg.Step[CProg.NomStep];
//					xRunSend(l_Required);
					PostMessage(0, WM_MOTION, IDC_RUN, l_Required);					
					init_CInput(0, 0, 0);
				}
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
//						xRunSend(l_Required);
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
//				xRunSend(STOP);                  //!!!!!!!!   муфта
				PostMessage(0, WM_MOTION, IDC_STP, 0);			
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
//						xRunSend(l_Required);
						PostMessage(0, WM_MOTION, IDC_RUN, l_Required);
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
			case IDC_KNIFE:
				PostMessage(0, WM_COMMAND, IDC_BUTTON_Rht, 0);
				break;			
			default:
				break;
		}
	}
	toDisplay(&CProg, &CInput, &CInputFlick);
}
