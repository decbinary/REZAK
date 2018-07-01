/****************************************/
/*          Очередь на массиве        */
/****************************************/
 
#ifndef _QUEUE_ARRAY_H_
#define _QUEUE_ARRAY_H_
 
/*Размер очереди*/
#define SIZE_QUEUE_ARRAY 3
 
/**********************************/
extern int errorQueueArray; //Переменная ошибки
 
typedef enum UINT {WM_INITDIALOG, WM_CLOSE, WM_COMMAND, WM_OPERATION, WM_MOTION} UINT;

typedef struct {
UINT uMsg;
int wParam;
int lParam;
} queueArrayBaseType; //Базовый тип очереди
 
/*Дискриптор очереди*/
typedef struct {
    queueArrayBaseType buf[SIZE_QUEUE_ARRAY];               // Буфер очереди
    unsigned ukEnd;                                         // Указатель на хвост
    unsigned ukBegin;                                       // Указатель на голову
    unsigned len;                                           // Длинна очереди
} QueueArray;
/********************/
 
/*Прототипы для работы*/
void initQueueArray(QueueArray *F);                             // Инициализация очереди
void putQueueArray(QueueArray *F, queueArrayBaseType *E);       // Включить в очередь
void getQueueArray(QueueArray *F, queueArrayBaseType *E);       // Исключить из очереди
int isFullQueueArray(QueueArray *F);                            // Предикат (Проверка полна ли очередь)
int isEmptyQueueArray(QueueArray *F);                           // Предикат (Проверка пуста ли очередь)
/***************************/
 
#endif
