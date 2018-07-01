typedef enum KNIFE {KnfUp, KnfDn, KnfRtn} KNIFE;

#define Knf_PORT			GPIOC
#define Knf_PIN				GPIO_Pin_6

void DI_Init(void);
void SetKnife(void);
