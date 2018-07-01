
struct CRunStepS{
uint8_t AddSub;														//Состояние автомата выполнения шага
int32_t gHSpd;														//Номер шага
int32_t RezStep;													//Значение шага, которое необходимо получить
};
#define CRUNSTEP_INIT {3, 0, 0}

extern struct CRunStepS CRunStep;
//void EXTERN_INTER( HWND );
void xRunSend(int32_t);
void xStpSend(int32_t);
//void Stop();

//int CALLBACK ZATLSTOP(HWND,UINT,UINT,DWORD);
