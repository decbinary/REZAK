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
//Обработка цифровых клавиш
///////////////////////////////////////////////////////////////
int Number (unsigned char l_Key, struct CInputS* CInput)
{
  if(CInput->Point == 0)
  {
    if (CInput->Count > NUM_TO_POINT)
      init_CInput((int)l_Key, 0, 0);    //если введено более 4 цифр сбросить счетчик
    else
    if (CInput->Count == NUM_TO_POINT)             //если введено 4 цифр добавить точку
      init_CInput(CInput->Numeric + (int)l_Key, CInput->Count, 1);
    else
    if (CInput->Count < NUM_TO_POINT)              //если введено менее 4 цифр
      init_CInput(CInput->Numeric + (int)l_Key, CInput->Count, 0);
  }
  else
  {
    if (CInput->Count > NUM_TO_POINT)
      init_CInput((int)l_Key, 0, 0);
    else
    if (CInput->Count <= NUM_TO_POINT)
      init_CInput(CInput->Numeric + (int)l_Key, CInput->Count, 1);
  }

  if (CInput->Numeric != 0)
    CInput->Count++;

  return 0;
}
