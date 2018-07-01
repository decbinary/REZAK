#include "OUT_DI.h"
#include "stm32f10x_conf.h"
#include "Queue.h"
#include "PostMessage.h"
#include "DlgProc.h"

void DI_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = Knf_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(Knf_PORT, &GPIO_InitStructure);
}

void SetKnife(void)
{
	static uint16_t tmp;
	static KNIFE KState = KnfUp;

	tmp = (uint16_t) GPIOC->IDR;	//Чтение порта С
	
	if(((tmp & Knf_PIN) == Knf_PIN) && KState == KnfUp)
	{
		KState = KnfDn;
	}
	else
	if(((tmp & Knf_PIN) != Knf_PIN) && KState == KnfDn)
	{
		KState = KnfRtn;
	}
	
	if(KState == KnfRtn)
	{
		KState = KnfUp;
		PostMessage(0, WM_OPERATION, IDC_KNIFE, 0);
	}
}
