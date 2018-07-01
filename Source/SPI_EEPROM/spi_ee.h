/**
  ******************************************************************************
  * @file    stm32l152d_eval_spi_ee.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-March-2012
  * @brief   This file contains all the functions prototypes for the stm32l152d_eval_spi_ee
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152D_EVAL_SPI_EE_H
#define __STM32L152D_EVAL_SPI_EE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
	 
#define FRAM

#ifndef FRAM	 
	#define EEPROM
#endif
/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32L152D_EVAL
  * @{
  */

/** @addtogroup STM32L152D_EVAL_SPI_EEPROM
  * @{
  */  

/** @defgroup STM32L152D_EVAL_SPI_EEPROM_Exported_Types
  * @{
  */ 
/**
  * @}
  */
  
/** @defgroup STM32L152D_EVAL_SPI_EEPROM_Exported_Constants
  * @{
  */
/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_SPI_EE
  * @{
  */
/**
  * @brief  sEE SPI Interface pins
  */
#define sEE_SPI                          SPI1
#define sEE_SPI_CLK                      RCC_APB2Periph_SPI1
   
#define sEE_SPI_SCK_PIN                  GPIO_Pin_5                  /* PA.05 */
#define sEE_SPI_SCK_GPIO_PORT            GPIOA                       /* GPIOA */
#define sEE_SPI_SCK_GPIO_CLK             RCC_APB2Periph_GPIOA
#define sEE_SPI_SCK_SOURCE               GPIO_PinSource5
#define sEE_SPI_SCK_AF                   GPIO_AF_SPI1

#define sEE_SPI_MISO_PIN                 GPIO_Pin_6                 /* PE.14 */
#define sEE_SPI_MISO_GPIO_PORT           GPIOA                       /* GPIOE */
#define sEE_SPI_MISO_GPIO_CLK            RCC_APB2Periph_GPIOA
#define sEE_SPI_MISO_SOURCE              GPIO_PinSource6
#define sEE_SPI_MISO_AF                  GPIO_AF_SPI1

#define sEE_SPI_MOSI_PIN                 GPIO_Pin_7                 /* PE.15 */
#define sEE_SPI_MOSI_GPIO_PORT           GPIOA                       /* GPIOE */
#define sEE_SPI_MOSI_GPIO_CLK            RCC_APB2Periph_GPIOA
#define sEE_SPI_MOSI_SOURCE              GPIO_PinSource7
#define sEE_SPI_MOSI_AF                  GPIO_AF_SPI1

#define sEE_SPI_CS_PIN                   GPIO_Pin_4                  /* PC.05 */
#define sEE_SPI_CS_GPIO_PORT             GPIOA                       /* GPIOC */
#define sEE_SPI_CS_GPIO_CLK              RCC_APB2Periph_GPIOA   

/**
  * @brief  M95 SPI EEPROM supported commands
  */  
#define sEE_CMD_WREN           0x06  /*!< Write enable instruction */
#define sEE_CMD_WRDI           0x04  /*!< Write disable instruction */
#define sEE_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sEE_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sEE_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sEE_CMD_READ           0x03  /*!< Read from Memory instruction */

/**
 * @brief  M95040 SPI EEPROM defines
 */  
#define sEE_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define sEE_DUMMY_BYTE         0xA5

#define sEE_PAGESIZE           128


  
/**
  * @}
  */ 
  
/** @defgroup STM32L152D_EVAL_SPI_EEPROM_Exported_Macros
  * @{
  */
/**
  * @brief  Select EEPROM: Chip Select pin low
  */
#define sEE_CS_LOW()       GPIO_ResetBits(sEE_SPI_CS_GPIO_PORT, sEE_SPI_CS_PIN)
/**
  * @brief  Deselect EEPROM: Chip Select pin high
  */
#define sEE_CS_HIGH()      GPIO_SetBits(sEE_SPI_CS_GPIO_PORT, sEE_SPI_CS_PIN)   
/**
  * @}
  */



/** @defgroup STM32L152D_EVAL_SPI_EEPROM_Exported_Functions
  * @{
  */
/**
  * @brief  High layer functions
  */
void     sEE_DeInit(void);
void     sEE_Init(void);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
void     sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
#ifdef EEPROM
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t* NumByteToWrite);
#endif
uint32_t sEE_WaitEepromStandbyState(void);

/**
  * @brief  Low layer functions
  */
void sEE_SPI_LowLevel_Init(void); 	
void sEE_SPI_LowLevel_DeInit(void);
uint8_t sEE_ReadByte(void);
uint8_t sEE_SendByte(uint8_t byte);
void sEE_WriteEnable(void);
void sEE_WriteDisable(void);
void sEE_WriteStatusRegister(uint8_t regval);
uint8_t sEE_ReadStatusRegister(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L152D_EVAL_SPI_EE_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
