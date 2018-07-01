typedef enum MOTOR {DnLSpd, DnHSpd, UpHSpd, UpLSpd, StpSpd} MOTOR;
typedef enum CLUTCH {CLUTCH_ON, CLUTCH_OFF} CLUTCH;

#define THREE_SPEED	//FOUR_SPEED, TWO_SPEED

#define UpHSpd_PORT			GPIOA
#define UpHSpd_PIN			GPIO_Pin_0

#define UpLSpd_PORT			GPIOA
#define UpLSpd_PIN			GPIO_Pin_1

#define DnHSpd_PORT			GPIOA
#define DnHSpd_PIN			GPIO_Pin_2

#define DnLSpd_PORT			GPIOA
#define DnLSpd_PIN			GPIO_Pin_3

#define CLUTCH_PORT			GPIOB
#define CLUTCH_PIN			GPIO_Pin_0

void DO_Init(void);
void Direction(enum MOTOR MState);
void ToggleCoupling(void);
void SetCoupling (enum CLUTCH CState);
enum CLUTCH GetCoupling(void);
