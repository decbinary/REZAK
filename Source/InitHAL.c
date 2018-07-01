/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "tm_stm32_hd44780.h"
#include "rotary_encoder.h"
#include "spi_ee.h"
#include "7seg.h"
#include "Queue.h"

#include "InitHAL.h"

QueueArray KeypadQueue;

void InitHAL(void)
{
#ifdef LOCK
	// Проверяем установлена или сброшена защита
  if (FLASH_GetReadOutProtectionStatus() == RESET)
  {
		FLASH_Unlock();                  // Разрешаем запись в память
    FLASH_ReadOutProtection(ENABLE); // Защищаемся от считывания прошивки
    FLASH_Lock();                    // Запрещаем запись в память
  }
#endif	
	
	OscillatorInit();
	RE_Init();
	SEG_Init();
	TM_HD44780_Init(LCD_COLUMN, LCD_ROW);
	sEE_Init();
	initQueueArray(&KeypadQueue); 
}

void OscillatorInit(void)
{
	RCC_DeInit(); //	сброс настроек тактового генератора
	RCC_HSEConfig(RCC_HSE_OFF); //	отключение внешнего тактового генератора
	
	// Enable Prefetch Buffer
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // включаем буферизацию для флеша

	//***************************************************************
	//   HSI=8MHz, HCLK=36MHz, HCLK=36MHz
	//***************************************************************
	// Flash 2 wait state
	FLASH_SetLatency(FLASH_Latency_2);
	// HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
	// Enable PLL
	RCC_PLLCmd(ENABLE);

	// Wait till PLL is ready
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}

	// Select PLL as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// Wait till PLL is used as system clock source
	while (RCC_GetSYSCLKSource() != 0x08);

	SystemCoreClockUpdate();
		
	if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}
