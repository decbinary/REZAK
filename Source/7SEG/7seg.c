// -- драйвер динамической индикации ИТП14_16 ------
#include "stm32f10x_conf.h"
#include "7seg.h"
#include "ASCIITable7.h"
#include <string.h>
#include "KEYBOARD.h"

extern int32_t g_Active;

#define MASK_SEGM (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 )
#define MASK_INDIK  (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12)

const uint16_t msk_ind[AM_IND] = {GPIO_Pin_12, GPIO_Pin_9, GPIO_Pin_8, GPIO_Pin_11, GPIO_Pin_10};

struct CPreciseS CPrecise = CPRECISE_INIT;

/**
 * Инициализация драйвера динамической индикации и кнопок
 * @return 0-ОК
 */
void SEG_Init()
{

  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO Ports Clock Enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	
	//----------------- сегменты индикатора ----------------------------
  GPIO_InitStructure.GPIO_Pin = MASK_SEGM; 

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//-----------------  индикаторы ----------------------------
  GPIO_InitStructure.GPIO_Pin = MASK_INDIK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  //отключаем все индикаторы
  GPIOA->BSRR = (((uint32_t)MASK_INDIK) << 16);
  //отключаем все сепменты
  GPIOC->BSRR = (((uint32_t)MASK_SEGM) << 16);	
}

/**
 * Такт драйвера индикации ~1 мс
 */
void SEG_run(void)
{
    uint16_t ind[AM_IND];
	  static uint8_t i = 0;

    //отключаем все индикаторы
    GPIOA->BSRR = (((uint32_t)MASK_INDIK) << 16);
    //отключаем все сепменты
    GPIOC->BSRR = (((uint32_t)MASK_SEGM) << 16);

		if ((g_Active > CPrecise.FActive && g_Active - CPrecise.FActive > CUstDef.Acur) ||
				(g_Active < CPrecise.FActive && CPrecise.FActive - g_Active > CUstDef.Acur))
		{
				CPrecise.Show = 0;
		}
		
		if (CPrecise.Show == 0)
			StrToImg(ind, ACTIVE_10);
		else
			StrToImg(ind, CPrecise.FActive/10);
	
    //текущий индикатор (0,1,2,3,4,0...)
    i++;
    if (i >= AM_IND)
    {
        i = 0;
    }

    GPIOC->BSRR = ind[i]; //включаем сегменты
    GPIOA->BSRR = msk_ind[i]; //включаем текущий индикатор
}

/**
 * Преобразование строки в образ 4-х разрядного индикатора в соответствии с ASCIITable7.h
 * @param pStr  - указатель на строку с нулем в конце
 * @param Blink  - маска мигания
 * объединяет точки с символами
 * первый символ выводится на левый индикатор
 * @return 0-ОК
 */
void StrToImg(uint16_t* ind, int32_t l_active)
{

		uint8_t i;
		for (i = 0; i < AM_IND; i++)
			ind[i] = 0;
	
		if (l_active <= 0)										//ничего не отображать, если не было привязки
			return;
	
    for (i = 0; i < AM_IND; i++)
    {
			if (l_active > 0)
			{
        ind[AM_IND - 1 - i] = Tbl[l_active % 10];
  			l_active /= 10;
			}
			else
			{
				if (i < 2)
					ind[AM_IND - 1 - i] = Tbl[0];		//оставить ноль до точки
			}
    }
		ind[AM_IND - 2] |= 0x8000; //совмещение точки
    return;
}

