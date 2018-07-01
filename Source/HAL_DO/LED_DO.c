#include <stdint.h>
#include "stm32f10x_conf.h"
#include "LED_DO.h"

#define LED_OFF(port, pin)			GPIO_ResetBits(port, pin)
#define LED_ON(port, pin)			GPIO_SetBits(port, pin)

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
}

void CheckDlgButton(uint32_t g_hWnd, enum OUT_LED OutDO, enum STATE state)
{
  switch (OutDO)
	{
		case IDC_CHECK0:
			if (state == BST_CHECKED)
			{
				LED_ON(LED3_PORT, LED3_PIN);
			}
			else
			if (state == BST_UNCHECKED)
			{
				LED_OFF(LED3_PORT, LED3_PIN);
			}
			break;
		case IDC_CHECK1:
			if (state == BST_CHECKED)
			{
				LED_ON(LED1_PORT, LED1_PIN);
			}
			else
			if (state == BST_UNCHECKED)
			{
				LED_OFF(LED1_PORT, LED1_PIN);
			}
			break;
		case IDC_CHECK2:
			if (state == BST_CHECKED)
			{
				LED_ON(LED2_PORT, LED2_PIN);
			}
			else
			if (state == BST_UNCHECKED)
			{
				LED_OFF(LED2_PORT, LED2_PIN);
			}
			break;			
	}
}
