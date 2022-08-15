#ifndef ADC_H
#define ADC_H

#define LEFT_X_GPIO GPIOA
#define LEFT_X_PIN GPIO_Pin_6

#define LEFT_Y_GPIO GPIOA
#define LEFT_Y_PIN GPIO_Pin_5

#define RIGHT_X_GPIO GPIOA
#define RIGHT_X_PIN GPIO_Pin_4

#define RIGHT_Y_GPIO GPIOC
#define RIGHT_Y_PIN GPIO_Pin_7

#define POT_A_GPIO GPIOD
#define POT_A_PIN GPIO_Pin_3

#define POT_B_GPIO GPIOD
#define POT_B_PIN GPIO_Pin_2

#define POT_C_GPIO GPIOD
#define POT_C_PIN GPIO_Pin_1

#define POT_D_GPIO GPIOD
#define POT_D_PIN GPIO_Pin_4

#define BAT_MON_GPIO GPIOC
#define BAT_MON_PIN GPIO_Pin_4

#define ADC_AVERAGE_NUMBER 10
#define ADC_BUFFER_SIZE 10
#define DEFAULT_ADC_REFERENCE 1.2234      //1.2240V

extern uint16_t ADC_Results[ADC_BUFFER_SIZE];
extern FlagStatus SYSTEM_ADC_UpdateFlag;

void TIM2_Update_ISR(void);
void SYSTEM_ADC_DMA_ISR(void);
void Analog_Init(void);

#endif