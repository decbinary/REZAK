#include <stdint.h>
#include "Queue.h"
#include "DlgProc.h"
#include "flick.h"
#include "zatl_stop.h"
#include "KEYBOARD.h"
#include "Keypad.h"
#include "Motion.h"
#include "CYCLE.h"
#include "Calibration.h"
#include "OUT_DO.h"
#include "OUT_DI.h"
#include "LED_DO.h"

int DlgProc(int hWnd, UINT uMsg, int wParam, int lParam)
{
	switch (uMsg) 
	{
		case WM_INITDIALOG:
			/*
			SetTimer(hWnd,1,1000,(TIMERPROC)DISPLAY);
      SetTimer(hWnd,3,5,(TIMERPROC)ENCODER);
			SetTimer(hWnd,4,2000,(TIMERPROC)FLICK);
			Key = Key_R0;                               //инициализация начальной функции обработки клавиш!!!!!!!!!! 
			g_hWnd = hWnd;
		*/
			ReadServise();		
			KEYPAD_Init();
			LED_Init();
			DO_Init();
			DI_Init();
			LINK_Init();
			STOP_Init();
			Key = Key_R0;                               //инициализация начальной функции обработки клавиш!!!!!!!!!! 		
			break;
		case WM_CLOSE:
			/*
			EndDialog(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			return TRUE;
		*/
			break;
		case WM_COMMAND:
//			switch( LOWORD( wParam ) )
			switch(wParam)		
			{
				case IDC_BUTTON_0:
					(*Key)(IDC_BUTTON_0);
					break;

				case IDC_BUTTON_1:
					(*Key)(IDC_BUTTON_1);
					break;

				case IDC_BUTTON_2:
					(*Key)(IDC_BUTTON_2);
					break;

				case IDC_BUTTON_3:
					(*Key)(IDC_BUTTON_3);
					break;

				case IDC_BUTTON_4:
					(*Key)(IDC_BUTTON_4);
					break;
					
				case IDC_BUTTON_5:
					(*Key)(IDC_BUTTON_5);
					break;

				case IDC_BUTTON_6:
					(*Key)(IDC_BUTTON_6);
					break;

				case IDC_BUTTON_7:
					(*Key)(IDC_BUTTON_7);
					break;

				case IDC_BUTTON_8:
					(*Key)(IDC_BUTTON_8);
					break;

				case IDC_BUTTON_9:
					(*Key)(IDC_BUTTON_9);
					break;

				case IDC_BUTTON_Pnt:
					(*Key)(IDC_BUTTON_Pnt);		//.
					break;

				case IDC_BUTTON_Ent:
					(*Key)(IDC_BUTTON_Ent);		//Enter
					break;

				case IDC_BUTTON_P:
					(*Key)(IDC_BUTTON_P);		//P
					break;

				case IDC_BUTTON_F:
					(*Key)(IDC_BUTTON_F);		// --->(F)
					break;
				
				case IDC_BUTTON_F_OFF:
					(*Key)(IDC_BUTTON_F_OFF);		//--->(F) (OFF)
					break;				

				case IDC_BUTTON_L:
					(*Key)(IDC_BUTTON_L);		//L
					break;

				case IDC_BUTTON_Lft:
					(*Key)(IDC_BUTTON_Lft);		//<-
					break;

				case IDC_BUTTON_Rht:
					(*Key)(IDC_BUTTON_Rht);		//->
					break;

				case IDC_BUTTON_Up:
					(*Key)(IDC_BUTTON_Up);		//^
					break;
				
				case IDC_BUTTON_Up_OFF:
					(*Key)(IDC_BUTTON_Up_OFF);		//^ (OFF)
					break;				

				case IDC_BUTTON_Ins:
					(*Key)(IDC_BUTTON_Ins);		//Ins
					break;

				case IDC_BUTTON_Del:
					(*Key)(IDC_BUTTON_Del);		//Del
					break;

				case IDC_BUTTON_o:
/*
                    if (b_Priv == 1)
                        b_Priv = 0;
                    else
                        b_Priv = 1;
*/
					(*Key)(IDC_BUTTON_o);		//(o)
					break;

				case IDC_BUTTON_Esc:
					(*Key)(IDC_BUTTON_Esc);		//Escape
					break;

				case IDC_BUTTON_Dn:
					(*Key)(IDC_BUTTON_Dn);		//_
					break;
				
				case IDC_BUTTON_Dn_OFF:
					(*Key)(IDC_BUTTON_Dn_OFF);		//_ (OFF)
					break;				
			}
			break;

		case WM_OPERATION:
//			switch( LOWORD( wParam ) )
			switch(wParam)		
			{
				case IDC_LINK:            //Переключение режима при привязке
					(*Key)(IDC_LINK);
					break;	
				
				case IDC_ON_LINK:            //Включение счета при наезде на концевик при привязке
					(*Key)(IDC_ON_LINK);
					break;

 				case IDC_RUN_BLOCK:            //Блокировка клавиатуры при движении
					(*Key)(IDC_RUN_BLOCK);
					break;

				case IDC_STP_UNLK:            //Разблокировка клавиатуры при стопе
					(*Key)(IDC_STP_UNLK);
					break;

 				case IDC_ERROR:
					(*Key)(IDC_ERROR);     //Режим ошибки
					break;

				case IDC_PROG_OUT:            //Корректный выход из режима программирования
					(*Key)(IDC_PROG_OUT);
					break;

				case IDC_COPY_PRG:            //Инициализация копии программы для редактирования
					(*Key)(IDC_COPY_PRG);
					break;

				case IDC_PRG_OUT:            //Выход из программирования программы
					(*Key)(IDC_PRG_OUT);
					break;

				case IDC_PROG_DEL:            //Удаление целой программы
					(*Key)(IDC_PROG_DEL);
					break;
				
				case IDC_KNIFE:            //Обработчик события рез совершен
					(*Key)(IDC_KNIFE);
					break;

				case IDC_USTR:            //Обработчик события рез совершен
					(*Key)(IDC_USTR);
					break;				
/*
				case IDC_EXT_INTER:
					EXTERN_INTER(hWnd);
					break;
								*/
			}
			break;
			
		case WM_MOTION:
			switch(wParam)		
			{		
				case IDC_RUN:
					xRunSend(lParam);
					break;
				case IDC_STP:
					xStpSend(STOP);
					break;
				case IDC_DN_HI:
					Direction(DnHSpd);
					break;					
				case IDC_DN_LO:
					Direction(DnLSpd);
					break;				
				case IDC_UP_HI:
					Direction(UpHSpd);
					break;				
				case IDC_UP_LO:
					Direction(UpLSpd);
					break;
				case IDC_STP_SPD:
					Direction(StpSpd);
					break;	
				case IDC_CLUTCH:
					ToggleCoupling();
					break;
			}
			break;
	}
return 0;
}
