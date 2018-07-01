/**
 * @defgroup TM_ROTARY_ENCODER_Typedefs
 * @brief    Library Typedefs
 * @{
 */
typedef enum LINK {NoLink, OnLink, FinalLink, YsLink} LINK;

extern int32_t g_Active;
extern enum LINK MLink;
/**
 * @brief  Rotary encoder rotation status
 */
#define b_A_Pin				GPIO_Pin_2
#define b_A_PinSourse GPIO_PinSource2

#define b_B_Pin 			GPIO_Pin_3
#define b_B_PinSourse GPIO_PinSource3

#define b_Port  			GPIOC
#define b_PortSourse	GPIO_PortSourceGPIOC

/**
 * @}
 */

/**
 * @defgroup ROTARY_ENCODER_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Prepare Rotary Encoder to work
 * @param  void
 * @retval None
 */
void RE_Init(void);

/**
 * @brief  Process function.
 * @note   This function have to be called inside your interrupt handler.
 * @param  void
 * @retval None
 */
void RE_Process(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */



