#define TRUE		1
#define FALSE		0
#define BOOL		int32_t

#define PROG_IN_MEMORY	100
#define STEP_IN_PROG		20
#define BYTE_IN_STEP		2
#define EMPTY_STEP			(int32_t)0x0000FFFF

#define NUM_TO_POINT 4

#define FLICK_TIME  4
#define	FLICK_ERROR 8

#define	ACTIVE_10		g_Active/10	//Размер при пониженой точности
#define	STOP				(int32_t)-1

extern struct CDispS CDisp;
extern struct CProgS CProg;
extern struct CUstDefS CUstDef;
extern struct CInputS CInput;
extern struct CInputFlickS CInputFlick;
extern struct CPreciseS CPrecise;

extern int g_Active;

extern unsigned int g_Regim;
extern unsigned int g_Error;

extern unsigned int g_OffMotion;

extern BOOL b_NoKeyP;

///*
#ifndef H_KEYBOARD
#define H_KEYBOARD

struct CInputS{
int Numeric;					//ВВодимое значение (умноженое на 10 для простоты вычисления значения шага)
int NumericInt;				//ВВодимое значение
unsigned char Count;	//Счетчик цифр в числе (0-4)
BOOL Point;						//Нажатие точки
};
#define CINPUT_INIT {0, 0, 0}

enum flick {fNoFlick, fCurProg, fNomStep, fStep};
struct CInputFlickS{
enum flick type;
int  timeflick;				//Время мигания шага
};
#define CINPUTFLICK_INIT {fNoFlick, 0}

struct CProgS{
int CurProg;														//Номер программы
int NomStep;														//Номер шага
int NomAllStep;													//Количество шагов в программе
unsigned int Step[(STEP_IN_PROG + 1)];	//Сама программа
};
#define  CPROG_INIT {1,0,0}

struct CUstDefS{
int	Mark;						//Размер при привязке	
int MaxSizeD10;			//Максимальный размер
int MinSizeD10;			//Минимальный размер

//int MaxSize;				//Максимальный размер
//int MinSize;				//Минимальный размер
int DnSpd;					//Расстояние перехода на малую скорость
int DnDist;					//Расстояние перехода с малой на большую от текущей позиции
int Stop;						//Расстояние включение тормоза
int Cross;					//Расстояние заезда за точку
int Acur;						//Точность
int TimeStop;				//Время в мС после которого определяется остановка

int	IncA;						//Инкремент канала A
int IncB;						//Инкремент канала B
};
#define CUSTDEF_INIT {30000, 30000, 300, 3000, 3000, 100, 3000, 10, 200, 1, 1}

struct CDispS{
unsigned int Required;	//Значение шага
unsigned int CurProg;		//Номер программы
unsigned int NomStep;		//Номер шага
unsigned int Numeric;		//Вводимое значение
unsigned int Error;			//Ошибка
unsigned int Regim;			//Режим
BOOL Point;							//Точка
BOOL NoKeyP;						//Флаг выбора программы
//BOOL Flag;						//Дополнительный флаг
};// CDisp;

struct CPreciseS{
int FActive;					//Число которое необходимо показывать
int Show;						//Флаг индикации значения
};
#define CPRECISE_INIT {0, 0}

#endif
//*/

extern void (*Key)(uint32_t );
void ReadServise(void);		//чтение юстировочных коэффициентов
void Key_R0(uint32_t );		//режим калибровки
void Key_R1(uint32_t );		//ручной режим
void Key_R2(uint32_t );		//режим выполнения
void Key_R3(uint32_t );		//режим программирования
void Key_RPS(uint32_t );		//режим редактирования шага
void Key_RPP(uint32_t );		//режим программирования программы
void Key_RD(uint32_t );		//режим удаления ????
void Key_SK(uint32_t );		//режим второй клавиши
void Key_Cl(uint32_t );		//режим движения
void Key_Er(uint32_t );		//режим ошибки

void Error(unsigned char );
int Number(unsigned char , struct CInputS*);
void toDisplay(struct CProgS*, struct CInputS*, struct CInputFlickS*);
/*inline*/ void init_CInput(int , BOOL , BOOL );
/*inline*/ void init_CInputFlick(enum flick , int );
