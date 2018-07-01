#include <stdint.h>
#include "display.h"
#include "Queue.h"
#include "InitHal.h"
#include "string.h"
#include "texterror.h"
#include "flick.h"
#include "KEYBOARD.h"
#include "tm_stm32_hd44780.h"
#include "LED_DO.h"
#include "OUT_DO.h"
#include "w_strcpy.h"

#include <stdio.h>
/**
 * Формирование строк для индикатора,
 * предача данных для вывода на LCD
 * @return 0-ОК
 */
int Display_run (void)
{
char LCD1[LCD_COLUMN + 1] = {0};
char LCD2[LCD_COLUMN + 1] = {0};
char buffer[LCD_COLUMN + 1];

int32_t n;

/////////////////////////////////////////////////////////////////////////
///  Отображаем запись в LCD
/////////////////////////////////////////////////////////////////////////
	if (CDisp.Regim == 0)
	{		
		w_strcpy(LCD1,"Калибровка.");
	}
	else
	if (CDisp.Regim == 1)  //Ручное управление
	{
		w_strcpy(LCD1,"Ручное упр.");
		n = sprintf(LCD2, "%d", CDisp.Numeric);
//		n = sprintf(LCD2, "%d", g_Active);		

		if (CDisp.Point == 1)
		{
			if (n == 1)
			{
				LCD2[3] = 0;
				LCD2[2] = LCD2[0];
//				if ('0' == (LCD2[2] = LCD2[0]))
//					LCD2[2] = 0;
				LCD2[1] = '.';
				LCD2[0] = '0';
			}
			else
			{
				LCD2[n] = LCD2[n - 1];
				LCD2[n - 1] = '.';
				LCD2[n + 1] = 0;
			}
		}
		else
		{
			if (n > 1)
				LCD2[n - 1] = 0;
		}
  }
	else
	if (CDisp.Regim == 2 || CDisp.Regim == 3 || CDisp.Regim == 5 || CDisp.Regim == 6)
	{
		if (CDisp.NoKeyP == 0)
			w_strcpy(LCD1,"Выбор прогр. ");
		
		n = sprintf(buffer, "%d", CDisp.Numeric);			// отображение Numeric
		if (CDisp.Point == 1)
		{
			if (n == 1)
			{
				buffer[3] = 0;
				buffer[2] = buffer[0];
//				if ('0' == (buffer[2] = buffer[0]))
//					buffer[2] = 0;
				buffer[1] = '.';
				buffer[0] = '0';
			}
			else
			{
				buffer[n] = buffer[n - 1];
				buffer[n - 1] = '.';
				buffer[n + 1] = 0;
			
//				if (buffer[n] == '0')
//					buffer[n] = 0;
			}
		}
		else
		{
			if (n > 1)
				buffer[n - 1] = 0;
		}
		strcat(LCD1, buffer);
		n = sprintf(LCD2, "P%02d N%02d V%d", CDisp.CurProg, CDisp.NomStep, CDisp.Required); //Отображение строки

		if (n < 11)				//отображение незначущего нуля
		{
			LCD2[n + 2] = 0;
			LCD2[n + 1] = LCD2[n - 1];
			LCD2[n] = '.';
			LCD2[n - 1] = '0';
		}
		else
		{
			LCD2[n] = LCD2[n - 1];
			LCD2[n - 1] = '.';
			LCD2[n + 1] = 0;
		}
	}
	else
	if (CDisp.Regim == 4)
	{
		if (CDisp.NoKeyP == 1)
		{
			w_strcpy(LCD1,"Выполн. ");
			w_strcpy(LCD2,"Удалить № N:");
			sprintf(buffer, "%d", CDisp.NomStep);
			strcat(LCD2, buffer);
		}
		else
		{
			w_strcpy(LCD1,"Выполн. № P:");
			w_strcpy(LCD2,"Удалить № P:");
			sprintf(buffer, "%d", CDisp.Numeric);
			strcat(LCD2, buffer);
		}
	}
/////////////////////////////////////////////////////////////////////////
///  Светим индикаторы
/////////////////////////////////////////////////////////////////////////
	ChekLed(CDisp.Regim);
	toIndicate(LCD1, LCD2);

return 0;
}

/**
 * Засветка светодиодов на клавиатуре,
 * которые соответствуют режиму работы.
 * Засветка светодиода муфты.
 * @param Regim - значение режима 
 * @return 0-ОК
 */
void ChekLed(uint32_t Regim)
{
	switch (Regim)
	{
		case 0:
		case 1:
			CheckDlgButton(0, IDC_CHECK1, BST_UNCHECKED);
			CheckDlgButton(0, IDC_CHECK2, BST_UNCHECKED);
			break;
		case 2:
			CheckDlgButton(0, IDC_CHECK1, BST_CHECKED);
			CheckDlgButton(0, IDC_CHECK2, BST_UNCHECKED);
			break;
		case 3:
		case 5:
		case 6:
			CheckDlgButton(0, IDC_CHECK1, BST_CHECKED);
			CheckDlgButton(0, IDC_CHECK2, BST_CHECKED);
			break;
		default:
			CheckDlgButton(0, IDC_CHECK1, BST_UNCHECKED);
			CheckDlgButton(0, IDC_CHECK2, BST_UNCHECKED);
			break;
	}
	
	if (GetCoupling() == CLUTCH_ON)
	{
		CheckDlgButton(0, IDC_CHECK0, BST_CHECKED);
	}
	else
	if (GetCoupling() == CLUTCH_OFF)
	{
		CheckDlgButton(0, IDC_CHECK0, BST_UNCHECKED);
	}	
}

/**
 * Обработка мигания,
 * вывод на LCD
 * @param LCD1, LCD2 - указатели на строки,
 * которые необходимо вывести
 * @return
 */
void toIndicate(char* LCD1, char* LCD2)
{
	static char LCD1_TMP[LCD_COLUMN + 1] = {0};
	static char LCD2_TMP[LCD_COLUMN + 1] = {0};
	static enum flick type = fNoFlick;
	int32_t n;

	if (GetSetInitFlick(Get, 1) != CInputFlick.type)
		GetSetInitFlick(Init, 0);

	type = GetSetInitFlick(Get, 0);

	switch (type)
	{
		case fCurProg:
			LCD2[1] = LCD2[2] = ' ';
			break;
		case fNomStep:
			LCD2[5] = LCD2[6] = ' ';
			break;
		case fStep:
			LCD2[9] = 0;
			break;
		default:
			break;
	}

	TextError(LCD2,CDisp.Error);
//	TextError(LCD2,5);	
//	SetDlgItemText(g_hWnd,IDC_STATIC2,LCD1);
//	SetDlgItemText(g_hWnd,IDC_STATIC3,LCD2);
//	TM_HD44780_Clear();
	
	for (n = 0; n < LCD_COLUMN; n++)
		LCD1_TMP[n] = ' ';							//заполняем пробелами
	LCD1_TMP[LCD_COLUMN] = 0;
	n = 0;
	while (LCD1[n] != 0)
	{
		LCD1_TMP[n] = LCD1[n];					//копируем текущую строку
		n++;
	}
	LCD1[n] = ' ';										//убираем символ конца строки, для полного вывода строки
	
	for (n = 0; n < LCD_COLUMN; n++)
		LCD2_TMP[n] = ' ';							//заполняем пробелами
	LCD2_TMP[LCD_COLUMN] = 0;
	n = 0;
	while (LCD2[n] != 0)
	{
		LCD2_TMP[n] = LCD2[n];					//копируем текущую строку
		n++;
	}
	LCD2[n] = ' ';										//убираем символ конца строки, для полного вывода строки
	
	TM_HD44780_Puts(0, 0, LCD1_TMP);
	TM_HD44780_Puts(0, 1, LCD2_TMP);
}
