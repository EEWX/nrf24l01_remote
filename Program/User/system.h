#ifndef SYSTEM_H
#define SYSTEM_H
#include "encoder.h"

#define SYSTEM_DAC_GPIO GPIOF
#define SYSTEM_DAC_PIN GPIO_Pin_0

#define SYSTEM_PWR_HOLD_GPIO GPIOE
#define SYSTEM_PWR_HOLD_PIN GPIO_Pin_7

#define SYSTEM_PWR_KEY_GPIO GPIOE
#define SYSTEM_PWR_KEY_PIN GPIO_Pin_6

#define SYSTEM_PWR_SAVE_GPIO GPIOE
#define SYSTEM_PWR_SAVE_PIN GPIO_Pin_6

#define DEFAULT_DAC_VALUE 1784



#define SYSTEM_SOFTWARE_VERSION 0x01
#define SYSTEM_HARDWARE_VERSION 0x01

#define DEFAULT_REFERENCE_VOLTAGE 1224 //->1.224V

#define SYSTEM_ReadPowerKey() GPIO_ReadInputDataBit(SYSTEM_PWR_KEY_GPIO,SYSTEM_PWR_KEY_PIN)
#define SYSTEM_PowerOff() GPIO_ResetBits(SYSTEM_PWR_HOLD_GPIO,SYSTEM_PWR_HOLD_PIN)

void SYSTEM_Init(void);
void SYSTEM_RecallParameters(void);
void SYSTEM_SaveParameters(void);
void SYSTEM_ADC_DMA_ISR(void);
void TIM1_Update_ISR(void);
void TIM4_Update_ISR(void);
void TIM1_CC3_ISR(void);

#endif