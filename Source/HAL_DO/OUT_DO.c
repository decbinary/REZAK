#include <stdint.h>
#include "OUT_DO.h"
#include "stm32f10x_conf.h"

static CLUTCH CState = CLUTCH_ON;

#define OUT_OFF(port, pin)			GPIO_ResetBits(port, pin)
#define OUT_ON(port, pin)			GPIO_SetBits(port, pin)

void DO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(UpHSpd_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CLUTCH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(CLUTCH_PORT, &GPIO_InitStructure);
	
	OUT_OFF(DnLSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
	OUT_ON(CLUTCH_PORT, CLUTCH_PIN);
}

void Direction(enum MOTOR MState)
{
#ifdef FOUR_SPEED
	switch (MState)
	{
		case DnLSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnLSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN);
			OUT_ON(DnLSpd_PORT, DnLSpd_PIN);
			break;
		case DnHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnHSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnLSpd_PIN);
			OUT_ON(DnHSpd_PORT, DnHSpd_PIN); 
			break;
		case UpLSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(UpLSpd_PORT, UpHSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			OUT_ON(UpLSpd_PORT, UpLSpd_PIN);  
			break;
		case UpHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnLSpd_PORT, UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			OUT_ON(UpHSpd_PORT, UpHSpd_PIN);
			break;
		case StpSpd:
		default:
			OUT_OFF(DnLSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			SetCoupling(CLUTCH_ON);
			break;
	}
#elif defined THREE_SPEED
	switch (MState)
	{
		case DnLSpd:
		case DnHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnHSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnLSpd_PIN);
			OUT_ON(DnHSpd_PORT, DnHSpd_PIN); 
			break;
		case UpLSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(UpLSpd_PORT, UpHSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			OUT_ON(UpLSpd_PORT, UpLSpd_PIN);  
			break;
		case UpHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnLSpd_PORT, UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			OUT_ON(UpHSpd_PORT, UpHSpd_PIN);
			break;
		case StpSpd:
		default:
			OUT_OFF(DnLSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			SetCoupling(CLUTCH_ON);
			break;
	}	
#elif defined TWO_SPEED
	switch (MState)
	{
		case DnLSpd:
		case DnHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(DnHSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnLSpd_PIN);
			OUT_ON(DnHSpd_PORT, DnHSpd_PIN); 		
			break;
		case UpLSpd:
		case UpHSpd:
			SetCoupling(CLUTCH_OFF);
			OUT_OFF(UpLSpd_PORT, UpHSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			OUT_ON(UpLSpd_PORT, UpLSpd_PIN);  
			break;
		case StpSpd:
		default:
			OUT_OFF(DnLSpd_PORT, UpHSpd_PIN | UpLSpd_PIN | DnHSpd_PIN | DnLSpd_PIN);
			SetCoupling(CLUTCH_ON);
			break;
	}
#endif	
}

void ToggleCoupling(void)
{
	if (CState == CLUTCH_OFF)
	{
		CState = CLUTCH_ON;
		OUT_ON(CLUTCH_PORT, CLUTCH_PIN);
	}
	else
	if (CState == CLUTCH_ON)
	{
		CState = CLUTCH_OFF;
		OUT_OFF(CLUTCH_PORT, CLUTCH_PIN);
	}
}

void SetCoupling (enum CLUTCH lState)
{
	if (lState == CLUTCH_ON)
	{
		CState = CLUTCH_ON;
		OUT_ON(CLUTCH_PORT, CLUTCH_PIN);
	}
	else
	if (lState == CLUTCH_OFF)
	{
		CState = CLUTCH_OFF;
		OUT_OFF(CLUTCH_PORT, CLUTCH_PIN);
	}
}
	
enum CLUTCH GetCoupling(void)
{
	return CState;
}
