#include <stdint.h>
#include "stm32f10x_conf.h"
#include "rotary_encoder.h"
#include "Calibration.h"
#include "Queue.h"
#include "PostMessage.h"
#include "DlgProc.h"

#define b_Link GPIO_Pin_7

void LINK_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = b_Link;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Calibration(uint16_t port, uint16_t enPin)
{
	if (enPin == b_A_Pin)
	{
		if ((MLink == NoLink) && ((port & b_Link) == b_Link) && (((port & b_A_Pin) == b_A_Pin) != ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = OnLink;
//			PostMessage(0, WM_MOTION, IDC_STP, 0);
			PostMessage(0, WM_OPERATION, IDC_ON_LINK, 0);
		}
		else
		if ((MLink == OnLink) && ((port & b_Link) == b_Link) && (((port & b_A_Pin) == b_A_Pin) == ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = FinalLink;
		}
		if ((MLink == FinalLink) && ((port & b_Link) == 0) && (((port & b_A_Pin) == b_A_Pin) != ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = OnLink;
		}		
		else
		if ((MLink == FinalLink) && ((port & b_Link) == 0) && (((port & b_A_Pin) == b_A_Pin) == ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = YsLink;
			PostMessage(0, WM_OPERATION, IDC_LINK, 0);			
		}
	}
	else
	if (enPin == b_B_Pin)
	{
		if ((MLink == NoLink) && ((port & b_Link) == b_Link) && (((port & b_A_Pin) == b_A_Pin) == ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = OnLink;
//			PostMessage(0, WM_MOTION, IDC_STP, 0);
			PostMessage(0, WM_OPERATION, IDC_ON_LINK, 0);			
		}
		else
		if ((MLink == OnLink) && ((port & b_Link) == b_Link) && (((port & b_A_Pin) == b_A_Pin) != ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = FinalLink;
		}
		if ((MLink == FinalLink) && ((port & b_Link) == 0) && (((port & b_A_Pin) == b_A_Pin) == ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = OnLink;
		}
		else		
		if ((MLink == FinalLink) && ((port & b_Link) == 0) && (((port & b_A_Pin) == b_A_Pin) != ((port & b_B_Pin) == b_B_Pin)))
		{
			MLink = YsLink;
			PostMessage(0, WM_OPERATION, IDC_LINK, 0);				
		}
	}
}
