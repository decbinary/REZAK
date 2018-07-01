#include <stdint.h>
#include "texterror.h"
#include "flick.h"
#include "KEYBOARD.h"
#include "w_strcpy.h"

/**
 * Запись текста ошибки в строку
 * @param Mas - указатели на строку,
 * @param Nom - номер ошибки
 * @return error = 0 строка погашена
 *         error = Nom номер ошибки (при этом
 * 								 записуется текст ошибки в строку)
 */
int TextError(char* Mas, unsigned int Nom)
{
static int error = 0;

	if (error != Nom)
	{
		if (Nom == 0)
			GetSetInitError(Init, 0);
		else
			GetSetInitError(Init, FLICK_ERROR);
	}

	error = GetSetInitError(Get, Nom);
	if (error != 0)
	{
		switch(Nom)
		{
			case 1:
				w_strcpy(Mas,"Ошиб. 1");  //неправильный размер
				break;
			case 2:
				w_strcpy(Mas,"Ошиб. 2");  //неправильный номер шага
				break;
			case 3:
				w_strcpy(Mas,"Ошиб. 3");  //исчерпано количество шагов
				break;
			case 4:
				w_strcpy(Mas,"Ошиб. 4");  //остался 1 шаг при удалении шага
				break;
			case 5:
				w_strcpy(Mas,"Ошиб. 5");  //программа повреждена
				break;
			case 10:
				w_strcpy(Mas,"Ошиб. 10");  //прошиб размер
				break;
			case 11:
				w_strcpy(Mas,"Ошиб. 11");  //попытка отработать неправильный размер
				break;
			default:
				w_strcpy(Mas,"Ошиб.");
				break;
		}
	}
	return error;
}
