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
//Запись в структуры дисплея
///////////////////////////////////////////////////////////////
void toDisplay(struct CProgS* CProg, struct CInputS* CInput, struct CInputFlickS* CInputFlick)
{
	CDisp.Required = CProg->Step[CProg->NomStep];
	if (CProg->NomAllStep == 0)
		CDisp.Required = 0;
//    if (CInputFlick->type == fStep)
//      CDisp.Required = CInput->Numeric;     //????   в режиме программирования

	CDisp.CurProg = CProg->CurProg;
	CDisp.NomStep = CProg->NomStep;
	CDisp.Numeric = CInput->Numeric;
	CDisp.Error   = g_Error;
	CDisp.Regim   = g_Regim;
	CDisp.Point   = CInput->Point;
	CDisp.NoKeyP  = b_NoKeyP;

    /*
    CDisp.CDispFlick.fCurProg = CInputFlick->fCurProg;
    CDisp.CDispFlick.fNomStep = CInputFlick->fNomStep;
    CDisp.CDispFlick.fStep = CInputFlick->fStep;
    CDisp.CDispFlick.timefCurProg = CInputFlick->timefCurProg;
    CDisp.CDispFlick.timefNomStep = CInputFlick->timefNomStep;
    CDisp.CDispFlick.timefStep = CInputFlick->timefStep;
    */
}
