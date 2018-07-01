#include "stm32f10x_conf.h"
#include "zatl_stop.h"
#include "CYCLE.h"
#include "Queue.h"
#include "PostMessage.h"
#include "DlgProc.h"
#include "rotary_encoder.h"
#include "KEYBOARD.h"
//#include "7seg.h"

/**
 * Настройка прерывания мигания
 * @return
 */
void STOP_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseConfig;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		/* TIM2 clock enable */
	TIM_BaseConfig.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1; 	// Запускаем таймер на тактовой частоте в 1 kHz (1mS)
	TIM_BaseConfig.TIM_Period = CUstDef.TimeStop; 	// STOP_TIME ms
	TIM_BaseConfig.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up; 	// Отсчет от нуля до TIM_Period

	TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	
	NVIC_EnableIRQ(TIM3_IRQn);
	
}

//int CALLBACK ZATLSTOP (HWND hWnd,UINT uMsg,UINT idEvent,DWORD dwTime)
void ZatlStop(void)
{
	KillTimer();     //!!!!!!!!!!!!!!!
	
	if (CRunStep.AddSub == 7)    
	{
		CRunStep.AddSub = 1;
		PostMessage(0, WM_MOTION, IDC_UP_LO, 0);//	Direction(UpLSpd);
	}
	else if (CRunStep.AddSub == 8)    
	{
		CRunStep.AddSub = 3;
		PostMessage(0, WM_OPERATION, IDC_STP_UNLK, 0); //Записываем в очередь обработки "кнопок" "разблокировка при стопе"
		if (g_Active - CRunStep.RezStep > 0)     //Если не доехало или переехало расстояние больше точности
		{
			if (g_Active - CRunStep.RezStep > CUstDef.Acur)
			{
				if (g_Regim == 2)
				{
					PostMessage(0, WM_COMMAND, IDC_BUTTON_P, 0);				
				}
				Error(10);//g_Error = 10;  //Прошиб размер					
			}
			else
			{
				if (g_Regim == 2)
				{
					CPrecise.FActive = CRunStep.RezStep;
					CPrecise.Show = 1;
				}
			}
		}
		else
		{
			if (CRunStep.RezStep - g_Active > CUstDef.Acur)
			{
				if (g_Regim == 2)
				{
					PostMessage(0, WM_COMMAND, IDC_BUTTON_P, 0);				
				}				
				Error(10);//g_Error = 10;  //Прошиб размер			
			}
			else
			{
				if (g_Regim == 2)
				{
					CPrecise.FActive = CRunStep.RezStep;
					CPrecise.Show = 1;
				}
			}
		}
	}
}

void KillTimer(void)
{
	TIM_Cmd(TIM3, DISABLE);
}

void SetTimer(void)
{
	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);
}

void ResetTimer(void)
{
	TIM_SetCounter(TIM3, 0);
}
