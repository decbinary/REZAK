#include "Queue.h"
#include "PostMessage.h"

extern QueueArray KeypadQueue;

//void SendQueue(void)
int PostMessage(int hWnd, UINT uMsg, int wParam, int lParam)
{
queueArrayBaseType lMessage;

lMessage.uMsg = uMsg;
lMessage.wParam = wParam;
lMessage.lParam = lParam;
putQueueArray(&KeypadQueue, &lMessage);
return 0;
}
