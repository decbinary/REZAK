// KEYBOARD.cpp: определяет точку входа для консольного приложения.
//
#include <stdio.h>
#include <tchar.h>

//#include "Key.h"
#include "Keypadproto.h"

unsigned int gTick;

int main(int argc, _TCHAR* argv[])
{
//    struct    OneKey key;
//    char name[CHAR_NAME] = {"ABC"};
        
    ScanKeypad();
	return 0;
}

