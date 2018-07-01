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
//Режим Редактирования шага
///////////////////////////////////////////////////////////////
void Key_RPS(uint32_t l_Key)
{
int n; //темп переменная для вычислений
static struct CProgS CProgCPY = CPROG_INIT; //Копия структуры программы

	if(l_Key >= IDC_BUTTON_0 && l_Key <= IDC_BUTTON_9)
	{
		Number((l_Key - IDC_BUTTON_0), &CInput);

		CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
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
					CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
				}
				break;
			case IDC_BUTTON_Ent:							//Enter
				if(CInput.Numeric > CUstDef.MinSizeD10 && CInput.Numeric < CUstDef.MaxSizeD10)
				{
					CProg.Step[CProg.NomStep] = CInput.Numeric;
//        WriteCurProg(&CProg);
					CProgCPY.Step[CProg.NomStep] = CInput.Numeric;

					PostMessage(0, WM_COMMAND, IDC_BUTTON_Esc, 0);
				}
				else
				{
					Error(1);
				}
				break;
			case IDC_BUTTON_Ins:										//Ins
				if(CInput.Numeric > CUstDef.MinSizeD10 && CInput.Numeric < CUstDef.MaxSizeD10)
				{
					if (CProg.NomAllStep < STEP_IN_PROG)
					{
						for(n = CProg.NomAllStep; n >= CProg.NomStep; n--)
						{
							CProg.Step[n + 1] = CProg.Step[n];
							CProgCPY.Step[n + 1] = CProgCPY.Step[n];
						}

						CProg.Step[CProg.NomStep] = CProgCPY.Step[CProg.NomStep];
						CProgCPY.Step[CProg.NomStep + 1] = CProg.Step[CProg.NomStep + 1];

						CProg.NomStep++;
						CProgCPY.NomStep++;

						CProg.NomAllStep++;
						CProgCPY.NomAllStep++;

//            WriteCurProg(&CProg);
						PostMessage(0, WM_COMMAND, IDC_BUTTON_Esc, 0);
					}
					else
					{
 //           PostMessage(g_hWnd, WM_COMMAND, IDC_BUTTON22, 0);
						Error(3);//g_Error = 3;				//Исчерпано количество шагов
					}
				}
				else
				{
					Error(1);//g_Error = 1;					//Не правильное значение шага
				}
				break;
			case IDC_BUTTON_Esc:								//Escape   //отмена введенного, переход в другое состояние
	//        n = CProg.NomStep;
	//        toReadProgram(&CProg, CProg.CurProg);
				for (n = 0; n < CProg.NomAllStep + 1; n++)
					CProg.Step[n] = CProgCPY.Step[n];
	//        CProg.NomStep = n;
				init_CInputFlick(fNoFlick, 0);
				init_CInput(0, 0, 0);
				g_Regim = 3;
				Key = Key_R3; 
				break;
			case IDC_ERROR:
				init_CInput(0, 0, 0);
				Key = Key_Er;
				break;
			case IDC_COPY_PRG:                        //создание копии текущей программы для возможности безболезненного редактирования
				CProgCPY.CurProg = CProg.CurProg;
				CProgCPY.NomAllStep = CProg.NomAllStep;
				CProgCPY.NomStep = CProg.NomStep;
				for(n = 0; n < STEP_IN_PROG + 1; n++)
					CProgCPY.Step[n] = CProg.Step[n];

				CProg.Step[CProg.NomStep] = CInput.Numeric;    //синхронное редактирование шага и вводимого значения
				break;
			default:
				break;
		}
  }
  toDisplay(&CProg, &CInput, &CInputFlick);
}
