// #include <windows.h>
// #include "KEYBOARD.h"
// #include "CYCLE.h"
// #include "READ_WRITE_DATA.h"
// #include "resource.h"
// #include "REZAK.h"// только для hwnd
#include <stdint.h>
#include "KEYBOARD.h"
#include "rotary_encoder.h"
#include "Queue.h"
#include "DlgProc.h"
#include "PostMessage.h"
///////////////////////////////////////////////////////////////
//Режим Удаления
///////////////////////////////////////////////////////////////
void Key_RD(uint32_t l_Key)
{
    /*
int n;

  switch(l_Key)
  {
    case 20:                           //Del
      if (b_NoKeyP == 0)
      {
        CProg.Step[0] = 0x0000FFFF;       //очищаем поле crc
//        WriteCurProg(g_Numeric);      //Удаление программы !!!! продумать
//		g_Numeric = 0;
      }
      else
      {
		  if (CProg.NomAllStep > 1)
		  {
            for(n = CProg.NomStep; n < CProg.NomAllStep; n++)
              CProg.Step[n] = CProg.Step[n + 1];

            CProg.Step[CProg.NomAllStep] = 0x0000FFFF;

            if (CProg.NomStep == CProg.NomAllStep)
              CProg.NomStep--;

            CProg.NomAllStep--;

            WriteCurProg(&CProg);
//            g_Numeric = 0;
		  }
		  else
		  {
		    Error(4);//g_Error = 4;				//остался 1 шаг
		  }
      }
      
      g_Regim = 2;
      Key = Key_R2;
        break;
*/}
