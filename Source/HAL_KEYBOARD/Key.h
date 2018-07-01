#define CHAR_NAME   4

typedef enum KeyState {IDLE, PRESSED, HOLD, RELEASED} KeyState;
typedef enum KeyChange {NO_CHANGE, CHANGED} KeyChange;

typedef struct
{
    char kname[CHAR_NAME];  //Имя клавиши
    int kid;								//Уникальный ID
    KeyState kstate;				//Текущее состояние клавиши
    KeyChange kchange;			//Флаг изменения состояния клавиши
}OneKey;

void SetKey (OneKey* CurrentKey, char* userKeyChar, int kid, KeyState kstate, KeyChange kchange);
