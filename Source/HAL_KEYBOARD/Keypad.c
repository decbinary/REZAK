#include "Queue.h"
#include "Keypad.h"
#include "Keypadproto.h"
#include "PostMessage.h"
#include "DlgProc.h"
#include "main_proto.h"

const unsigned int keyROWS[ROWS] = {GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9};
const unsigned int keyCOLS[COLS] = {GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

KeyMap KeypadMap[] = {

{"->",	IDC_BUTTON_Rht, RELEASED, NO_CHANGE, UNDEF_TIME, 0, 0},
{"^",		IDC_BUTTON_Up, 	RELEASED, NO_CHANGE, UNDEF_TIME, 0, 1},
{"<-",	IDC_BUTTON_Lft, RELEASED, NO_CHANGE, UNDEF_TIME, 0, 2},
{"4",		IDC_BUTTON_4, 	RELEASED, NO_CHANGE, UNDEF_TIME, 0, 3},
{"5",		IDC_BUTTON_5, 	RELEASED, NO_CHANGE, UNDEF_TIME, 0, 4},
{"6",		IDC_BUTTON_6, 	RELEASED, NO_CHANGE, UNDEF_TIME, 0, 5},
{"F",		IDC_BUTTON_F, 	RELEASED, NO_CHANGE, UNDEF_TIME, 1, 0},
{"P",		IDC_BUTTON_P, 	RELEASED, NO_CHANGE, UNDEF_TIME, 1, 2},
{"7",		IDC_BUTTON_7, 	RELEASED, NO_CHANGE, UNDEF_TIME, 1, 3},
{"8",		IDC_BUTTON_8, 	RELEASED, NO_CHANGE, UNDEF_TIME, 1, 4},
{"9",		IDC_BUTTON_9, 	RELEASED, NO_CHANGE, UNDEF_TIME, 1, 5},
{"Del",	IDC_BUTTON_Del, RELEASED, NO_CHANGE, UNDEF_TIME, 2, 0},
{"(0)",	IDC_BUTTON_o, 	RELEASED, NO_CHANGE, UNDEF_TIME, 2, 1},
{"Ins",	IDC_BUTTON_Ins, RELEASED, NO_CHANGE, UNDEF_TIME, 2, 2},
{"1",		IDC_BUTTON_1, 	RELEASED, NO_CHANGE, UNDEF_TIME, 2, 3},
{"2",		IDC_BUTTON_2, 	RELEASED, NO_CHANGE, UNDEF_TIME, 2, 4},
{"3",		IDC_BUTTON_3, 	RELEASED, NO_CHANGE, UNDEF_TIME, 2, 5},
{"Esc",	IDC_BUTTON_Esc, RELEASED, NO_CHANGE, UNDEF_TIME, 3, 0},
{"_",		IDC_BUTTON_Dn, 	RELEASED, NO_CHANGE, UNDEF_TIME, 3, 1},
{"Ent",	IDC_BUTTON_Ent, RELEASED, NO_CHANGE, UNDEF_TIME, 3, 2},
{"0",		IDC_BUTTON_0, 	RELEASED, NO_CHANGE, UNDEF_TIME, 3, 3},
{".",		IDC_BUTTON_Pnt, RELEASED, NO_CHANGE, UNDEF_TIME, 3, 4}

};

KeyMapRel keyReleaseEv[] = {
{IDC_BUTTON_F, IDC_BUTTON_F_OFF},
{IDC_BUTTON_Up, IDC_BUTTON_Up_OFF},
{IDC_BUTTON_Dn, IDC_BUTTON_Dn_OFF}
};
#define KEY_REL_EV sizeof(keyReleaseEv)/sizeof(keyReleaseEv[0])

/**
  * @brief  Инициализация клавиатуры.
  * @param  None
  * @retval None
  */
void KEYPAD_Init(void)
{
	int i;
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = 0;
	for (i = 0; i < (sizeof(keyROWS)/sizeof(keyROWS[0])); i++)
	{
		GPIO_InitStructure.GPIO_Pin |= keyROWS[i];
	}
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	ROW_HI(GPIO_InitStructure.GPIO_Pin)
	
	GPIO_InitStructure.GPIO_Pin = 0;
	for (i = 0; i < (sizeof(keyCOLS)/sizeof(keyCOLS[0])); i++)
	{
		GPIO_InitStructure.GPIO_Pin |= keyCOLS[i];
	}
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  Сканирование клавиатуры.
  * @param  None
  * @retval None
  */
void ScanKeypad(void)
{
int n, k;

  for (n = 0; n < ROWS; n++)
  {
    ROW_LOW(keyROWS[n])

    k = 0;
    while (KeypadMap[k].krow != n && k < ALL_KEY)
    {
       k++;
    }
		
    for (; k < ALL_KEY; k++)
    {
      if (KeypadMap[k].krow != n)
          break;

      nextKeyState(&KeypadMap[k], ((COL_READ(keyCOLS[KeypadMap[k].kcol]) != 0) ? OPEN : CLOSE));
    }
		ROW_HI(keyROWS[n])
  }
}
/**
  * @brief  Перевод клавиши в другое состояние.
  * @param  KeyMap* - Указатель на массив описателей клавиш
						KeyPin - Состояние клавиши
  * @retval None
  */
void nextKeyState(KeyMap* KeypadMap, KeyPin state)
{
	switch (KeypadMap->kKey.kstate) 
	{
		case RELEASED:                                      //Ожидание нажатия
			if (state == CLOSE) 
			{
				KeypadMap->kKey.kstate = PRESSED;
				KeypadMap->kTimer = g_Tick; 
			}		// Get ready for next HOLD state.
			break;
		case PRESSED:                                       //Ожидание окончания дребезга
			if ((g_Tick - KeypadMap->kTimer) > HOLD_TIME)	// Waiting for a key HOLD...
			{
				KeypadMap->kKey.kstate = HOLD;
				KeypadMap->kKey.kchange = CHANGED;
				KeypadMap->kTimer = UNDEF_TIME;
			}
			else 
			if (state == OPEN)				// or for a key to be RELEASED.
			{
				KeypadMap->kKey.kstate = RELEASED;
				KeypadMap->kTimer = UNDEF_TIME;
			}
			break;
		case HOLD:                                          //Ожидание отжатия
			if (state == OPEN) 
			{
				KeypadMap->kKey.kstate = IDLE;
				KeypadMap->kTimer = g_Tick; 
			}		// Get ready for next HOLD state.
			break;
		case IDLE:                                         //Ожидание окончания дребезга
			if ((g_Tick - KeypadMap->kTimer) > HOLD_TIME)	// Waiting for a key RELEASED...
			{
				KeypadMap->kKey.kstate = RELEASED;
				KeypadMap->kKey.kchange = CHANGED;
				KeypadMap->kTimer = UNDEF_TIME;
			}
			else 
			if (state == CLOSE)				// or for a key to be RELEASED.
			{
				KeypadMap->kKey.kstate = HOLD;
				KeypadMap->kTimer = UNDEF_TIME;
			}
			break;
	}
}

/**
  * @brief  Обновление данных о состоянии кнопок.
  * @param  None
  * @retval None
  */
void UpdateKeypad(void)
{
int n,k;

	for (n = 0; n < ALL_KEY; n++)
	{
		if (KeypadMap[n].kKey.kchange == CHANGED)
		{
        if (KeypadMap[n].kKey.kstate == HOLD)
        {
          PostMessage(0, WM_COMMAND, KeypadMap[n].kKey.kid, 0);
          KeypadMap[n].kKey.kchange = NO_CHANGE;        
        }
        else
        if(KeypadMap[n].kKey.kstate == RELEASED)
        {
            for (k = 0; k < KEY_REL_EV; k++)
            {
                if (KeypadMap[n].kKey.kid == keyReleaseEv[k].kid)
                {
                    PostMessage(0, WM_COMMAND, keyReleaseEv[k].needParam, 0);
                    break;
                }
            }
            KeypadMap[n].kKey.kchange = NO_CHANGE;
        }
		}
	}
}
