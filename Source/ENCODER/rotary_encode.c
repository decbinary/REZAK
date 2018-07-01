#include "stm32f10x_conf.h"
#include "rotary_encoder.h"
#include "Motion.h"
#include "Calibration.h"
#include "KEYBOARD.h"

int32_t g_Active = 0;

enum LINK MLink = NoLink;

void RE_Init() 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = b_A_Pin | b_B_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(b_Port, &GPIO_InitStructure);	

  /* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI2 and EXTI3 Line to PC.02 and PC.03 pin */
	GPIO_EXTILineConfig(b_PortSourse, b_A_PinSourse);
  GPIO_EXTILineConfig(b_PortSourse, b_B_PinSourse);	

  /* Configure EXTI2 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line2 | EXTI_Line3);

  /* Enable and set EXTI2 and EXTI3 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;	
  NVIC_Init(&NVIC_InitStructure);	
}

void EXTI2_IRQHandler(void)
{ 
	static uint16_t tmp;

	tmp = (uint16_t) GPIOC->IDR;	//Чтение порта С
	EXTI->PR = EXTI_Line2;				//Сброс прерывания

	if(CUstDef.IncA > 0)
	{	
		if (MLink != NoLink)
		{
			if ((tmp & b_A_Pin) == b_A_Pin)
			{
				if ((tmp & b_B_Pin) == b_B_Pin)
				{
					g_Active -= CUstDef.IncA;
				}
				else
				{
					g_Active += CUstDef.IncA;
				}
			}
			else
			{
				if ((tmp & b_B_Pin) == b_B_Pin)
				{
					g_Active += CUstDef.IncA;
				}
				else
				{
					g_Active -= CUstDef.IncA;
				}
			}
			Motion();
		}
		
		if (MLink != YsLink)
		{
			Calibration(tmp, b_A_Pin);
		}
	}	
}

void EXTI3_IRQHandler(void)
{
	static uint16_t tmp;

	tmp = (uint16_t) GPIOC->IDR;  //Чтение порта С
	EXTI->PR = EXTI_Line3;				//Сброс прерывания

	if(CUstDef.IncB > 0)
	{	
		if (MLink != NoLink)
		{	
			if ((tmp & b_B_Pin) == b_B_Pin)
			{
				if ((tmp & b_A_Pin) == b_A_Pin)
				{
					g_Active += CUstDef.IncB;
				}
				else
				{
					g_Active -= CUstDef.IncB;
				}
			}
			else
			{
				if ((tmp & b_A_Pin) == b_A_Pin)
				{
					g_Active -= CUstDef.IncB;
				}
				else
				{
					g_Active += CUstDef.IncB;
				}
			}
			Motion();
		}
		
		if (MLink != YsLink)
		{
			Calibration(tmp, b_B_Pin);
		}
	}		
}
