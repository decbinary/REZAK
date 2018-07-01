#include "stm32f10x_conf.h"
#include "flick.h"
#include "KEYBOARD.h"
#include "Queue.h"
#include "PostMessage.h"
#include "DlgProc.h"

void Flick_run (void)
{
	GetSetInitFlick(Set, 0);
	GetSetInitError(Set, 0);
}

/**
 * Учтановка, проверка, инициализация мигания
 * @param type - тип обращения,
 * @param timeorstate - устанавливает необходимое время,
 * 											либо состояние мигания
 * @return flick - состояние мигания
 */
enum flick GetSetInitFlick(enum getset type, int32_t timeorstate)
{
    static enum flick newenum = fNoFlick;
    static int32_t count = 0;       //общее время мигания
    static int32_t state = 0;       //состояние мигания
		static int32_t tmp = 0;					//предделитель мигания

    if (type == Get)
    {
        if (timeorstate == 1)
        {
          return newenum;
        }
        else
        {
            if (state == 0)
                return fNoFlick;
            else
                return newenum;
        }
    }
    else
    if (type == Set)
    {
        if (count == 0)
        {
            state = 0;        
        }
        else
        if (count > 0 || count == -1)
        {
            if (state == 0 && ++tmp >= FLICK_PERIOD)
                state = 1;
            else
						if (state == 1 && --tmp <= 0)
                state = 0;
        }

        if (count > 0)
        {
            if (--count == 0)
            {
                CInputFlick.type = newenum = fNoFlick;            
            }
        }
    }
    else
    if (type == Init)
    {
      count = CInputFlick.timeflick;
      newenum = CInputFlick.type;
      state = 1;              //возможно синхронизация таймера
			tmp = FLICK_PERIOD;
    }      
    return fNoFlick;
}

/**
 * Учтановка, проверка, инициализация ошибки
 * @param type - тип обращения,
 * @param timeorstate - устанавливает необходимое время мигания ошибки,
 * 											либо состояние ошибки
 * @return state - состояние мигания
 */
int GetSetInitError(enum getset type, int32_t timeorstate)
{
    static int32_t count = 0;
    static int32_t state = 0;       //состояние мигания
    
    if (type == Get)
    {
      if (state == 0)
        return 0;
      else
        return timeorstate;
    }
    else
    if (type == Set)
    {
        if (count == 0)
        {
            state = 0;        
        }
        else
        if (count > 0)
        {
            state = 1;					

            if (--count == 0)
            {
//							PostMessage(g_hWnd, WM_COMMAND, IDC_BUTTON22, 0);
							PostMessage(0, WM_COMMAND, IDC_BUTTON_Esc, 0);							
            }
        }
    }
    else
    if (type == Init)
    {
      count = timeorstate;

      if (count == 0)
          state = 0;
      else
          state = 1;              //возможно синхронизация таймера
    }
    return state;
}
