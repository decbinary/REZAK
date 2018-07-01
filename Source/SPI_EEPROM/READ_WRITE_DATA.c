//#include <stdio.h>
//#include <windows.h>
#include <stdint.h>
#include "CRC16.h"
#include "KEYBOARD.h"
#include "READ_WRITE_DATA.h"
#include "spi_ee.h"

///////////////////////////////////////////////////////////////
//Проверки для чтения программы
///////////////////////////////////////////////////////////////
void toReadProgram(struct CProgS* CProg, int nomProg)
{
	int n;

	if((nomProg > 0 && nomProg < 100) == 0)
	{
		init_CInput(0, 0, 0);
		nomProg = 1;
	}

	if (ReadCurProg(CProg, nomProg) == 1)
	{
		Error(5);
	}

	if (CProg->NomAllStep == 0)
	{
		CProg->NomStep = 0;
	}
	else
	{
		CProg->NomStep = 1;
	}

	for (n = 1; n < CProg->NomAllStep; n++)
	{
		if ((CProg->Step[n] > (unsigned int)CUstDef.MinSizeD10 && CProg->Step[n] < (unsigned int)CUstDef.MaxSizeD10) == 0)
		{
			Error(5);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////
//Чтение программы из памяти
///////////////////////////////////////////////////////////////
int ReadCurProg(struct CProgS *CProg, unsigned int l_CurProg)
{
  unsigned char l_Mas[STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP];
  unsigned char l_Temp[6];
  unsigned int n,k;
  unsigned short l_Crc;

    CProg->NomAllStep = 0;
    CProg->CurProg = l_CurProg;
    SPI_Read((unsigned int) ((STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP) * l_CurProg), l_Mas, (STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP));
	for(n = BYTE_IN_STEP, k = 1; n < STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP; n += BYTE_IN_STEP, k++ )
	{
	  CProg->Step[k] = (unsigned int) l_Mas[n + 1];
	  CProg->Step[k] <<= 8;
	  CProg->Step[k] |= (unsigned int) l_Mas[n];

	  if (CProg->Step[k] == EMPTY_STEP)
        break;
	}

    if (k == 1 || k > STEP_IN_PROG + 1)
    {
      CProg->NomAllStep = 0;
      CProg->Step[0] = EMPTY_STEP;
    }
    else 
      CProg->NomAllStep = k - 1;

    if (CProg->NomAllStep < STEP_IN_PROG)
    {
      for (n = k + 1; n < STEP_IN_PROG + 1; n++)
      {
        CProg->Step[n] = EMPTY_STEP;
      }
    }

    if (CProg->NomAllStep != 0)
	{
	  CProg->Step[0] = (unsigned int) l_Mas[1];
	  CProg->Step[0] <<= 8;
	  CProg->Step[0] |= (unsigned int) l_Mas[0];

		l_Crc = crc16(l_Mas + 2, (unsigned short) ((CProg->NomAllStep) * 2));
//		g_Required =(unsigned int) crc16_int(g_Step,g_NomAllStep);
	  if(l_Crc != (unsigned short) (CProg->Step[0]))                   //Программа повреждена
	  {
        CProg->Step[0] = l_Crc;

//	    Error(5);//g_Error = 5;

	    l_Temp[0] = (unsigned char)(l_Crc & 0x00FF);
	    l_Temp[1] = (unsigned char)((l_Crc >> 8)& 0x00FF);

		SPI_Write((unsigned int) ((STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP) * l_CurProg), l_Temp, BYTE_IN_STEP);
        return 1;
	  }
	}
  return 0;
}

///////////////////////////////////////////////////////////////
//Запись программы в память
///////////////////////////////////////////////////////////////
void WriteCurProg(struct CProgS *CProg)
{
  unsigned char l_Mas[STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP];
//  unsigned char l_Temp[6];
  unsigned int n,k;
  unsigned short l_Crc = 0;


  if (CProg -> NomAllStep < STEP_IN_PROG)
  {
    for (n = CProg -> NomAllStep + 1; n < STEP_IN_PROG + 1; n++)
      CProg -> Step[n] = EMPTY_STEP;
  }
  else if (CProg -> NomAllStep > STEP_IN_PROG)
      CProg -> NomAllStep = STEP_IN_PROG;

  for(n = 0, k = 0; n < STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP; n += BYTE_IN_STEP, k++ )
  {
    l_Mas[n] = (unsigned char)((CProg -> Step[k]) & 0x000000ff);
    l_Mas[n + 1] = (unsigned char) ((CProg -> Step[k] >> 8) & 0x000000ff);	
  }

  if (CProg -> NomAllStep != 0)
  {
    l_Crc = crc16(l_Mas + 2, (unsigned short) ((CProg -> NomAllStep) * BYTE_IN_STEP));
    
    if (l_Crc == CProg -> Step[0])
      return;
    
    CProg -> Step[0] = (unsigned short) l_Crc;

    l_Mas[0] = (unsigned char)(l_Crc & 0x00FF);
		l_Mas[1] = (unsigned char)((l_Crc >> 8)& 0x00FF);
  }

  SPI_Write((unsigned int) ((STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP) * (CProg -> CurProg)), l_Mas, (STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP));
  return;
}  

///////////////////////////////////////////////////////////////
//Запись шага в память  !!!!!!!!!!!!
///////////////////////////////////////////////////////////////
void WriteCurStep(struct CProgS *CProg)
{
  unsigned char l_Mas[2];
  unsigned short l_Crc;

//  l_Mas[0] = (unsigned char) (CProg -> Step[l_NomStep] & 0x000000ff);
//  l_Mas[1] = (unsigned char) ((CProg -> Step[l_NomStep] >> 8) & 0x000000ff);
  l_Mas[0] = (unsigned char) (CProg -> Step[CProg -> NomStep] & 0x000000ff);
  l_Mas[1] = (unsigned char) ((CProg -> Step[CProg -> NomStep] >> 8) & 0x000000ff);

  SPI_Write((unsigned int) ((STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP) * (CProg -> CurProg) + (CProg -> NomStep) * BYTE_IN_STEP), l_Mas, BYTE_IN_STEP);

  l_Crc = crc16_int(CProg -> Step + 1, (unsigned short) (CProg -> NomAllStep));
  CProg -> Step[0] = (unsigned short) l_Crc;

  l_Mas[0] = (unsigned char)(l_Crc & 0x00FF);
  l_Mas[1] = (unsigned char)((l_Crc >> 8)& 0x00FF);

  SPI_Write((unsigned int) ((STEP_IN_PROG * BYTE_IN_STEP + BYTE_IN_STEP) * (CProg -> CurProg)), l_Mas, (unsigned int) BYTE_IN_STEP);
}


void SPI_Read(unsigned int l_AdrProg, unsigned char * l_Mas, unsigned int l_NumByte)
{
	/*
	FILE *file;
    char* file_name = "file.txt";
    fpos_t pos;
  	
    file = fopen( file_name, "rb" );
	fgetpos (file,&pos);
	pos = pos + l_AdrProg;
	fsetpos (file,&pos);
	if( file != 0 )
	{
	    fread( l_Mas,1, l_NumByte , file ); 
	}
	fclose(file);
	*/
	uint16_t NumByte = (uint16_t)l_NumByte;
sEE_ReadBuffer(l_Mas, l_AdrProg, &NumByte);
	
}

void SPI_Write(unsigned int l_AdrProg, unsigned char * l_Mas, unsigned int l_NumByte)
{
	/*
    FILE *file;
    char* file_name = "file.txt";
    fpos_t pos;
  	
    file = fopen( file_name, "rb+" );
	fgetpos (file,&pos);
	pos = pos + l_AdrProg;
	fsetpos (file,&pos);
	if( file != 0 )
	{
	    fwrite( l_Mas,1, l_NumByte , file ); 
	}
    fclose(file);
	*/
sEE_WriteBuffer(l_Mas, l_AdrProg, l_NumByte);
}
