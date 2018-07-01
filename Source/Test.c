#include "Test.h"
#include "main_proto.h"
#include "7seg.h"
#include "Keypad.h"
#include "tm_stm32_hd44780.h"
#include "rotary_encoder.h"
#include "spi_ee.h"
#include "Queue.h"
#include "KEYBOARD.h"

#include <stdio.h>
#include <string.h>

/*-------- LCD VARIABLES -----------*/
/* Rectangle for custom character */
/* xxx means doesn't care, lower 5 bits are important for LCD */
uint8_t customChar[] = {
	0x1F,	/*  xxx 11111 */
	0x11,	/*  xxx 10001 */
	0x11,	/*  xxx 10001 */
	0x11,	/*  xxx 10001 */
	0x11,	/*  xxx 10001 */
	0x11,	/*  xxx 10001 */
	0x11,	/*  xxx 10001 */
	0x1F	/*  xxx 11111 */
};

/* ---------- EEPROM VARIABLES --------*/
/* Private define ------------------------------------------------------------*/
#define sEE_WRITE_ADDRESS        0x50
#define sEE_READ_ADDRESS         0x50
#define BUFFER_SIZE             (countof(Tx_Buffer)-1)
#define FAILED 0 
#define PASSED 1
/* Private variables ---------------------------------------------------------*/
uint8_t Tx_Buffer[] = "/* STM32L1xx SPI Firmware Library EEPROM driver example:" \
                        "This firmware provides a basic example of how to use the SPI firmware library and"\
                        "an associate SPI EEPROM driver to communicate with an SPI EEPROM device" \
                        "SPI peripheral is configured in Master Mode during write operation and" \
                        "read operation from SPI EEPROM.";

uint8_t Rx_Buffer[BUFFER_SIZE]; 
uint8_t TransferStatus1 = 0, TransferStatus2 = 0;
volatile uint16_t NumDataRead = 0;

/*--------- KEY VARIABLES ------------*/
extern QueueArray KeypadQueue;

/*--------- ENCODER VARIABLES ------------*/
extern int g_Active;
/**
  * @brief Тестирование EEPROM.
  **/

void sEE_TEST(void)
{
	  /* First write in the memory followed by a read of the written data --------*/
  /* Write on SPI EEPROM from sEE_WRITE_ADDRESS1 */
  sEE_WriteBuffer(Tx_Buffer, sEE_WRITE_ADDRESS, BUFFER_SIZE); 

  /* Wait for EEPROM standby state */
  sEE_WaitEepromStandbyState();  
  
  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE;
  
  /* Read from SPI EEPROM from sEE_READ_ADDRESS1 */
  sEE_ReadBuffer(Rx_Buffer, sEE_READ_ADDRESS, (uint16_t *)(&NumDataRead)); 
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BUFFER_SIZE);
  /* TransferStatus1 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus1 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */
		 
		 
		 while(1);
}

/**
  * @brief Тестирование EEPROM.
  **/

void sEE_ERASE(void)
{
uint8_t Tx_Buffer[(STEP_IN_PROG +  1) * BYTE_IN_STEP];
uint32_t n;
//uint16_t sEE_WRITE_ADDRESS = 0;
int32_t EmpStep = EMPTY_STEP;
uint8_t EmpLow = (uint8_t)(EmpStep & (0x000000ff));
uint8_t	EmpHi = (uint8_t)((EmpStep & (0x0000ff00)) >> 8);
	
	for (n = 0; n < (STEP_IN_PROG +  1) * BYTE_IN_STEP; n += 2)
	{
		Tx_Buffer[n] = EmpLow;
		Tx_Buffer[n + 1] = EmpHi;
	}
	
	for (n = 0; n < PROG_IN_MEMORY; n++)
	{	
		sEE_WriteBuffer(Tx_Buffer, (STEP_IN_PROG +  1) * BYTE_IN_STEP * n, (STEP_IN_PROG +  1) * BYTE_IN_STEP); 
		/* Wait for EEPROM standby state */
		sEE_WaitEepromStandbyState();  
	}
		 while(1);
}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}


/**
  * @brief Тестирование LCD.
  **/
void LCD_TEST(void)
{
	/* Put string to LCD */
	TM_HD44780_Puts(0, 0, "STM32F10x");
	TM_HD44780_Puts(0, 1, "ABCDEFGHJKLM");


	/* Wait a little */
	Delay(3000);
	
	/* Clear LCD */
	TM_HD44780_Clear();
	
	/* Show cursor */
	TM_HD44780_CursorOn();
	
	/* Write new text */
	TM_HD44780_Puts(0, 0, "CLEARED!");
	
	/* Wait a little */
	Delay(1000);
	
	/* Enable cursor blinking */
	TM_HD44780_BlinkOn();
	
	/* Show custom character at x = 1, y = 2 from RAM location 0 */
	TM_HD44780_PutCustom(1, 1, 0);
}

/**
  * @brief Тестирование клавиатуры + LCD.
  **/
void KEY_TEST(void)
{
	queueArrayBaseType tmp;
	char buf[16];
	
	while(1)
	{
		ScanKeypad();
		UpdateKeypad();
	
		while (isEmptyQueueArray(&KeypadQueue) == 0)
		{
			getQueueArray(&KeypadQueue, &tmp);
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "0x%x",tmp.wParam);
			TM_HD44780_Clear();
			TM_HD44780_Puts(0, 0, buf);
		}
	}
}

void ENCODER_TEST(void)
{
	char buf[16];
	g_Active = 0;
	while(1)
	{
		Delay(1000);
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%d",g_Active);
			TM_HD44780_Clear();
			TM_HD44780_Puts(0, 0, buf);
	}
}
