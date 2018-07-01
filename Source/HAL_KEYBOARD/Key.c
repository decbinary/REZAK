#include "Key.h"

void SetKey (OneKey* CurrentKey, char* userKeyChar, int kid, KeyState kstate, KeyChange kchange) 
{
int n;
  for (n = 0; n < CHAR_NAME; n++)
    CurrentKey->kname[n] = userKeyChar[n];
  CurrentKey->kid = kid;
  CurrentKey->kstate = kstate;
  CurrentKey->kchange = kchange;
}
