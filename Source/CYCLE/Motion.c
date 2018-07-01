#include <stdint.h>
#include "Queue.h"
#include "Motion.h"
#include "rotary_encoder.h"
#include "CYCLE.h"
#include "KEYBOARD.h"
#include "PostMessage.h"
#include "DlgProc.h"
#include "zatl_stop.h"

void Motion()
{
	int32_t l_Delta;

	l_Delta = g_Active - CRunStep.RezStep;

	switch (CRunStep.AddSub)
	{
		case 0:
			if(l_Delta > CUstDef.Cross)          //Ожидание заезда за точку
			{
				CRunStep.AddSub = 7;             //1
				PostMessage(0, WM_MOTION, IDC_STP_SPD, 0);

//				SetTimer(g_hWnd,10,10,(TIMERPROC)ZATLSTOP);  //В очередь для ZATLSTOP() пишем l_RezStep
				SetTimer();
			}
			break;
		case 1:
			if(l_Delta < CUstDef.Stop)                 //Ожидание остановки
			{
				CRunStep.AddSub = 8;             //3
				PostMessage(0, WM_MOTION, IDC_STP_SPD, 0);
//				SetTimer(g_hWnd,10,10,(TIMERPROC)ZATLSTOP);  //В очередь для ZATLSTOP() пишем l_RezStep
				SetTimer();				
			 }
			 break;
		case 2:                                  
			if(l_Delta < CUstDef.DnSpd)                  //Ожидание перехода на малую переднюю
			{
				CRunStep.AddSub = 1;
				PostMessage(0, WM_MOTION, IDC_UP_LO, 0);				 
			}
			break;
		case 3:																			//Конец цикла, нет движения
//				Direction(StpSpd);
//			PostMessage(0, WM_MOTION, IDC_STP, 0);		 
			break;
		case 4:
			if(l_Delta < CRunStep.gHSpd)                   //Ожидание перехода на большую переднюю
			{
				CRunStep.AddSub = 2;
				PostMessage(0, WM_MOTION, IDC_UP_HI, 0);				
			}
			break;
		case 5:
			if(l_Delta > CRunStep.gHSpd)                  //Ожидание перехода на большую заднюю
			{
				CRunStep.AddSub = 6;
				PostMessage(0, WM_MOTION, IDC_DN_HI, 0);				
			}
			break;
		case 6:
			if(l_Delta > 0)                  //Ожидание перехода на малую заднюю
			{
				CRunStep.AddSub = 0;
				PostMessage(0, WM_MOTION, IDC_DN_LO, 0);				
			}
			break;
		case 7:
		case 8:
				ResetTimer();
			break;
		case 9:
			CRunStep.AddSub = 3;
			PostMessage(0, WM_MOTION, IDC_STP, 0);		
			break;

		default :
			CRunStep.AddSub = 3;
			break;
	}
}
