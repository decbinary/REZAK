//#include <Windows.h>
#include <stdint.h>
#include "KEYBOARD.h"
#include "CYCLE.h"
#include "Queue.h"
#include "PostMessage.h"
#include "DlgProc.h"
//#include "REZAK.h"// только для hwnd

struct CRunStepS CRunStep = CRUNSTEP_INIT;

/*

*/

void xRunSend(int32_t l_Nom)
{
  if (l_Nom >= CUstDef.MinSizeD10 && l_Nom <= CUstDef.MaxSizeD10)
  {
		CRunStep.RezStep = l_Nom*10;

		if((g_Active - CRunStep.RezStep) > 0)         //Движение вперед
		{
			if ((g_Active - CRunStep.RezStep) <= CUstDef.Cross)  //Если расстояние меньше пути заезда за точку повторить заезд
			{
				CRunStep.AddSub = 0;
				PostMessage(0, WM_MOTION, IDC_DN_LO, 0);	//DnLSpd
			}
			else
			{
				if ((g_Active - CRunStep.RezStep - CUstDef.DnSpd - CUstDef.DnSpd - CUstDef.DnDist) <= 0)  //Стоит ли переходить на большую скорость
					CRunStep.AddSub = 1;
				else
				{
					CRunStep.AddSub = 4;
					CRunStep.gHSpd = g_Active - CUstDef.DnDist - CRunStep.RezStep;  //Вычисление расстояния для перехода на большую скорость
				}
				PostMessage(0, WM_MOTION, IDC_UP_LO, 0);	//UpLSpd
			}
		}
		else                                           //Движение назад
		{
			if ((int32_t)(CRunStep.RezStep - g_Active) <= CUstDef.Acur)  //Если расстояние меньше тормозного пути повторить заезд
				CRunStep.AddSub = 0;
			else
			if((int32_t)(CRunStep.RezStep - g_Active - CUstDef.DnSpd - CUstDef.DnSpd - CUstDef.DnDist) <= 0)  //Стоит ли переходить на большую скорость
				CRunStep.AddSub = 0;            //
			else
			{
				CRunStep.AddSub = 5;
				CRunStep.gHSpd = g_Active + CUstDef.DnDist - CRunStep.RezStep;  //Вычисление расстояния для перехода на большую скорость
			}
			PostMessage(0, WM_MOTION, IDC_DN_LO, 0);	//DnLSpd
		}
		PostMessage(0, WM_OPERATION, IDC_RUN_BLOCK, 0 ); //Записываем в очередь обработки "кнопок" "блокировка при движении"
  }
}

void xStpSend(int32_t l_Nom)
{
	if (l_Nom == STOP)
	{	
		CRunStep.AddSub = 3;
		PostMessage(0, WM_MOTION, IDC_STP_SPD, 0);	//Записываем в очередь обработки "остановка"
		PostMessage(0, WM_OPERATION, IDC_STP_UNLK, 0); //Записываем в очередь обработки "кнопок" "разблокировка при стопе"	
	}
}
/*
void EXTERN_INTER( HWND hWnd )
{
BOOL b_DecA;
  if (b_A == 1)
  {
    if (b_B == 1)
      b_DecA = 0;      //g_Active++;
    else
      b_DecA = 1;       //g_Active--;
  }
  else
  {
    if (b_B == 1)
      b_DecA = 1;       //g_Active--;
    else
      b_DecA = 0;       //g_Active++;
  }

  if (b_Fasten == 1)    //Привязка линейки
  {
    if (b_DecA == 1)
      g_Active -=5 ;             //!!!!!!!!!!!!!
    else
      g_Active +=5;             //!!!!!!!!!!!!!

    if (g_AddSub != 3)
      Motion();  
  }
  else
  {
    if (b_Priv == 1)
    {
      b_Link = 1;
      if (b_DecA == 0)
        g_AddSub = 9;     //!!!!!!!!!!!!!!Остановка только при ходе назад
      PostMessage(hWnd, WM_COMMAND, 60, 0 ); //Записываем в очередь обработки "кнопок"
    }

    if (b_Priv == 0 && b_Link == 1 && b_DecA == 1)
    {
      b_Fasten = 1;
    }
    Motion();
  }
}
*/
