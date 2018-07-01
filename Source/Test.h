#include "stdint.h"
/* --- LCD TEST ---*/
void LCD_TEST(void);

/* --- KEYBOARD + LCD TEST ---*/
void KEY_TEST(void);

/* --- ENCODER TEST ---*/
void ENCODER_TEST(void);

/* --- EEPROM TEST ---*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

void sEE_TEST(void);
void sEE_ERASE(void);
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
