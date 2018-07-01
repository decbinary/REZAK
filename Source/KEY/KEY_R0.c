//#include <windows.h>
#include <stdint.h>
#include "KEYBOARD.h"
#include "rotary_encoder.h"
#include "Queue.h"
#include "DlgProc.h"
#include "PostMessage.h"
//#include "CYCLE.h"
#include "READ_WRITE_DATA.h"
//#include "resource.h"
//#include "REZAK.h"// только для hwnd

void (*Key)(uint32_t);

struct CDispS CDisp = {0};
struct CProgS CProg = CPROG_INIT;
struct CUstDefS CUstDef = CUSTDEF_INIT;
struct CUstDefS CUstDefInit = CUSTDEF_INIT;
struct CInputS CInput = CINPUT_INIT;
struct CInputFlickS CInputFlick = CINPUTFLICK_INIT;

unsigned int g_Regim = 0;     //Калибровка=0/Ручное=1/Автомат=2/Модификация=3/Удаление=4/РедактированиеШага=5/НоваяПрограмма=6
unsigned int g_Error = 0;

unsigned int g_OffMotion = 0;	// Флаг разрешения остановки после отжатия клавиш ^ _

BOOL b_NoKeyP = 1;          //Выставлен ли номер программы
///////////////////////////////////////////////////////////////
//Режим чтения юстировочных коэффициентов
///////////////////////////////////////////////////////////////
void ReadServise(void)
{
	int tmp;
	tmp = ReadCurProg(&CProg, 0);

	if (tmp == 1 || CProg.NomAllStep != sizeof(struct CUstDefS)/sizeof(int))
	{
		CProg.Step[1]		= (unsigned int)(CUstDef.Mark = CUstDefInit.Mark)/10;					//значение размера при привязке
		CProg.Step[2]		= (unsigned int)(CUstDef.MaxSizeD10 = CUstDefInit.MaxSizeD10); //максимальный размер
		CProg.Step[3]		= (unsigned int)(CUstDef.MinSizeD10 = CUstDefInit.MinSizeD10); //минимальный размер
		CProg.Step[4]		= (unsigned int)(CUstDef.DnSpd = CUstDefInit.DnSpd)/10;					//расстояние перехода на малую скорость
		CProg.Step[5]		= (unsigned int)(CUstDef.DnDist = CUstDefInit.DnDist)/10;			//расстояние перехода с малой на большую от текущей позиции
		CProg.Step[6]		= (unsigned int)(CUstDef.Stop = CUstDefInit.Stop)/10;					//расстояние остановки
		CProg.Step[7]		= (unsigned int)(CUstDef.Cross = CUstDefInit.Cross)/10;				//расстояние заезда за точку
		CProg.Step[8]	= (unsigned int)(CUstDef.Acur = CUstDefInit.Acur)/10;						//точность
		CProg.Step[9]	= (unsigned int)(CUstDef.TimeStop = CUstDefInit.TimeStop)*10;		//время после последнего импульса, которое считается остановкой
		CProg.Step[10]	= (unsigned int)(CUstDef.IncA = CUstDefInit.IncA)*10;					//коэффициенты для энкодера
		CProg.Step[11]	= (unsigned int)(CUstDef.IncB = CUstDefInit.IncB)*10;					//коэффициенты для энкодера
		CProg.NomAllStep = sizeof(struct CUstDefS)/sizeof(int);
		CProg.CurProg = 0;
		CProg.NomStep = 1;
		WriteCurProg(&CProg);
	}
	else
	{
		CUstDef.Mark = (int)(10*CProg.Step[1]);
		CUstDef.MaxSizeD10 = (int)(CProg.Step[2]);
		CUstDef.MinSizeD10 = (int)(CProg.Step[3]);
		CUstDef.DnSpd = (int)(10*CProg.Step[4]);		
		CUstDef.DnDist = (int)(10*CProg.Step[5]);
		CUstDef.Stop = (int)(10*CProg.Step[6]);	
		CUstDef.Cross = (int)(10*CProg.Step[7]);		
		CUstDef.Acur = (int)(10*CProg.Step[8]);		
		CUstDef.TimeStop = (int)(CProg.Step[9])/10;		
		CUstDef.IncA = (int)(CProg.Step[10])/10;				
		CUstDef.IncB = (int)(CProg.Step[11])/10;						
	}

CProg.CurProg = 1;
CProg.NomStep = 0;
CProg.NomAllStep = 0;	
}
///////////////////////////////////////////////////////////////
//Режим №0 Калибровка
///////////////////////////////////////////////////////////////
void Key_R0(uint32_t l_Key)
{
	switch(l_Key)
	{
		case IDC_BUTTON_Ent:							//Enter
//			g_Active = CUstDef.MinSize;
//			PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MaxSizeD10);IDC_DN_HI
			PostMessage(0, WM_MOTION, IDC_DN_HI, 0);
			break;
		case IDC_BUTTON_Up:                                    //^
//			g_Active = CUstDef.MinSize;
//			PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MaxSizeD10);
			PostMessage(0, WM_MOTION, IDC_DN_HI, 0);		
			break;
		case IDC_BUTTON_Esc:								//Escape   //остановка при нажатии
			PostMessage(0, WM_MOTION, IDC_STP, 0);
			break;
		case IDC_BUTTON_Dn:                                    //_
//			g_Active = CUstDef.MaxSize;
//			PostMessage(0, WM_MOTION, IDC_RUN, CUstDef.MinSizeD10);
			PostMessage(0, WM_MOTION, IDC_UP_HI, 0);		
			break;
		case IDC_BUTTON_o:									//Изменение состояния муфты
			PostMessage(0, WM_MOTION, IDC_CLUTCH, 0);
			break;
		case IDC_LINK:                            //Изменение режима при выполнении привязки
			Key = Key_R1;
			g_Active = CUstDef.Mark;
			g_Regim = 1;
			break;
		case IDC_ON_LINK:                            //Изменение режима при наезде на концевик привязки
			g_Active = CUstDef.Mark;
			break;		
		case IDC_RUN_BLOCK:                            //Изменение режима при движении
			Key = Key_Cl;
			break;
		default:
			break;
	}
	toDisplay(&CProg, &CInput, &CInputFlick);	
}
