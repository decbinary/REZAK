#include "w_strcpy.h"
#include "Queue.h"
#include "InitHal.h"

int w_strcpy(char * out_buffer /*s*/, const char * in_buffer /*utf-8 string*/)
{
	int n = 0;
	int k = 0;
	
	while ((in_buffer[n] != 0x00) || (n > (LCD_COLUMN + 1)*2))
	{
		if (in_buffer[n] < 128)
		{
			out_buffer[k] = in_buffer[n];
			k++;
			n++;
		}
		else
		if (in_buffer[n] < 2048)
		{
			out_buffer[k] = (char)((int)(((in_buffer[n] & 0x1F) * 0x40) + (in_buffer[n + 1] & 0x3F) - 0x350) & 0xFF);
			k++;
			n += 2;
		}
		else
			break;
	}
//	k++;
	out_buffer[k] = 0;
	return k;
}
