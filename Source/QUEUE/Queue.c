/****************************************/
/*          Очередь на массиве          */
/****************************************/
 
#include <stdio.h>
#include "Queue.h"
 
/*Переменная ошибок*/
int errorQueueArray;
 
/*Описание исключительных ситуаций*/
const int okQueueArray = 0;                                 // Все нормально
const int fullQueueArray = 1;                               // Очередь переполнена
const int emptyQueueArray = 2;                              // Очередь пуста

/*Инициализация очереди*/
void initQueueArray(QueueArray *F) 
{
    F->ukBegin = 0;
    F->ukEnd = 0;
    F->len = 0;
    errorQueueArray = okQueueArray;
}
 
/*Включение в очередь*/
void putQueueArray(QueueArray *F, queueArrayBaseType *E) 
{
 
    /*Если очередь переполнена*/
    if (isFullQueueArray(F)) 
    {
        return;
    }
    /*Else*/
    F->buf[F->ukEnd] = *E;                           // Включение элемента
    F->ukEnd = (F->ukEnd + 1) % SIZE_QUEUE_ARRAY;    // Сдвиг указателя
    F->len++;                                        // Увеличение количества элементов очереди
}
 
/*Исключение из очереди*/
void getQueueArray(QueueArray *F, queueArrayBaseType *E) 
{
    /*Если очередь пуста*/
    if (isEmptyQueueArray(F)) 
    {
        return;
    }
    /*Else*/
    *E = F->buf[F->ukBegin];                           // Запись элемента в переменную
    F->ukBegin = (F->ukBegin + 1) % SIZE_QUEUE_ARRAY;  // Сдвиг указателя
    F->len--;                                          // Уменьшение длины
}
 
/*Предикат: полна ли очередь*/
int isFullQueueArray(QueueArray *F) 
{
    if (F->len == SIZE_QUEUE_ARRAY) 
    {
        errorQueueArray = fullQueueArray;
        return 1;
    }
    return 0;
}
 
/*Предикат: пуста ли очередь*/
int isEmptyQueueArray(QueueArray *F) 
{
    if (F->len == 0) 
    {
        errorQueueArray = emptyQueueArray;
        return 1;
    }
    return 0;
}
