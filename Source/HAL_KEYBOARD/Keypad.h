#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "Key.h"

//////////////////////
//#include "mc.h"
#include "stm32f10x_conf.h"
//////////////////////

#define ROWS 4
#define COLS 6
#define ALL_KEY ROWS*COLS

#define ROW_LOW(x)  GPIOB->BSRR = x;
#define ROW_HI(x)		GPIOB->BSRR = (x << 16);
#define COL_READ(x) ((GPIOB->IDR) & x)
#define HOLD_TIME  (int32_t)100   //time in mS for BOUNCE
#define UNDEF_TIME (int32_t)-1

typedef enum KeyPin { OPEN, CLOSE } KeyPin;

typedef struct
{
	OneKey kKey;		//Описание состояния клавиши
	int32_t kTimer;	//Текущая временная метка
	int32_t krow;		//Ряд клавиши
	int32_t kcol;		//Столбец клавиши
}KeyMap;

typedef struct
{
	int32_t kid;					//ID клавиши, для которой необходима регистрация отжатия
	int32_t needParam;		//ID посылаемый при отжатии клавиши
}KeyMapRel;

void KEYPAD_Init(void);
void ScanKeypad(void);
void UpdateKeypad(void);
void nextKeyState(KeyMap* kMap, KeyPin state); 

#endif /* KEYPAD_H */
