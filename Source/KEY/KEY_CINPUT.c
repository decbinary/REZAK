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
//Инлайн инициализация вводимого значения
//int - если число без точки, записиваем в CInput.NumericInt
//если число с точкой, записиваем в CInput.Numeric
///////////////////////////////////////////////////////////////
/*inline*/ void init_CInput (int Numeric, BOOL Count, BOOL Point)
{
  if (Point == 1)
  {
      CInput.NumericInt = -1;   //если введено число с точкой, то храним особое значение
      CInput.Numeric = Numeric; //Numeric умножено на 10 для хранения точки
  }
  else
  {
      CInput.NumericInt = Numeric;
      CInput.Numeric = Numeric * 10;
  }
  CInput.Count = Count;
  CInput.Point = Point;
}

///////////////////////////////////////////////////////////////
//Инлайн инициализация мигания значения (предположительно в мС)
///////////////////////////////////////////////////////////////
/*inline*/ void init_CInputFlick (enum flick type, int timeflick)
{
    /*
    CInputFlick.fCurProg = fCurProg;
    CInputFlick.timefCurProg = timefCurProg;
    CInputFlick.fNomStep = fNomStep;
    CInputFlick.timefNomStep = timefNomStep;
    CInputFlick.fStep = fStep;
    CInputFlick.timefStep = timefStep;
*/
/*
char TMP[10] = {0};
sprintf(TMP, " %d+set ", type);
OutputDebugStringA(TMP);
*/
    CInputFlick.type = type;
    CInputFlick.timeflick = timeflick;
}
