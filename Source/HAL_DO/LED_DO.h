enum OUT_LED {IDC_CHECK0, IDC_CHECK1, IDC_CHECK2};
enum  STATE {BST_UNCHECKED, BST_CHECKED};

#define LED0_PORT			GPIOD
#define LED0_PIN			GPIO_Pin_2

#define LED1_PORT			GPIOB
#define LED1_PIN			GPIO_Pin_1

#define LED2_PORT			GPIOB
#define LED2_PIN			GPIO_Pin_2

#define LED3_PORT			GPIOB
#define LED3_PIN			GPIO_Pin_5

void LED_Init(void);
void CheckDlgButton(uint32_t g_hWnd, enum OUT_LED OutDO, enum STATE state);
