#ifndef ENCODER_H
#define ENCODER_H

#define ENCODER_A_GPIO GPIOD
#define ENCODER_A_PIN GPIO_Pin_5

#define ENCODER_B_GPIO GPIOD
#define ENCODER_B_PIN GPIO_Pin_6

#define ENCODER_BUTTON_GPIO GPIOA
#define ENCODER_BUTTON_PIN GPIO_Pin_7

#define ENCODER_BEEPER_GPIO GPIOA
#define ENCODER_BEEPER_PIN GPIO_Pin_0

typedef enum
{
  Key_Null = (uint8_t)0x00,  
  Key_Push  =  (uint8_t)0x01,
  Key_Increase  =  (uint8_t)0x02,
  Key_Decrease  =  (uint8_t)0x04,
}
Key_TypeDef;

extern Key_TypeDef Encoderkey;
extern FlagStatus KeyStatus;

void ENCODER_Init(void);
void ENCODER_BeeperBeep(uint8_t time10ms);
void ENCODER_BeeperClickSound(void);
void ENCODER_Push_ISR(void);
void ENCODER_Rotate_ISR(void);

#endif