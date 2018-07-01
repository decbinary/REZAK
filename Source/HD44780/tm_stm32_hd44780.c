/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2016 Tilen Majerle
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32_hd44780.h"

const unsigned char russian[]={ 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45,
0xA3, 0xA4, 0xA5, 0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50,
0x43, 0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD,0xAE,
0x62, 0xAF, 0xB0, 0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6,
0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63,
0xBF, 0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4,
0xC5, 0xC6, 0xC7 };

/* Private HD44780 structure */
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} HD44780_Options_t;

/* Private functions */
static void TM_HD44780_InitPins(void);
static void TM_HD44780_Cmd(uint8_t cmd);
static void TM_HD44780_Cmd4bit(uint8_t cmd);
static void TM_HD44780_Data(uint8_t data);
static void TM_HD44780_CursorSet(uint8_t col, uint8_t row);

/* Private variable */
static HD44780_Options_t HD44780_Opts;
int Time = 0;

/* Pin definitions */
#define HD44780_RS_LOW              GPIO_ResetBits(HD44780_RS_PORT, HD44780_RS_PIN)
#define HD44780_RS_HIGH             GPIO_SetBits(HD44780_RS_PORT, HD44780_RS_PIN)
#define HD44780_E_LOW               GPIO_ResetBits(HD44780_E_PORT, HD44780_E_PIN)
#define HD44780_E_HIGH              GPIO_SetBits(HD44780_E_PORT, HD44780_E_PIN)

#define HD44780_E_BLINK             HD44780_E_HIGH; HD44780_Delay(20); HD44780_E_LOW; HD44780_Delay(20)
#define HD44780_Delay(x)            Time = x * (SystemCoreClock / 1000000)/6; while (Time > 0){Time--;}//Delay in uS //Delay(x)

/* Commands*/
#define HD44780_CLEARDISPLAY        0x01
#define HD44780_RETURNHOME          0x02
#define HD44780_ENTRYMODESET        0x04
#define HD44780_DISPLAYCONTROL      0x08
#define HD44780_CURSORSHIFT         0x10
#define HD44780_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR        0x40
#define HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define HD44780_DISPLAYON           0x04
#define HD44780_CURSORON            0x02
#define HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAYMOVE         0x08
#define HD44780_CURSORMOVE          0x00
#define HD44780_MOVERIGHT           0x04
#define HD44780_MOVELEFT            0x00

/* Flags for function set */
#define HD44780_8BITMODE            0x10
#define HD44780_4BITMODE            0x00
#define HD44780_2LINE               0x08
#define HD44780_1LINE               0x00
#define HD44780_5x10DOTS            0x04
#define HD44780_5x8DOTS             0x00

void TM_HD44780_Init(uint8_t cols, uint8_t rows) {
	
	/* Init pinout */
	TM_HD44780_InitPins();
	
	/* At least 40ms */
	HD44780_Delay(45000);
	
	/* Set LCD width and height */
	HD44780_Opts.Rows = rows;
	HD44780_Opts.Cols = cols;
	
	/* Set cursor pointer to beginning for LCD */
	HD44780_Opts.currentX = 0;
	HD44780_Opts.currentY = 0;
	
	HD44780_Opts.DisplayFunction = HD44780_4BITMODE | HD44780_5x8DOTS | HD44780_1LINE;
	if (rows > 1) {
		HD44780_Opts.DisplayFunction |= HD44780_2LINE;
	}
	
	/* Try to set 4bit mode */
	TM_HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);
	
	/* Second try */
	TM_HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);
	
	/* Third goo! */
	TM_HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);	
	
	/* Set 4-bit interface */
	TM_HD44780_Cmd4bit(0x02);
	HD44780_Delay(100);
	
	/* Set # lines, font size, etc. */
	TM_HD44780_Cmd(HD44780_FUNCTIONSET | HD44780_Opts.DisplayFunction);

	/* Turn the display on with no cursor or blinking default */
	HD44780_Opts.DisplayControl = HD44780_DISPLAYON;
	TM_HD44780_DisplayOn();

	/* Clear lcd */
	TM_HD44780_Clear();

	/* Default font directions */
	HD44780_Opts.DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
	TM_HD44780_Cmd(HD44780_ENTRYMODESET | HD44780_Opts.DisplayMode);

	/* Delay */
	HD44780_Delay(4500);
}

void TM_HD44780_Clear(void) {
	TM_HD44780_Cmd(HD44780_CLEARDISPLAY);
	HD44780_Delay(3000);
}

void TM_HD44780_Puts(uint8_t x, uint8_t y, char* str) {
	TM_HD44780_CursorSet(x, y);
	while (*str) {
		if (HD44780_Opts.currentX >= HD44780_Opts.Cols) {
			HD44780_Opts.currentX = 0;
			HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		}
		if (*str == '\n') {
			HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		} else if (*str == '\r') {
			TM_HD44780_CursorSet(0, HD44780_Opts.currentY);
		} else {
			if (*str > 191)
				TM_HD44780_Data(russian[*str - 192]);
			else
				TM_HD44780_Data(*str);
			HD44780_Opts.currentX++;
		}
		str++;
	}
}

void TM_HD44780_DisplayOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_DISPLAYON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_DisplayOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_DISPLAYON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_BlinkOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_BLINKON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_BlinkOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_BLINKON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_CursorOn(void) {
	HD44780_Opts.DisplayControl |= HD44780_CURSORON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_CursorOff(void) {
	HD44780_Opts.DisplayControl &= ~HD44780_CURSORON;
	TM_HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}

void TM_HD44780_ScrollLeft(void) {
	TM_HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
}

void TM_HD44780_ScrollRight(void) {
	TM_HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
}

void TM_HD44780_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	TM_HD44780_Cmd(HD44780_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		TM_HD44780_Data(data[i]);
	}
}

void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	TM_HD44780_CursorSet(x, y);
	TM_HD44780_Data(location);
}

/* Private functions */
static void TM_HD44780_Cmd(uint8_t cmd) {
	/* Command mode */
	HD44780_RS_LOW;
	
	/* High nibble */
	TM_HD44780_Cmd4bit(cmd >> 4);
	/* Low nibble */
	TM_HD44780_Cmd4bit(cmd & 0x0F);
}

static void TM_HD44780_Data(uint8_t data) {
	/* Data mode */
	HD44780_RS_HIGH;
	
	/* High nibble */
	TM_HD44780_Cmd4bit(data >> 4);
	/* Low nibble */
	TM_HD44780_Cmd4bit(data & 0x0F);
}

static void TM_HD44780_Cmd4bit(uint8_t cmd) {
	/* Set output port */
	GPIO_WriteBit(HD44780_D7_PORT, HD44780_D7_PIN, (BitAction)(cmd & 0x08 ? 1 : 0));
	GPIO_WriteBit(HD44780_D6_PORT, HD44780_D6_PIN, (BitAction)(cmd & 0x04 ? 1 : 0));
	GPIO_WriteBit(HD44780_D5_PORT, HD44780_D5_PIN, (BitAction)(cmd & 0x02 ? 1 : 0));
	GPIO_WriteBit(HD44780_D4_PORT, HD44780_D4_PIN, (BitAction)(cmd & 0x01 ? 1 : 0));
	HD44780_E_BLINK;
}

static void TM_HD44780_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Go to beginning */
	if (row >= HD44780_Opts.Rows) {
		row = 0;
	}
	
	/* Set current column and row */
	HD44780_Opts.currentX = col;
	HD44780_Opts.currentY = row;
	
	/* Set location address */
	TM_HD44780_Cmd(HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}

static void TM_HD44780_InitPins(void) {
	/* Init all pins */
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Set pins low */
	GPIO_ResetBits(HD44780_RS_PORT, HD44780_RS_PIN);
	GPIO_ResetBits(HD44780_E_PORT, HD44780_E_PIN);
	GPIO_ResetBits(HD44780_D4_PORT, HD44780_D4_PIN);
	GPIO_ResetBits(HD44780_D5_PORT, HD44780_D5_PIN);
	GPIO_ResetBits(HD44780_D6_PORT, HD44780_D6_PIN);
	GPIO_ResetBits(HD44780_D7_PORT, HD44780_D7_PIN);
}
