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
//Режим №3 Изменение программы
///////////////////////////////////////////////////////////////
void Key_R3(uint32_t l_Key)
{
    
int n;
int l_Required; //темп переменная для вычислений
//static int l_PrConf = 0; //флаг редактирования шага
//static int l_RNewProg = 0; //флаг программирования новой программы

	if(l_Key >= IDC_BUTTON_0 && l_Key <= IDC_BUTTON_9)
	{
		Number((l_Key - IDC_BUTTON_0), &CInput);
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
				CPrecise.Show = 0;	// сброс скрытия ошибки
				if (CProg.NomAllStep == 0)  //Если программа пуста, то вкл. режим программирования
				{
					g_Regim = 6;
					Key = Key_RPP;

					init_CInput(ACTIVE_10, NUM_TO_POINT + 1, 1);
					init_CInputFlick(fStep, -1);
					PostMessage(0, WM_OPERATION, IDC_COPY_PRG, 0);
				}
				else                    //Если нет, редактирование шага
				{
					g_Regim = 5;
					Key = Key_RPS;

					init_CInput(ACTIVE_10, NUM_TO_POINT + 1, 1);
					init_CInputFlick(fStep, -1);
					PostMessage(0, WM_OPERATION, IDC_COPY_PRG, 0);
				}
				break;
			case IDC_BUTTON_F:							// -----> (F)
				Key = Key_SK;
				break;
			case IDC_BUTTON_Lft:								//<-
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
				init_CInput(0, 0, 0);
				break;
			case IDC_BUTTON_Rht:								//->
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
				init_CInput(0, 0, 0);
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
						Error(1);//g_Error = 1;
					}
				}
				break;///*
			case IDC_BUTTON_Del:                           //Del
				if (CProg.NomAllStep > 1)
				{
					for(n = CProg.NomStep; n < CProg.NomAllStep; n++)
						CProg.Step[n] = CProg.Step[n + 1];

					CProg.Step[CProg.NomAllStep] = EMPTY_STEP;

					if (CProg.NomStep == CProg.NomAllStep)
						CProg.NomStep--;

					CProg.NomAllStep--;
				}
				else
				if (CProg.NomAllStep <= 1)
				{
					CProg.Step[0] = EMPTY_STEP;       //очищаем поле crc
					CProg.NomAllStep = CProg.NomStep = 0;
					CProg.Step[1] = EMPTY_STEP;
				}
				break;
			case IDC_BUTTON_Esc:								//Escape   //остановка при нажатии
/*        if (l_RNewProg == 1)
        {
          PostMessage(g_hWnd, WM_COMMAND, 64, 0 );
        }
        else
        {
          toReadProgram(&CProg, CProg.CurProg);
        }*/
				init_CInputFlick(fNoFlick, 0);
				init_CInput(0, 0, 0);
//        l_PrConf = 0;
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
			case IDC_PROG_OUT:
				WriteCurProg(&CProg);

				init_CInputFlick(fNoFlick, 0);
				init_CInput(0, 0, 0);
//      l_RNewProg = 0;
//      l_PrConf = 0;
				g_Regim = 2;
				Key = Key_R2;
			
				if (CProg.CurProg == 0)
				{
					ReadServise();
				}	
				break;
			case IDC_PROG_DEL:
				if (CProg.NomAllStep > 0)
				{
					for(n = 0; n < STEP_IN_PROG + 1; n++)
						CProg.Step[n] = EMPTY_STEP;

					CProg.Step[0] = EMPTY_STEP;       //очищаем поле crc
					CProg.NomAllStep = CProg.NomStep = 0;
					CProg.Step[1] = EMPTY_STEP;
				}
				break;
			default:
				break;
		}
	}
	toDisplay(&CProg, &CInput, &CInputFlick);
}
